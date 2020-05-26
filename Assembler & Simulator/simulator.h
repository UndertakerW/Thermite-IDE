#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstring>
#include <map>
#include <iostream>

void _ADD(int * rs, int * rt, int* rd, int shamt);
void _ADDU(int * rs, int * rt, int* rd, int shamt);
void _ADDI(int * rs, int * rt, int imm);
void _ADDIU(int * rs, int * rt, int imm);
void _AND(int * rs, int * rt, int* rd, int shamt);
void _ANDI(int * rs, int * rt, int imm);
void _DIV(int * rs, int * rt, int* rd, int shamt);
void _DIVU(int * rs, int * rt, int* rd, int shamt);
void _MULT(int * rs, int * rt, int* rd, int shamt);
void _MULTU(int * rs, int * rt, int* rd, int shamt);
void _NOR(int * rs, int * rt, int* rd, int shamt);
void _OR(int * rs, int * rt, int* rd, int shamt);
void _ORI(int * rs, int * rt, int imm);
void _SLL(int * rs, int * rt, int* rd, int shamt);
void _SLLV(int * rs, int * rt, int* rd, int shamt);
void _SRA(int * rs, int * rt, int* rd, int shamt);
void _SRAV(int * rs, int * rt, int* rd, int shamt);
void _SRL(int * rs, int * rt, int* rd, int shamt);
void _SRLV(int * rs, int * rt, int* rd, int shamt);
void _SUB(int * rs, int * rt, int* rd, int shamt);
void _SUBU(int * rs, int * rt, int* rd, int shamt);
void _XOR(int * rs, int * rt, int* rd, int shamt);
void _XORI(int * rs, int * rt, int imm);
void _LUI(int * rs, int * rt, int imm);
void _SLT(int * rs, int * rt, int* rd, int shamt);
void _SLTU(int * rs, int * rt, int* rd, int shamt);
void _SLTI(int * rs, int * rt, int imm);
void _SLTIU(int * rs, int * rt, int imm);
void _BEQ(int * rs, int * rt, int imm);
void _BGEZ(int * rs, int * rt, int imm);
void _BGEZAL(int * rs, int * rt, int imm);
void _BGTZ(int * rs, int * rt, int imm);
void _BLEZ(int * rs, int * rt, int imm);
void _BLTZAL(int * rs, int * rt, int imm);
void _BLTZ(int * rs, int * rt, int imm);
void _BNE(int * rs, int * rt, int imm);
void _J(int target);
void _JAL(int target);
void _JALR(int * rs, int * rt, int* rd, int shamt);
void _JR(int * rs, int * rt, int* rd, int shamt);
void _TEQ(int * rs, int * rt, int* rd, int shamt);
void _TEQI(int * rs, int * rt, int imm);
void _TNE(int * rs, int * rt, int* rd, int shamt);
void _TNEI(int * rs, int * rt, int imm);
void _TGE(int * rs, int * rt, int* rd, int shamt);
void _TGEU(int * rs, int * rt, int* rd, int shamt);
void _TGEI(int * rs, int * rt, int imm);
void _TGEIU(int * rs, int * rt, int imm);
void _TLT(int * rs, int * rt, int* rd, int shamt);
void _TLTU(int * rs, int * rt, int* rd, int shamt);
void _TLTI(int * rs, int * rt, int imm);
void _TLTIU(int * rs, int * rt, int imm);
void _LB(int * rs, int * rt, int imm);
void _LBU(int * rs, int * rt, int imm);
void _LH(int * rs, int * rt, int imm);
void _LHU(int * rs, int * rt, int imm);
void _LW(int * rs, int * rt, int imm);
void _LWL(int * rs, int * rt, int imm);
void _LWR(int * rs, int * rt, int imm);
void _LL(int * rs, int * rt, int imm);
void _SB(int * rs, int * rt, int imm);
void _SH(int * rs, int * rt, int imm);
void _SW(int * rs, int * rt, int imm);
void _SWL(int * rs, int * rt, int imm);
void _SWR(int * rs, int * rt, int imm);
void _SC(int * rs, int * rt, int imm);
void _MFHI(int * rs, int * rt, int* rd, int shamt);
void _MFLO(int * rs, int * rt, int* rd, int shamt);
void _MTHI(int * rs, int * rt, int* rd, int shamt);
void _MTLO(int * rs, int * rt, int* rd, int shamt);
void _MUL(int * rs, int * rt, int* rd, int shamt);
void _SYSCALL();
bool overflow(int x, int y);

#endif // SIMULATOR_H
