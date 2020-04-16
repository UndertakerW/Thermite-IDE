#include <iostream>
#include <stack>
#include <array>
#include <map>
#include "registers.h"

using namespace std;

static map<string, string> regMap;
static stack<array<int, 34>> regStatus;

//register names
//name of the register
const char* regName[32] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0",   "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0",   "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8",   "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};


//register number of t0-t9
static int tReg[] = {8, 9, 10, 11, 12, 13, 14, 15, 24, 25};
static int sReg[] = {16, 17, 18, 19, 20, 21, 22, 23};

string getReg(string var){

    if(regStatus.empty()){
        regStatus.push({0});
    }

    //register is using
    if(regMap.count(var)){
        return regMap[var];
    }

    //check $t0-$t9
    for(int i=0; i<10; i++){
        if(regStatus.top()[tReg[i]]==0){
            regStatus.top()[tReg[i]] = 1;
            regMap[var] = regName[tReg[i]];
            return regName[tReg[i]];
        }
    }

    //check $s0-$s7
    for(int i=0; i<8; i++){
        if(regStatus.top()[sReg[i]]==0){
            regStatus.top()[sReg[i]] = 1;
            regMap[var] = regName[sReg[i]];
            return regName[sReg[i]];
        }

    }

    return "full";
}

void upLevel(){

    if(regStatus.empty()){
        regStatus.push({0});
    }
    else{
        //record the last status
        regStatus.push(regStatus.top());
    }
}

void delevel(){
    if(!(regStatus.empty())){
        int oldStatus[34];
        for(int i=0; i<34; i++){
            oldStatus[i] = regStatus.top()[i];
        }

        regStatus.pop();

        if(regStatus.empty()){
            return;
        }

        for(int i =8; i<26; i++){
            //remove the local variables in the map
            cout << "           compare: " << regName[i] << endl;
            if(!(regStatus.top()[i]==oldStatus[i])){
                cout << "deleting->" << regName[i] << endl;
                for (auto it = regMap.begin(); it != regMap.end();){
                    cout << "checking: " << it->first << endl;
                    if ((it->second)==regName[i]){
                        regMap.erase(it++);
                        break;
                    }
                    else{
                        it++;
                    }
                }
            }
        }
    }
}



/*用来测试这个文件的
 *
 *
 *
void prt(){
    cout << "------------check---------------" << endl;
    for(auto it =regMap.begin(); it != regMap.end(); it++){
        cout << (it->first) << "--->" << (it->second) << endl;
    }
    cout << "------------end check-----------" << endl;
}

int main(){
    string holder;

//---------------------------------
    for(int i=0; i<9; i++){
        cout << "enter: ";
        cin >> holder;
        getReg(holder);
    }
    prt();

    //------------------------------
    upLevel();
    for(int i= 0; i<4; i++){
        cout << "enter2: ";
        cin >> holder;
        getReg(holder);
    }
    prt();

        //--------------------------------------
    upLevel();
    for(int i= 0; i<4; i++){
        cout << "enter2: ";
        cin >> holder;
        getReg(holder);
    }
    prt();
    delevel();
        //end----------------------------------
    prt();
    delevel();
    //end------------------------------------

    prt();
    return 0;
}
*/
