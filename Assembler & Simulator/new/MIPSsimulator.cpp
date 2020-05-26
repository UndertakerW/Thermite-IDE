#include <cstdio>
//#include <malloc.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <map>
#include "MIPSsimulator.h"
#include "assembler.h"
#include "dataHandler.h"

using namespace std;

unsigned int stringToCodes(string line);
void storeMachindeCode(string filename, int * pdata);
void excute(int & pc, unsigned codes);
void Rins(unsigned int codes);
void Iins(int mark, unsigned int codes, int type);
void Jins(int, unsigned int);
int * excute(int * pc);
bool overflow(int x, int y);
typedef void (*RIns) (int *, int *, int *, int);
typedef void (*IIns) (int *, int *, int);
typedef void (*JIns) (int);

static map<int, RIns> RFunctions={
    {0x20, _ADD}, {0x21, _ADDU}, {0x24, _AND},   {0x1a, _DIV},
    {0x1b, _DIVU},{0x18, _MULT}, {0x19, _MULTU}, {0x27, _NOR},
    {0x25, _OR},  {0, _SLL},     {4, _SLLV},     {3, _SRA},
    {7, _SRAV},   {2, _SRL},     {6, _SRLV},     {0x22, _SUB},
    {0x23, _SUBU},{0x26, _XOR},  {0x2a, _SLT},   {0x2b, _SLTU},
    {9, _JALR},   {8, _JR},      {0x34, _TEQ},   {0x36, _TNE},
    {0x30, _TGE}, {0x31, _TGEU}, {0x32, _TLT},   {0x33, _TLTU},
    {0x10, _MFHI},{0x12, _MFLO}, {0x11, _MTHI},  {0x13, _MTLO},
};

static map<int, IIns> IFunctions={
    {0xf, _LUI}, {8, _ADDI},  {9, _ADDIU},{0xc, _ANDI}, {0xd, _ORI},
    {0x23, _LW}, {0xe, _XORI},{0xf, _LUI},{0xa, _SLTI},
    {0xb, _SLTIU},{4, _BEQ}, {7, _BGTZ},  {6, _BLEZ}, {5, _BNE},
    {0x20, _LB}, {0x24, _LBU},{0x21, _LH},{0x25, _LHU},
    {0x23, _LW}, {0x22, _LWL},{0x26, _LWR},{0x30, _LL},
    {0x28, _SB}, {0x29, _SH}, {0x2b, _SW}, {0x2a, _SWL},
    {0x2e, _SWR},{0x38, _SC}
};

static map<int, IIns> IFUNCTIONS_2 = {
    {1, _BGEZ}, {0x11, _BGEZAL}, {0x10, _BLTZAL}, {0, _BLTZ},
    {0xc, _TEQI},{0xe, _TNEI}, {8, _TGEI}, {9, _TGEIU},
    {0xa, _TLTI}, {0xb, _TLTIU}
};

static map<int, JIns> JFunctions={
    {3, _JAL}, {2, _J}
};
static int * pc;
static int * textStart;
static int flag = 0;
static int * pReg;
static int * hi;
static int * lo;
static int llbit=0;
static int * dynamicdata;


int main(int argc, char *argv[]){

    unsigned int memorysize = 6 * 1024 * 1024;
    unsigned int registersize = 32 * 4;

    if (argc != 3){
        cout << "please enter the names of the input and output files" << endl;
        return -1;
    }

    assembleCode(argv[1], argv[2]);

    //allocate the memory
    char * p;
    p = (char*) malloc(memorysize);
    memset(p, 0, memorysize);

    //allocate the register
    pReg = (int *) malloc(registersize);
    memset(pReg, 0, registersize);
    //$zero
    pReg[0] = 0;
    //$sp
    pReg[29] = memorysize + 0x400000 - 1;
    //$fp
    pReg[30] = memorysize + 0x400000 - 1;
    //$gp
    pReg[28] = 1024*1024 + 0x400000;

    //allocate the hi and lo
    int * holder = (int *) malloc(4*2);
    hi = holder;
    lo = holder+1;

    // pointer for data segment
    int * pdataSeg = (int *) (p+1024*1024);

    //store the .data
    dynamicdata = dataHandle(argv[1], pdataSeg);

    string newaddr = argv[2];
    storeMachindeCode(newaddr, (int *) p);
    pc = (int*) p;
    textStart = (int*) p;
    while((*pc)!=0){
        if(flag) break;
        pc = excute(pc);
    }
    cout << "end address: "<< hex << (pc-textStart)*4+0x400000-4 << endl;
}

