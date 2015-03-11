%token_type {TokenInfo}  
  
%include {
#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>
#include <assert.h>
#include "lexer.h"

extern ListNode statements;

using namespace std;

void reportError(string msg);
}  

%default_type {TokenInfo}
%type inst {TokenInfo}
   
%syntax_error {  
    stringstream ss;
    
    ss << "Syntax error. Unexpected " << Lexer::getTokenString(yymajor, TOKEN); 
    reportError(ss.str());
}   

%start_symbol program
   
program ::= inst_list opt_eol TK_EOF.
   
inst_list ::= inst_list TK_EOL inst (A). { statements.push_back(A.node); }
inst_list ::= inst (A). { statements.push_back(A.node); }

opt_eol ::= TK_EOL.
opt_eol ::= .

inst(R) ::= TK_ID(A) TK_SEMICOLON. { R.node = new MIPS8LabelDecl(A.str); free(A.str); }
inst(R) ::= KW_NOP. { R.node = new MIPS8Nop(); }
inst(R) ::= KW_ADD TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Add(A.ival, B.ival); }
inst(R) ::= KW_SUB TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Sub(A.ival, B.ival); }
inst(R) ::= KW_OR  TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Or(A.ival, B.ival); }
inst(R) ::= KW_AND TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8And(A.ival, B.ival); }
inst(R) ::= KW_XOR TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Xor(A.ival, B.ival); }
inst(R) ::= KW_MOV TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Mov(A.ival, B.ival); }
inst(R) ::= KW_LW  TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Lw(A.ival, B.ival); }
inst(R) ::= KW_SW  TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Sw(A.ival, B.ival); }
inst(R) ::= KW_LI  TK_REG(A) TK_COMMA constant(N). { R.node = new MIPS8Li(A.ival, N.ival); }
inst(R) ::= KW_ADDI TK_REG(A) TK_COMMA constant(N). { R.node = new MIPS8Li(A.ival, N.ival); }
inst(R) ::= KW_SUBI TK_REG(A) TK_COMMA constant(N). { R.node = new MIPS8Li(A.ival, N.ival); }
inst(R) ::= KW_CMP TK_REG(A) TK_COMMA TK_REG(B). { R.node = new MIPS8Cmp(A.ival, B.ival); }
inst(R) ::= KW_JZ TK_ID(L). { R.node = new MIPS8Jz(L.str); free(L.str); }
inst(R) ::= KW_JNZ TK_ID(L). { R.node = new MIPS8Jnz(L.str); free(L.str); }
inst(R) ::= KW_JG TK_ID(L). { R.node = new MIPS8Jg(L.str); free(L.str); }
inst(R) ::= KW_JL TK_ID(L). { R.node = new MIPS8Jl(L.str); free(L.str); }
inst(R) ::= KW_JUMP TK_ID(L). { R.node = new MIPS8Jump(L.str); free(L.str); }

constant(R) ::= TK_DEC_CONST(N). { R.ival = N.ival; }
constant(R) ::= TK_HEX_CONST(N). { R.ival = N.ival; }
constant(R) ::= TK_BIN_CONST(N). { R.ival = N.ival; }
