// Lexer.cpp: implementation of the Lexer class.
//
#include <cstdlib>
#include "lexer.h"
#include <cstring>

#define APPEND_SYMBOLS(f, sb) \
            do { \
                while (f(ch)) { \
                    sb << (char)ch;\
                    ch = nextChar();\
                } \
                ungetChar(ch); \
            } while (0)

struct  Keyword
{
    const char *name;
    int tokenKind;
};

Keyword kw[] = {
          {"nop", KW_NOP},
          {"add", KW_ADD},
          {"sub", KW_SUB},
          {"or", KW_OR},
          {"and", KW_AND},
          {"xor", KW_XOR},
          {"mov", KW_MOV},
          {"lw", KW_LW},
          {"sw", KW_SW},
          {"li", KW_LI},
          {"addi", KW_ADDI},
          {"subi", KW_SUBI},
          {"cmp", KW_CMP},
          {"jz", KW_JZ},
          {"jnz", KW_JNZ},
          {"jg", KW_JG},
          {"jl", KW_JL},
          {"jmp", KW_JUMP},
          {"r0", TK_REG},
          {"r1", TK_REG},
          {"r2", TK_REG},
          {"r3", TK_REG},
          };

inline int isbdigit(int ch) {
    return ((ch == '0') || (ch == '1'));
}

int Lexer::getNextToken()
{
    int ch;

    stringBuffer.str("");
    
    while(1) {
        ch = nextChar();

        if (ch == ' ' || ch == '\t')
            continue;
            
        if (ch == '#') {
            while (1) {
                ch = nextChar();
                
                if (ch==EOF || ch=='\n')
                    break;
            }
        }
        
        stringBuffer << (char)ch;

        if (ch == '\n'){
            ch = nextChar();
            if(ch == EOF)
                return TK_EOF;
            ungetChar(ch);
            line++;
            return TK_EOL;
        }
        else if (ch == '\r') {
            ch = nextChar();
            if (ch == '\n')
            {
                ch = nextChar();
                if(ch == EOF)
                    return TK_EOF;
                line++;
                return TK_EOL;
            }
            ungetChar(ch);
        }
        else if (isalpha(ch) || ch=='_') {
            
            stringBuffer.str("");
            while ( isalnum(ch) || ch=='_') {
                stringBuffer << (char)ch;

                ch = nextChar();
            }

            ungetChar(ch);

            string s = stringBuffer.str();
            int tokenKind = 0;

            for (int i=0; i<sizeof(kw)/sizeof(kw[0]); i++) {
                if (strcmp(s.c_str(), kw[i].name) == 0) {
                    tokenKind = kw[i].tokenKind;
                    break;
                }
            }

            if (tokenKind == 0) {
                tokenInfo.str = strdup(s.c_str());

                return TK_ID;
            } else if (tokenKind == TK_REG) {
                tokenInfo.ival = s[1] - '0';
                return tokenKind;
            } else {
                tokenInfo.str = strdup(s.c_str());
                return tokenKind;
            }

        } else if (ch == '0') {
            string s;
            ch = nextChar();

            if (ch == 'x') {
                stringBuffer.str("");
                ch = nextChar();
                APPEND_SYMBOLS(isxdigit, stringBuffer);

                s = stringBuffer.str();
                tokenInfo.ival = strtol(s.c_str(), NULL, 16);

                return TK_HEX_CONST;
            } else if (ch == 'b') {
                stringBuffer.str("");
                ch = nextChar();
                APPEND_SYMBOLS(isbdigit, stringBuffer);

                s = stringBuffer.str();
                tokenInfo.ival = strtol(s.c_str(), NULL, 2);

                return TK_BIN_CONST;
            } else if (isdigit(ch)) {
                stringBuffer.str("");
                stringBuffer << '0';
                APPEND_SYMBOLS(isdigit, stringBuffer);

                s = stringBuffer.str();
                tokenInfo.ival = strtol(s.c_str(), NULL, 10);

                return TK_DEC_CONST;
            } else {
                ungetChar(ch);
                stringBuffer.str("0");

                tokenInfo.ival = 0;

                return TK_DEC_CONST;
            }
        } else if (isdigit(ch)) {
                stringBuffer.str("");
                APPEND_SYMBOLS(isdigit, stringBuffer);

                string s = stringBuffer.str();
                tokenInfo.ival = strtol(s.c_str(), NULL, 10);

                return TK_DEC_CONST;
        } else if (ch == ':')
            return TK_SEMICOLON;
        else if (ch == ',')
            return TK_COMMA;
        else if (ch == EOF)
            return TK_EOF;
        else {
            printf("Invalid character %d\n", ch);
            return TK_NONE;
        }
    }
}

string byteToBinary(unsigned char n) {
    stringstream ss;

    for(unsigned char mask = 0x80; mask != 0; mask >>= 1) {
        if ((n & mask) != 0)
            ss << "1";
        else
            ss << "0";
    }

    return ss.str();
}

string Lexer::getTokenString(int token, TokenInfo info)
{
    stringstream ss;

    switch (token) {
        case TK_EOF: ss << "end of file"; break;
        case TK_EOL: ss << "end of line"; break;
        case TK_ID:  ss << "identifier '" << info.str << "'"; break;
        case TK_SEMICOLON: ss << "semicolon"; break;
        case KW_NOP: ss << "keyword 'nop'"; break;
        case KW_ADD: ss << "keyword 'add'"; break;
        case TK_REG:  ss << "register 'r" << info.ival << "'"; break;
        case TK_COMMA: ss << "comma"; break;
        case KW_SUB: ss << "keyword 'sub'"; break;
        case KW_OR: ss << "keyword 'or'"; break;
        case KW_AND: ss << "keyword 'and'"; break;
        case KW_XOR: ss << "keyword 'xoir'"; break;
        case KW_MOV: ss << "keyword 'mov'"; break;
        case KW_LW: ss << "keyword 'lw'"; break;
        case KW_SW: ss << "keyword 'sw'"; break;
        case KW_LI: ss << "keyword 'lw'"; break;
        case KW_ADDI: ss << "keyword 'addi'"; break;
        case KW_SUBI: ss << "keyword 'subi'"; break;
        case KW_CMP: ss << "keyword 'cmp'"; break;
        case KW_JZ: ss << "keyword 'jz'"; break;
        case KW_JNZ: ss << "keyword 'jnz'"; break;
        case KW_JG: ss << "keyword 'jg'"; break;
        case KW_JL: ss << "keyword 'jl'"; break;
        case KW_JUMP: ss << "keyword 'jmp'"; break;
        case TK_DEC_CONST: ss << "decimal constant '" << info.ival << "'"; break;
        case TK_HEX_CONST: ss << "hexadecimal constant '0x" << hex << info.ival << "'"; break;
        case TK_BIN_CONST: ss << "binary constant '0b" << byteToBinary(info.ival) << "'"; break;
        default:
                 ss << "Unknown token '" << token << "'"; break;
    }

    return ss.str();
}