unsigned int stringToCodes(string line){
    unsigned int holder = 0;
    unsigned int exp = 0;
    int leng = line.length();

    for(int i = leng-1; i>-1; i--){
        if(exp==0) exp=1;
        else exp *= 2;

        holder += (line[i]-'0')*exp;

    }
    return holder;
}

void storeMachindeCode(string filename, int * pdata){
    ifstream infile(filename.c_str());
    unsigned int * codes = (unsigned int *) pdata;
    string buffer;
    while(getline(infile, buffer)){
        *codes = stringToCodes(buffer);
        codes++;
    }
}

int * excute(int * PC){
    unsigned int codes = *PC;
    int  opcode;
    opcode = (0xfc000000)&codes;
    opcode >>= 26;
    opcode &= 0x0000003f;

    if(codes==0xc){
        _SYSCALL();
        return ++pc;
    }

    if(opcode == 0||opcode == 0x1c){
        Rins(codes);
        return ++pc;
    }
    else if(opcode==2||opcode==3){
        Jins(opcode, codes);
        return ++pc;
    }
    else{
        if(opcode==1){
            int rt;
            rt = 0x001f0000&codes;
            rt >>= 16;
            Iins(rt, codes, 1);
        }
        else Iins(opcode, codes, 0);
        return ++pc;
    }
}

bool overflow(int x, int y){
    long long X = (long long) x;
    long long Y = (long long) y;
    long long sum = X + Y;
    if(sum<INT_MIN||sum>INT_MAX){
        return true;
    }
    return false;
}

void Rins(unsigned int codes){
    int opcode = (0xfc000000)&codes;
    opcode >>= 26;
    opcode &= 0x0000003f;

    int Rs = (codes&0x3e00000);
    Rs >>= 21;
    int * rs = (pReg+Rs);

    int Rt = (codes&0x1f0000);
    Rt >>= 16;
    int * rt = (pReg+Rt);

    int Rd = (codes&0xf800);
    Rd >>= 11;
    int * rd = (pReg+Rd);

    unsigned int shamt = (codes&0x7c0);
    shamt >>= 6;
    int funct = (codes&0x3f);

    if(opcode==0x1c) _MUL(rs, rt, rd, shamt);
    else RFunctions[funct](rs, rt, rd, shamt);
}

void Iins(int mark, unsigned int codes, int type){
    int Rs = (codes&0x3e00000);
    Rs >>= 21;
    int * rs = (pReg+Rs);

    int Rt = (codes&0x1f0000);
    Rt >>= 16;
    int * rt = (pReg+Rt);

    int imm = (codes&0xffff);
    //transfer 2's complement
    if(imm&0x8000){
        imm ^= 0xffff;
        imm +=1;
        imm *= -1;
    }
    if(type==0) IFunctions[mark](rs, rt, imm);
    else IFUNCTIONS_2[mark](rs, rt, imm);
}

void Jins(int opcode, unsigned int codes){
    int target = (codes&0x3ffffff);
    target <<= 2;
    int curAddr = (pc-textStart) + 0x400000;
    curAddr &= 0xfc000000;
    target = curAddr|target;
    JFunctions[opcode](target);
}

//R funct = 0x20
void _ADD(int * rs, int * rt, int* rd, int shamt){
    if(overflow(*rs, *rt)){
        cout << "overflow" << endl;
        flag = 1;
    }
    else{
        *rd = *rs + *rt;
    }
}
//R 0x21
void _ADDU(int * rs, int * rt, int* rd, int shamt){
    *rd = *rs + *rt;
}

//I op=8
void _ADDI(int * rs, int * rt, int imm){
    if(overflow(*rs, imm)){
        cout << "overflow" << endl;
        flag = 1;
    }
    else{
//        cout << "addi " << *rt << " " << *rs << " " << imm <<endl;
        *rt = *rs + imm;
    }
}

//I op=9
void _ADDIU(int * rs, int * rt, int imm){
    *rt = *rs + imm;
}

//R 0x24
void _AND(int * rs, int * rt, int* rd, int shamt){
    *rd = *rs & *rt;
}

