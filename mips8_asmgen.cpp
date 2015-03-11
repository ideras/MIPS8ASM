#include <sstream>
#include "mips8_asmgen.h"

using namespace std;

int opcode[] = {
    0x01, //Add
    0x02, //Sub
    0x03, //Or
    0x04, //And
    0x05, //Xor
    0x05, //Mov
    0x07, //Lw
    0x08, //Sw
    0x09, //Li
    0x0a, //Addi
    0x0b, //Subi
    0x0c, //Cmp
    0x0d, //Jz
    0x0e, //Jnz
    0x10, //Jl
    0x0f, //Jg
    0x11, //Jump
    0x00, //Nop
};


void reportError(string msg);

char xdigit[] = "0123456789ABCDEF";

void numToHex(char str[], int n, int size) {
    unsigned char d0 = n & 0x000F;
    unsigned char d1 = (n & 0x00F0) >> 4;
    unsigned char d2 = (n & 0x0F00) >> 8;
    unsigned char d3 = (n & 0xF000) >> 12;

    if (size == 16) {
        str[0] = xdigit[d3];
        str[1] = xdigit[d2];
        str[2] = xdigit[d1];
        str[3] = xdigit[d0];
        str[4] = '\0';
    } else {
        str[0] = xdigit[d1];
        str[1] = xdigit[d0];
        str[2] = '\0';
    }
}

void MIPS8AsmGen::resolveLabels()
{
    ListNode::iterator it = statements.begin();
    int index = 0;

    while (it != statements.end()) {
        MIPS8Node *node = *it;

        if (node->getKind() == MIPS8_LABEL_DECL) {
            MIPS8LabelDecl *labelDecl = (MIPS8LabelDecl *)node;

            labelDecl->setAddress(index);
            labelMap[labelDecl->getLabel()] = index;
        } else {
            index++;
        }
        it++;
    }
}

int MIPS8AsmGen::genAsm(ostream &out)
{
    ListNode::iterator it = statements.begin();
    int index = 0;
    char str[5];

    resolveLabels();

    out << "module InstructionMemory(" << endl <<
           "    input [7:0] address," << endl <<
           "    output reg [15:0] data" << endl <<
           ");" << endl << endl <<
           "    always @ (address)" << endl <<
           "    begin" << endl <<
           "        case (address)" << endl;

    while (it != statements.end()) {
        MIPS8Node *node = *it;
        int nodeKind = node->getKind();

        it++;
        if (nodeKind == MIPS8_LABEL_DECL)
            continue;

        int instruction = opcode[nodeKind] << 11;

        switch(node->getKind()) {
            case MIPS8_ADD_INST:
            case MIPS8_SUB_INST:
            case MIPS8_OR_INST:
            case MIPS8_AND_INST:
            case MIPS8_XOR_INST:
            case MIPS8_MOV_INST:
            case MIPS8_LW_INST:
            case MIPS8_SW_INST:
            case MIPS8_CMP_INST: {
                MIPS8RFmtInst *inode = (MIPS8RFmtInst *)node;
                int r0 = inode->getReg1();
                int r1 = inode->getReg2();

                instruction |= ((r0 & 0x3) << 9) | ((r1 & 0x3) << 7);
                break;
            }
            case MIPS8_LI_INST:
            case MIPS8_ADDI_INST:
            case MIPS8_SUBI_INST: {
                MIPS8IFmtInst *inode = (MIPS8IFmtInst *)node;
                int r = inode->getReg();
                int imm = inode->getImmediate();

                instruction |= ((r & 0x3) << 9) | ((imm & 0xFF) << 1);
                break;
            }
            case MIPS8_JZ_INST:
            case MIPS8_JNZ_INST:
            case MIPS8_JL_INST:
            case MIPS8_JG_INST:
            case MIPS8_JUMP_INST: {
                MIPS8JFmtInst *inode = (MIPS8JFmtInst *)node;
                string label = inode->getLabel();

                if (labelMap.find(label) != labelMap.end()) {
                    int target = labelMap[label];

                    instruction |= ((target & 0xFF) << 3);
                } else {
                    stringstream ss;

                    ss << "Unresolved label '" << label << "'";

                    reportError(ss.str());

                    return 0;
                }
                break;
            }
            case MIPS8_NOP_INST: break;
        }

        numToHex(str, index, 8);

        out << "            8'h" << str << ": data = 16'h";
        numToHex(str, instruction, 16);
        out << str << ";" << endl;

        index++;
    }

	out << "            default: data = 16'h0000;" << endl;

    out << "        endcase" << endl <<
           "    end" << endl <<
           "endmodule";

    return 1;
}
