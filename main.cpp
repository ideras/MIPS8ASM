#include <cstdlib>
#include <cstdio>
#include <cstring>
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

static inline void showUsage(string programName) {
    cerr << "Usage: " << programName << " [--use-2-bit-regaddr] [--binary-format] <input file> <output file>" << endl << endl;
}

int main(int argc, char *argv[])
{
    string programName = argv[0];
    string infile, outfile;
    bool use3BitRegAddr = true;
    bool useBinaryFormat = false;
    
    if (argc > 2) {
            ++argv, --argc; /* El primer argumento es el nombre del programa */
            
            switch (argc) {
                case 2: {
                    infile = argv[0];
                    outfile = argv[1];
                    break;
                }
                case 3: {
                    if (strcmp(argv[0], "--use-2-bit-regaddr") == 0)
                        use3BitRegAddr = false;
                    else if (strcmp(argv[0], "--binary-format") == 0)
                        useBinaryFormat = true;
                    else {
                        showUsage(programName);
                        return 1;
                    }
                    infile = argv[1];
                    outfile = argv[2];
                    break;
                }
                case 4: {
                    if (strcmp(argv[0], "--use-2-bit-regaddr") == 0)
                        use3BitRegAddr = false;
                    else if (strcmp(argv[0], "--binary-format") == 0)
                        useBinaryFormat = true;
                    else {
                        showUsage(programName);
                        return 1;
                    }
                    if (strcmp(argv[1], "--use-2-bit-regaddr") == 0)
                        use3BitRegAddr = false;
                    else if (strcmp(argv[1], "--binary-format") == 0)
                        useBinaryFormat = true;
                    else {
                        showUsage(programName);
                        return 1;
                    }
                    infile = argv[2];
                    outfile = argv[3];
                    break;
                }
                default:
                    showUsage(programName);
                    return 1;
            }
            in.open(infile.c_str(), ifstream::in|ifstream::binary);
            
            if (!in.is_open()) {
                cerr << "Cannot open input file " << infile << endl << endl;
                return 0;
            }

            out.open(outfile.c_str(), ofstream::out | ofstream::trunc);

            if (!out.is_open()) {
                cerr << "Cannot open output file " << outfile << endl << endl;
                in.close();
                return 0;
            }
    }
    else {
        showUsage(programName);
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
            
            if (token != TK_EOF)
                Parse(pParser, token, tokenInfo);
        } else {
            tokenInfo = lex.getTokenInfo();
            Parse(pParser, token, tokenInfo);
        }

        token = lex.getNextToken();
    }
    Parse(pParser, TK_EOF, tokenInfo);
    Parse(pParser, 0, tokenInfo);

    ParseFree(pParser, free);

    if (errors != 0) {
        cout << "Found " << errors << " error(s)" << endl;
    } else {
        MIPS8AsmGen gen(statements, use3BitRegAddr, useBinaryFormat);

        if (!gen.genAsm(out)) {
            cout << "Failed." << endl;
        } else {
            cout << "Sucess. Output written to file '" << outfile << "'" << endl << endl;
        }
    }

    in.close();
    out.close();

    return 0;
}