//I 0xc
void _ANDI(int * rs, int * rt, int imm){
    *rt = *rs & imm;
}

//R 0x1a
void _DIV(int * rs, int * rt, int* rd, int shamt){
    if(*rs==INT_MIN && *rt==-1){
        cout << "overflow" << endl;
        flag = 1;
    }
    else{
        *lo = *rs / *rt;
        *hi = *rs % *rt;
    }
}

//R 0x1b
void _DIVU(int * rs, int * rt, int* rd, int shamt){
    *lo = *rs / *rt;
    *hi = *rs % *rt;
}

// R 0x18
void _MULT(int * rs, int * rt, int* rd, int shamt){
    long long RS = (long long ) *rs;
    long long RT = (long long ) *rt;
    long long result = RS * RT;
    *lo = (int) result;
    result >>= 32;
    *hi = (int) result;
}

//R 0x19
void _MULTU(int * rs, int * rt, int* rd, int shamt){
    unsigned long long RS = (long long) *rs;
    unsigned long long RT = (long long) *rt;
    unsigned long long result = RS * RT;
    *lo = (int) result;
    result >>= 32;
    *hi = (int) result;
}

//R 0x1c
void _MUL(int * rs, int * rt, int* rd, int shamt){
    int RS = (long long ) *rs;
    int RT = (long long ) *rt;
    int result = RS * RT;
    *rd = (int) result;
}


//R 0x27
void _NOR(int * rs, int * rt, int* rd, int shamt){
    *rd = ~(*rs | *rt);
}

// R 0x25
void _OR(int * rs, int * rt, int* rd, int shamt){
    *rd = *rs | *rt;
}

// I 0xd
void _ORI(int * rs, int * rt, int imm){
//    cout << "ORI " << endl;
    *rt = *rs | imm;
}

// R 0
void _SLL(int * rs, int * rt, int* rd, int shamt){
//    cout << "rt: " << hex << *rt << " rd: " << *rd << " shamt " << shamt << dec<< endl;
    *rd = (*rt)<<shamt;
//    cout << "before: " << hex << *rt << " after: " << *rd << " shamt " << shamt << dec<< endl;
}

//R 4
void _SLLV(int * rs, int * rt, int* rd, int shamt){
    *rd = (*rt)<<(*rs);
}

//R 3
void _SRA(int * rs, int * rt, int* rd, int shamt){
    *rd = (*rt) >> shamt;
}

//R 7
void _SRAV(int * rs, int * rt, int* rd, int shamt){
    *rd = (*rt) >> *rs;
}

// R 2
void _SRL(int * rs, int * rt, int* rd, int shamt){
    unsigned RT = (unsigned) *rt;
    *rd = (RT >> shamt);
}

//R 6
void _SRLV(int * rs, int * rt, int* rd, int shamt){
    unsigned RT = (unsigned) *rt;
    *rd = (RT >> (*rs));
}

//R 0x22
void _SUB(int * rs, int * rt, int* rd, int shamt){
    if(overflow(*rs, -(*rd))) flag = 1;
    else *rd = *rs - *rt;
}

//R 0x23
void _SUBU(int * rs, int * rt, int* rd, int shamt){
    *rd = *rs - *rt;
}

//R 0x26
void _XOR(int * rs, int * rt, int* rd, int shamt){
    *rd = *rs ^ *rt;
}

//I 0xe
void _XORI(int * rs, int * rt, int imm){
    *rt = *rs ^ imm;
}

//I 0xf
void _LUI(int * rs, int * rt, int imm){
    int lower = imm & 0xffff;
    lower <<= 16;
    *rt = lower;
}

//R 0x2a
void _SLT(int * rs, int * rt, int* rd, int shamt){
    *rd = (*rs < *rt);
}

//R 0x2b
void _SLTU(int * rs, int * rt, int* rd, int shamt){
    unsigned int RS = (unsigned int ) *rs;
    unsigned int RT = (unsigned int ) *rt;
    *rd = (RS < RT);
}

//I 0xa
void _SLTI(int * rs, int * rt, int imm){
    *rt = (*rs < imm);
}

//I 0xb
void _SLTIU(int * rs, int * rt, int imm){
    unsigned int RS = (unsigned int) *rs;
    unsigned int IMM = (unsigned int) imm;
    *rt = (RS < IMM);
}

//I 4
void _BEQ(int * rs, int * rt, int imm){
    if(*rs==*rt) pc+= imm;
}

