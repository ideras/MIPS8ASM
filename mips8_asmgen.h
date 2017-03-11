#ifndef MIPS8ASMGEN_H
#define MIPS8ASMGEN_H

#include <map>
#include "mips8asm_tree.h"

class MIPS8AsmGen
{
public:
    MIPS8AsmGen(ListNode &statements, bool use3BitRegAddress, bool useBinaryFormat) {
        this->useBinaryFormat = useBinaryFormat;
        this->use3BitRegAddress = use3BitRegAddress;
        this->statements = statements;
    }

    int genAsm(ostream &out);
private:
    void resolveLabels();
private:
    bool use3BitRegAddress;
    bool useBinaryFormat;
    ListNode statements;
    map<string, int> labelMap;
};

#endif // MIPS8ASMGEN_H
