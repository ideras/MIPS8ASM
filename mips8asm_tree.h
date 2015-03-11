#ifndef MIPS8ASM_TREE_H
#define MIPS8ASM_TREE_H

#include <list>
#include <string>

#define MIPS8_ADD_INST  0
#define MIPS8_SUB_INST  1
#define MIPS8_OR_INST   2
#define MIPS8_AND_INST  3
#define MIPS8_XOR_INST  4
#define MIPS8_MOV_INST  5
#define MIPS8_LW_INST   6
#define MIPS8_SW_INST   7
#define MIPS8_LI_INST   8
#define MIPS8_ADDI_INST 9
#define MIPS8_SUBI_INST 10
#define MIPS8_CMP_INST  11
#define MIPS8_JZ_INST   12
#define MIPS8_JNZ_INST  13
#define MIPS8_JL_INST   14
#define MIPS8_JG_INST   15
#define MIPS8_JUMP_INST 16
#define MIPS8_NOP_INST  17
#define MIPS8_LABEL_DECL 99

using namespace  std;

class MIPS8Node {
public:
    virtual int getKind() = 0;
};

class MIPS8Nop: public MIPS8Node {
public:
    MIPS8Nop() { }

    int getKind() { return MIPS8_NOP_INST; }
};

class MIPS8RFmtInst: public MIPS8Node {
public:
    int getReg1() { return reg1; }
    int getReg2() { return reg2; }
protected:
    int reg1;
    int reg2;
};

class MIPS8IFmtInst: public MIPS8Node {
public:
    int getReg() { return reg; }
    int getImmediate() { return imm; }
protected:
    int reg;
    int imm;
};

class MIPS8JFmtInst: public MIPS8Node {
public:
    string getLabel() { return label; }
protected:
    string label;
};

class MIPS8Add: public MIPS8RFmtInst {
public:
    MIPS8Add(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_ADD_INST; }
};

class MIPS8Sub: public MIPS8RFmtInst {
public:
    MIPS8Sub(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_SUB_INST; }
};

class MIPS8Or: public MIPS8RFmtInst {
public:
    MIPS8Or(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_OR_INST; }
};

class MIPS8And: public MIPS8RFmtInst {
public:
    MIPS8And(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_AND_INST; }
};

class MIPS8Xor: public MIPS8RFmtInst {
public:
    MIPS8Xor(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_XOR_INST; }
};

class MIPS8Mov: public MIPS8RFmtInst {
public:
    MIPS8Mov(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_MOV_INST; }
};

class MIPS8Lw: public MIPS8RFmtInst {
public:
    MIPS8Lw(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_LW_INST; }
};

class MIPS8Sw: public MIPS8RFmtInst {
public:
    MIPS8Sw(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_SW_INST; }
};

class MIPS8Cmp: public MIPS8RFmtInst {
public:
    MIPS8Cmp(int reg1, int reg2) {
        this->reg1 = reg1;
        this->reg2 = reg2;
    }

    int getKind() { return MIPS8_CMP_INST; }
};

class MIPS8Li: public MIPS8IFmtInst {
public:
    MIPS8Li(int reg, int imm) {
        this->reg = reg;
        this->imm = imm;
    }

    int getKind() { return MIPS8_LI_INST; }
};

class MIPS8Addi: public MIPS8IFmtInst {
public:
    MIPS8Addi(int reg, int imm) {
        this->reg = reg;
        this->imm = imm;
    }

    int getKind() { return MIPS8_ADDI_INST; }
};

class MIPS8Subi: public MIPS8IFmtInst {
public:
    MIPS8Subi(int reg, int imm) {
        this->reg = reg;
        this->imm = imm;
    }

    int getKind() { return MIPS8_SUBI_INST; }
};

class MIPS8Jz: public MIPS8JFmtInst {
public:
    MIPS8Jz(string label) {
        this->label = label;
    }

    int getKind() { return MIPS8_JZ_INST; }
};

class MIPS8Jnz: public MIPS8JFmtInst {
public:
    MIPS8Jnz(string label) {
        this->label = label;
    }

    int getKind() { return MIPS8_JNZ_INST; }
};

class MIPS8Jl: public MIPS8JFmtInst {
public:
    MIPS8Jl(string label) {
        this->label = label;
    }

    int getKind() { return MIPS8_JL_INST; }
};

class MIPS8Jg: public MIPS8JFmtInst {
public:
    MIPS8Jg(string label) {
        this->label = label;
    }

    int getKind() { return MIPS8_JG_INST; }
};

class MIPS8Jump: public MIPS8JFmtInst {
public:
    MIPS8Jump(string label) {
        this->label = label;
    }

    int getKind() { return MIPS8_JUMP_INST; }
};

class MIPS8LabelDecl: public MIPS8Node {
public:
    MIPS8LabelDecl(string label) {
        this->label = label;
    }

    string getLabel() { return label; }
    int getAddress() { return address; }
    void setAddress(int address) { this->address = address; }

    int getKind() { return MIPS8_LABEL_DECL; }
protected:
    string label;
    int address;
};

typedef list<MIPS8Node *> ListNode;

#endif // MIPS8ASM_TREE_H