//?
//B 1 1
void _BGEZ(int * rs, int * rt, int imm){
    if(*rs>=0) pc+= imm;
}

//? ?register
//B 1 0x11
void _BGEZAL(int * rs, int * rt, int imm){
    if(*rs>=0){
        pc += imm;
        pReg[31] = (pc-textStart)*4+(0x00400000);
    }
}

//?
//B 7 0
void _BGTZ(int * rs, int * rt, int imm){
    if(*rs>0) pc+= imm;
}

//?
//B 6
void _BLEZ(int * rs, int * rt, int imm){
    if(*rs<=0) pc+= imm;
}

//? ?register
//B 1 0x10
void _BLTZAL(int * rs, int * rt, int imm){
    if(*rs<0){
        pc += imm;
        pReg[31] = (pc-textStart)*4+(0x00400000);
    }
}

//? ?
//B 1 0
void _BLTZ(int * rs, int * rt, int imm){
    if(*rs<0) pc += imm;
}

//I 5
void _BNE(int * rs, int * rt, int imm){
    if(*rs != *rt) pc += imm;
}

//J 2
void _J(int target){
    target -= 0x400000;
    pc = textStart+(target/4)-1;
}

//J 3
void _JAL(int target){
//    cout << "JAL" << endl;
    target -= 0x400000;
    pReg[31] = (pc-textStart)*4+0x400000;
    pc =  textStart+(target/4)-1;
}

//????default
//R 9
void _JALR(int * rs, int * rt, int* rd, int shamt){
    *rd = (pc-textStart)*4+0x400000;
    pc =  textStart+((*rs)-0x400000)/4-1;
}

//R 8
void _JR(int * rs, int * rt, int* rd, int shamt){
    pc = textStart+((*rs)-(0x400000))/4;
}

//R 0x34
void _TEQ(int * rs, int * rt, int* rd, int shamt){
    if(*rs == *rt) flag = 1;
}

//I 1
//T 0xc
void _TEQI(int * rs, int * rt, int imm){
    if(*rs == imm) flag=1;
}

//R 0x36
void _TNE(int * rs, int * rt, int* rd, int shamt){
    if(*rs != *rt) flag = 1;
}

//I 1
//T 0xe
void _TNEI(int * rs, int * rt, int imm){
    if(*rs != imm) flag = 1;
}

//R 0x30
void _TGE(int * rs, int * rt, int* rd, int shamt){
    if(*rs >= *rt) flag = 1;
}

//R 0x31
void _TGEU(int * rs, int * rt, int* rd, int shamt){
    unsigned int RS = (unsigned int) (*rs);
    unsigned int RT = (unsigned int) (*rt);
    if(RS >= RT) flag = 1;
}

//I 1
//T 8
void _TGEI(int * rs, int * rt, int imm){
    if(*rs >= imm) flag = 1;
}

//I 1
//T 9
void _TGEIU(int * rs, int * rt, int imm){
    unsigned int RS = (unsigned int) (*rs);
    unsigned int IMM = (unsigned int) imm;
    if(RS >= IMM) flag = 1;
}

//R 0x32
void _TLT(int * rs, int * rt, int* rd, int shamt){
    if(*rs < *rt) flag = 1;
}

//R 0x33
void _TLTU(int * rs, int * rt, int* rd, int shamt){
    unsigned int RS = (unsigned int) (*rs);
    unsigned int RT = (unsigned int) (*rt);
    if(RS < RT) flag = 1;
}

//I 1
//T a
void _TLTI(int * rs, int * rt, int imm){
    if(*rs < imm) flag = 1;
}

//I 1
//T b
void _TLTIU(int * rs, int * rt, int imm){
    unsigned int RS = (unsigned int) (*rs);
    unsigned int IMM = (unsigned int) (imm);
    if(RS < IMM) flag = 1;
}

//I 0x20
void _LB(int * rs, int * rt, int imm){
    int8_t * BYTE;
    int8_t * addr = (int8_t *) ((*rs-0x400000)/4+textStart);
    BYTE = (int8_t *)(addr+imm);
    *rt = *BYTE;
}

//I 0x24
void _LBU(int * rs, int * rt, int imm){
    uint8_t * BYTE;
    uint8_t * addr = (uint8_t *) ((*rs-0x400000)/4+textStart);
    BYTE = (uint8_t *) (addr+imm);
    *rt = *BYTE;
}

