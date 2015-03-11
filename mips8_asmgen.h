#ifndef MIPS8ASMGEN_H
#define MIPS8ASMGEN_H

#include <map>
#include "mips8asm_tree.h"

class MIPS8AsmGen
{
public:
    MIPS8AsmGen(ListNode &statements) {
        this->statements = statements;
    }

    int genAsm(ostream &out);
private:
    void resolveLabels();
private:
    ListNode statements;
    map<string, int> labelMap;
};

#endif // MIPS8ASMGEN_H
