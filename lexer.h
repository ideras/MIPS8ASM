// Analex.h: interface for the Lexer class.
//

#if !defined(__LEXER_H__)
#define __LEXER_H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mips8asm_tree.h"
#include "mips8asm_grammar.h"

using namespace std;

#define TK_NONE         999

union TokenInfo {
    MIPS8Node *node;
    int ival;
    char *str;
};

class Lexer  
{
public:
    Lexer(ifstream *in) {
        this->in = in;
        line = 1;
    }

    ~Lexer() { }

    int getLine() {
        return line;
    }

    string getTokenContent() {
        return stringBuffer.str();
    }

    TokenInfo getTokenInfo() { return tokenInfo; }

    int getNextToken();

    static string getTokenString(int token, TokenInfo info);

private:
    int nextChar() {
        return in->get();
    }

    void ungetChar(int ch) {
        in->unget();
    }

private:
    TokenInfo tokenInfo;
	stringstream stringBuffer;
    ifstream *in;
    int line;
};

#endif