//I 0x21
void _LH(int * rs, int * rt, int imm){
    int16_t * halfword;
    int8_t * addr = (int8_t *) ((*rs-0x400000)/4+textStart);
    halfword = (int16_t *) (addr+imm);
    *rt = *halfword;
}

//I 0x25
void _LHU(int * rs, int * rt, int imm){
    uint16_t * halhword;
    uint8_t * addr = (uint8_t *) ((*rs-0x400000)/4+textStart);
    halhword = (uint16_t *) (addr+imm);
}

//I 0x23
void _LW(int * rs, int * rt, int imm){
    int * NUM;
    NUM = ((*rs-0x400000)/4+textStart+imm);
    *rt = *NUM;
}

//I 0x22
void _LWL(int * rs, int * rt, int imm){
    int addr = *rs + imm*4;
    int remian = addr % 4;
    int num = 4 - remian;
    uint8_t *textST = (uint8_t *) textStart;
    uint8_t * newaddr = (uint8_t *) (addr-0x400000+textST);
    uint8_t * RT = (uint8_t *) rt;
    for(int i =0; i < num; i++){
        *(RT+i) = * (newaddr+i);
    }
}

//I 0x26
void _LWR(int * rs, int * rt, int imm){
    int addr = *rs + imm*4;
    int remain = (addr % 4) + 1;
    uint8_t * textST = (uint8_t *) textStart;
    uint8_t * newaddr = (uint8_t *) (addr-0x400000+textST);
    uint8_t * RT = (uint8_t *) rt + 3;
    for(int i =0; i < remain; i++){
        *(RT-i) = * (newaddr-i);
    }
}

//I 0x30
void _LL(int * rs, int * rt, int imm){
    int * NUM;
    NUM = ((*rs-0x400000)/4+textStart+imm);
    *rt = *NUM;
    llbit = 1;
}

//I 0x28
//lower byte
void _SB(int * rs, int * rt, int imm){
   uint8_t lowerbyte = uint8_t (*rt);
   int * addr;
   addr = (*rs-0x400000)/4+textStart+imm;
   *addr = lowerbyte;
}

//I 0x29
void _SH(int * rs, int * rt, int imm){
    uint16_t lowerbyte = uint16_t (*rt);
    int * addr;
    addr = (*rs-0x400000)/4+textStart+imm;
    *addr = lowerbyte;
}

//I 0x2b
void _SW(int * rs, int * rt, int imm){
    int * NUM;
    int off = (*rs-0x400000)/4+imm;
    NUM = (textStart+off);
    *NUM = *rt;
}

//I 0x2a
void _SWL(int * rs, int * rt, int imm){
    int addr = *rs + imm*4;
    int remian = addr % 4;
    int num = 4 - remian;
    uint8_t *textST = (uint8_t *) textStart;
    uint8_t * newaddr = (uint8_t *) (addr-0x400000+textST);
    uint8_t * RT = (uint8_t *) rt;
    for(int i =0; i < num; i++){
        * (newaddr+i) = *(RT+i) ;
    }
}

//I 0x2e
void _SWR(int * rs, int * rt, int imm){
    int addr = *rs + imm*4;
    int remain = (addr % 4) + 1;
    uint8_t * textST = (uint8_t *) textStart;
    uint8_t * newaddr = (uint8_t *) (addr-0x400000+textST);
    uint8_t * RT = (uint8_t *) rt + 3;
    for(int i =0; i < remain; i++){
        * (newaddr-i) = *(RT-i);
    }
}

//I 0x38
void _SC(int * rs, int * rt, int imm){
    if(llbit==1){
        int * NUM;
        NUM = ((*rs-0x400000)/4+textStart+imm);
        *NUM = *rt;
        *rt = 1;
        llbit = 0;
    }
    else{
        *rt = 0;
        llbit = 1;
    }
}

//R 0x10
void _MFHI(int * rs, int * rt, int* rd, int shamt){
    *rd = *hi;
}

//R 0x12
void _MFLO(int * rs, int * rt, int* rd, int shamt){
    *rd = *lo;
}

//R 0x11
void _MTHI(int * rs, int * rt, int* rd, int shamt){
    *hi = *rs;
}

//R 0x13
void _MTLO(int * rs, int * rt, int* rd, int shamt){
    *lo = *rs;
}

