#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "lexer.h"
#include "mips8_asmgen.h"

static int errors = 0;

ListNode statements;
ifstream in;
ofstream out;
Lexer lex(&in);

using namespace std;

void reportError(string msg) {
    errors++;
    cout << "Line: " << lex.getLine() << ":" << msg << endl;
}

void *ParseAlloc(void *(*mallocProc)(size_t));
void ParseFree(void *p, void (*freeProc)(void*));
void Parse(void *yyp, int yymajor, TokenInfo yyminor);

int main(int argc, char *argv[])
{
    if (argc > 2) {
            ++argv, --argc; /* El primer argumento es el nombre del programa */
            in.open(argv[0], ifstream::in|ifstream::binary);

            if (!in.is_open()) {
                    cerr << "Cannot open input file " << argv[0] << endl << endl;
                    return 0;
            }

            out.open(argv[1], ofstream::out | ofstream::trunc);

            if (!out.is_open()) {
                    cerr << "Cannot open output file " << argv[1] << endl << endl;
                    in.close();
                    return 0;
            }
    }
    else {
            cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl << endl;
            return 0;
    }

    TokenInfo tokenInfo;
    int token;

    void* pParser = ParseAlloc (malloc);

    while ((token = lex.getNextToken()) == TK_EOL);

    while ( token != TK_EOF) {

        if (token == TK_EOL) {
            while ((token = lex.getNextToken()) == TK_EOL);

            tokenInfo = lex.getTokenInfo();

            Parse(pParser, TK_EOL, tokenInfo);
            Parse(pParser, token, tokenInfo);
        } else {
            tokenInfo = lex.getTokenInfo();
            Parse(pParser, token, tokenInfo);
        }

        token = lex.getNextToken();
    }
	Parse(pParser, TK_EOF, tokenInfo);

    ParseFree(pParser, free);

    if (errors != 0) {
        cout << "Found " << errors << " error(s)" << endl;
    } else {
        MIPS8AsmGen gen(statements);

        if (!gen.genAsm(out)) {
            cout << "Failed." << endl;
        } else {
            cout << "Sucess. Output written to file '" << argv[1] << "'" << endl << endl;
        }
    }

    in.close();
    out.close();

    return 0;
}