void _SYSCALL(){
    //create a map for file operation
    static map<int, fstream*> filemap;
    static map<string, int> filenamemap;
    static int filecount = 0;

    if(pReg[2]==1){
        int * NUM = pReg+4;
        cout << "syscall out: "<< *NUM <<endl;
    }
    else if(pReg[2]==4){
        string holder;
        char temp;
        char * textST = (char *) textStart;
        char * STR = (char * ) ((pReg[4]-0x400000)+textST);
        temp = *STR;
        while(true){
            if(*STR == '\0') break;
            holder += *STR;
            STR++;
        }
        cout << "syscall out: "<< holder  << endl;
    }
    else if(pReg[2]==5){
        int num;
        cout << "syscall: enter a num:";
        cin >> num;
        pReg[2] = num;
    }
    else if(pReg[2]==8){
        string holder;
        string STR;
        char * textST = (char *) textStart;
        int diff;
        int Len;
        char tempt;
        cout << "syscall: enter a string: ";
        cin.clear();
        getline(cin, holder);
        Len = holder.length();
        diff = pReg[5]-1-holder.length();
        STR = holder.substr(0, pReg[5]-1);
        Len = (diff>0) ? holder.length() : pReg[5]-1;
        char * pchar = (char *) ((pReg[4]-0x400000)+textST);
        for(int i=0; i<Len; i++){
            *pchar = STR[i];
            pchar++;
        }
        *pchar = '\0';
    }
    else if(pReg[2]==9){
        int addr = (dynamicdata-textStart)*4+0x400000;
        pReg[2] = addr;
        dynamicdata += pReg[4];
    }
    else if(pReg[2]==10){
        flag = 1;
        cout << "end of the program" << endl;
        exit(0);
    }
    else if(pReg[2]==11){
        char CHAR = (char) (pReg[4]);
        cout << "syscall out: " <<  CHAR << endl;
    }
    else if(pReg[2]==12){
        char holder;
        cout << "syscall: enter a char: ";
        cin >> holder;
        pReg[2] = holder;
    }
    else if(pReg[2]==13){
        string filename;
        fstream testfile;
        char * textST = (char *) textStart;
        char * addr = (char *) ((pReg[4]-0x400000)+textST);
        int filedes;
        filename = addr;

        if(filenamemap.count(filename)){
            filedes = filenamemap[filename];
        }
        else{
            filedes = filecount;
            filenamemap[filename] = filecount;
        }

        //read only
        if(pReg[5]==0) {
            filemap[filedes] = new fstream(filename.c_str(), ios::in);
        }
        //write only
        else if(pReg[5]==1){
            filemap[filedes] = new fstream(filename.c_str(), ios::out);
        }
        //write and read
        else {
            filemap[filedes] = new fstream(filename.c_str(), ios::out|ios::in);;
        }

        if((*filemap[filedes]).fail()){
            cout << "fail to open the file" << endl;
            pReg[4] = -1;
            return;
        }

        pReg[4] = filedes;
        filecount++;
    }
    else if(pReg[2]==14){
        string cache="";
        char holder;
        int count = 0;
        char * textST = (char *) textStart;
        char * pchar = (char *) ((pReg[5]-0x400000)+textST);

        if((*filemap[pReg[4]]).fail()){
            pReg[4] = 0;
            cout << "the file is failed to open, unable to read" << endl;
            return;
        }

        while(((*filemap[pReg[4]]).get(holder))&&(count<pReg[6])){
            *pchar = holder;
            pchar++;
            count++;
        }
        *pchar = '\0';

        pReg[4] = count;
    }
    else if(pReg[2]==15){
        char * textST = (char *) textStart;
        string cache;
        char * addr = (char *) ((pReg[5]-0x400000)+textST);
        cache = addr;
        cache = cache.substr(0, pReg[6]);

        if((*filemap[pReg[4]]).fail()){
            pReg[4] = 0;
            cout << "the file is failed to open, unable to write" << endl;
            return;
        }

        (*filemap[pReg[4]]) << cache;
        pReg[4] = pReg[6];
    }
    else if(pReg[2]==16){
        (*filemap[pReg[4]]).close();
        filemap.erase(pReg[4]);
    }
    else if(pReg[2]==17){
        cout << "syscall out: " << pReg[4] << endl;
        flag = 1;
    }
}
