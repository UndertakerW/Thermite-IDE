#include<iostream>
#include<set>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stack>
#include <string.h>
#include <stdio.h>
#include"compiler.h"
#include"functions.h"
//#include "registers.h"

using namespace std;

const set<string> instruction_key_word={"if","else","while","for","cin","cout","int","char","string","{","}", "short","return","continue","break"};

const set<char> set_key={'=','<','>','!'};

const set<char> equation_key={'+','-','*','/','%','|','&'};

const set<char> number_set={'1','2','3','4','5','6','7','8','9','0'};

const set<string> self_eq_key={"++","--","+=","-=","*=","/="};

const set<string> equation__key={"+","-","*","/","%","|","&"};

const map<string,int> mark_key={make_pair("=",0),make_pair("<",1),make_pair(">",2),
                               make_pair("<=",3),make_pair(">=",4),make_pair("==",5),make_pair("!=",6)};

map<string,string> variabels_map;

vector<string> word_of_sentence;

vector<string> things_that_would_wrtie;

vector<string> data_section;

list<string> string_handle_para;

list<string> label_break;

vector<int> break_index_write;
vector<int>break_layer_num;

list<string> for_iter_list;

//new added
//save the address of the string, char, short variables
map<string, int> dataSecAdress;
static int dataSecCount=0;

static stack<array<int, 34>> regStatus;
static stack<array<int, 34>> regChange;

//used to store the information of registers
//when the new register covering the old one
//<<variable name, register>, reglevel>
map<array<string,2>, int> globalReg;
int reglevel=-1;


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

static int tsReg[] = {8, 9, 10, 11, 12, 13, 14, 15, 24, 25,
                      16, 17, 18, 19, 20, 21, 22, 23, 27, 29, 7};

static int tsRegNum = 21;

//register for function parameters
const char* pReg[] = {"$k1", "$sp", "$a3"};

static std::map<std::string, std::string> regMap;

//static int rapointer=0x600000;

int write(string MIPS,int &get_index,bool haslabel);

int scan_key(string firw, string line);

//end new added

//1:all bracket has label after }
//2:loop bracket has jump to label when detect }
list<int> bracket_type;

list<int> labels_manage;

list<string> label_name_manage;

list<string> loop_name_manage;

map<string,string> function_name_manage;

list<int> function_manage;

list<string> function_label_back;

map<string,vector<vector<string>>> function_par_and_type;

list<int> loop_manage;

list<string> function_work_on;

list<string> function_stack_ret;
list<string> function_stack_cal;
list<int> return_list;
list<string> for_iter_store;
int adressAllocater(int leng);
int arithmetic(string line);
//string getReg(string var, bool cover=false, bool newcover=false){
//    return "$"+var;
//}

void storeRegs(){
    int a;
    for(int i=0; i<tsRegNum; i++){
        string tempt = regName[tsReg[i]];
        write("sw "+tempt+", 0($at)", a, false);
        write("addi $at, $at, 4",a,false);
    }
}

void recoverRegs(){
    int a;
    for(int i=tsRegNum-1; i>=0; i--){
        string tempt = regName[tsReg[i]];
        write("addi $at, $at, -4",a,false);
        write("lw "+tempt+", 0($at)", a, false);
    }
}

void saveRa(){
    int a;
//    rapointer += 4;
//    write("lui $gp, 96", a, false);
    write("addi $gp, $gp, 4",a,false);
    write("sw $ra, 0($gp)",a,false);
}

void getRa(){
    int a;
//    write("lui $gp, 96",a,false);
    write("lw $ra, 0($gp)",a,false);
    write("addi $gp, $gp, -4",a,false);
//    rapointer -=4;
}

string findReg(string var){
    return regMap[var];
}

int RegOrder(string var){
    for(int i=0; i<32; i++){
        if(var==regName[i]){
            return i;
        }
    }
    return  -1;
}

int vecSearch(vector<string> & vec, string var){
    int leng = vec.size();
    for(int i=0; i<leng; i++){
        if(vec[i]==var){
            return i;
        }
    }
    return -1;

}
//string getReg(string var,bool cover=false, bool newcover=false){    return "$"+var;}

string getReg(string var, bool cover=false, bool newcover = false){
    stringstream ss(var);
    ss >> var;
    int isPar=-1;
    string name;

    if(!string_handle_para.empty()){
        name = string_handle_para.front();
        isPar = vecSearch(function_par_and_type[name][2], var);
    }

    if((!cover&&!newcover)||(!cover&&newcover)){
        if(isPar==-1){
            if(regMap.count(var))
                return regMap[var];

            if(regStatus.empty()){
                reglevel++;
                regStatus.push({0});
                regChange.push({0});
            }

            if(var=="temp_func"){
                regMap[var] = "$k0";
                return "$k0";
            }


            //if register is using before, allocate a new register
            //for the local variable
            if(regMap.count(var)){
                //if the register is used in this level
                if(regChange.top()[RegOrder(regMap[var])]){
                    return regMap[var];
                }
                globalReg[{var, regMap[var]}] = reglevel-1;
                regMap.erase(var);
            }

            //check $t0-$t9
            for(int i=0; i<10; i++){
                if(regStatus.top()[tReg[i]]==0){
                    regStatus.top()[tReg[i]] = 1;
                    regChange.top()[tReg[i]] = 1;
                    regMap[var] = regName[tReg[i]];
                    return regName[tReg[i]];
                }
            }

            //check $s0-$s7
            for(int i=0; i<8; i++){
                if(regStatus.top()[sReg[i]]==0){
                    regStatus.top()[sReg[i]] = 1;
                    regChange.top()[sReg[i]] = 1;
                    regMap[var] = regName[sReg[i]];
                    return regName[sReg[i]];
                }

            }

            cout << "register is full" << endl;
            exit(0);
            return "full";
        }
        return function_par_and_type[name][1][isPar];
    }

    if(cover&&!newcover){
        if(isPar!=-1){
            return regMap[var];
        }

        if(regStatus.empty()){
            reglevel++;
            regStatus.push({0});
            regChange.push({0});
        }

        if(var=="temp_func"){
            regMap[var] = "$k0";
            return "$k0";
        }


        //if register is using before, allocate a new register
        //for the local variable
        if(regMap.count(var)){
            //if the register is used in this level
            if(regChange.top()[RegOrder(regMap[var])]){
                return regMap[var];
            }
            globalReg[{var, regMap[var]}] = reglevel-1;
            regMap.erase(var);
        }

        //check $t0-$t9
        for(int i=0; i<10; i++){
            if(regStatus.top()[tReg[i]]==0){
                regStatus.top()[tReg[i]] = 1;
                regChange.top()[tReg[i]] = 1;
                regMap[var] = regName[tReg[i]];
                return regName[tReg[i]];
            }
        }

        //check $s0-$s7
        for(int i=0; i<8; i++){
            if(regStatus.top()[sReg[i]]==0){
                regStatus.top()[sReg[i]] = 1;
                regChange.top()[sReg[i]] = 1;
                regMap[var] = regName[sReg[i]];
                return regName[sReg[i]];
            }

        }

        cout << "register is full" << endl;
        exit(0);
        return "full";
    }

    else{
        if(isPar==-1){
            return regMap[var];
        }

        if(regMap.count(var)){
            return regMap[var];
        }

        if(regStatus.empty()){
            reglevel++;
            regStatus.push({0});
            regChange.push({0});
        }

        if(var=="temp_func"){
            regMap[var] = "$k0";
            return "$k0";
        }


        //if register is using before, allocate a new register
        //for the local variable
        if(regMap.count(var)){
            //if the register is used in this level
            if(regChange.top()[RegOrder(regMap[var])]){
                return regMap[var];
            }
            globalReg[{var, regMap[var]}] = reglevel-1;
            regMap.erase(var);
        }

        //check $t0-$t9
        for(int i=0; i<10; i++){
            if(regStatus.top()[tReg[i]]==0){
                regStatus.top()[tReg[i]] = 1;
                regChange.top()[tReg[i]] = 1;
                regMap[var] = regName[tReg[i]];
                return regName[tReg[i]];
            }
        }

        //check $s0-$s7
        for(int i=0; i<8; i++){
            if(regStatus.top()[sReg[i]]==0){
                regStatus.top()[sReg[i]] = 1;
                regChange.top()[sReg[i]] = 1;
                regMap[var] = regName[sReg[i]];
                return regName[sReg[i]];
            }

        }

        cout << "register is full" << endl;
        exit(0);
        return "full";
    }
}

// *new requirement:
//  there is a list called string_handle_para, if list is empty, work as normal
// if no-empty, when it meet the parameter, you should first check if it is a parameter--> function_par_and_type[name][2].count(vari)==0
// if it is goto map function_par_and_type[name][2].find(vari) to get index, and use index to get the register
// by get function_par_and_type[name][1][index]. this is straightly register you will return

// only used when a function is created and it will used to get register for parameter
// give parameter in, and give register out. that's all
string getparareg(string var){
    static int regCounter=0;
    regMap[var] = pReg[(regCounter++)%3];
    return pReg[(regCounter++)%3];
}

string free_reg(string var){
    if(regMap.count(var)){
        string result = regMap[var];
        //search for the position of the registers
        int posi=-1;
        for(int i =0; i<34; i++){
            if(regMap[var]==regName[i]){
                posi = i;
                break;
            }
        }
        regMap.erase(var);
        regStatus.top()[posi]=0;
        regChange.top()[posi]=0;
        return  result;
    }
    return "none";
}


//this function will generate a uniqueue label once needed.
//if the label appear exactly behind '}' as usually, then
//once generated, you need to push it into the list: label_name_manage
//if the label appear right behind the instruction while/for, and before the '{'
//then put label into loop_name_manage, the write function will handle it.
string label_generate(){
    static int labelNO=0;
    stringstream aaa;
    aaa << labelNO;
    string a;
    aaa >> a;
    string label="label"+a;
    labelNO++;
    return label;
}

int judge_else(){}

//register part
void uplevel(){
    reglevel++;
    regChange.push({0});
    if(regStatus.empty()){
        regStatus.push({0});
    }
    else{
        //record the last status
        regStatus.push(regStatus.top());
    }
}
//register part
void delevel(){
    if(!(regStatus.empty())){
        int oldStatus[34];
        regChange.pop();
        reglevel--;

        for(int i=0; i<34; i++){
            oldStatus[i] = regStatus.top()[i];
        }

        regStatus.pop();

        if(regStatus.empty()){
            regMap.clear();
            return;
        }
        for(int i =8; i<26; i++){
            //remove the local variables in the map
            if(!(regStatus.top()[i]==oldStatus[i])){
                for (auto it = regMap.begin(); it != regMap.end();){
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

        //recover the covered varialbes
        for(auto it=globalReg.begin(); it!= globalReg.end(); it++){
            if((it->second)==reglevel){
                regMap[(it->first)[0]] = it->first[1];
            }
        }
    }
}

//write the code of MIPS, and also add label if the requirement is ok
//once this line is beq or bne instruction which should be put a label
//then just assign the paramenter last to be true.
//then it can handle to put the label at right place
int write(string MIPS,int &get_index,bool haslabel){
    static int counter=0;
    //static int label_need=0;
    things_that_would_wrtie.push_back(MIPS);
    if(haslabel){
        labels_manage.push_front(counter);
    }

    get_index=counter;
    counter++;
    return 0;
}

//deal with life time
int write_bracket_front(){
    uplevel();
    return 0;
}

//generate a label when there should be one after }, put it in list of labels
// for furthering adding label in MIPS
int write_bracket(){
    //if type 1: generate a label
    if(bracket_type.front()==1){
        string label=label_generate();
        int a;
        label_name_manage.push_front(label);
        write(label+": ",a,false);
        bracket_type.pop_front();
        int index=labels_manage.front();
        things_that_would_wrtie[index]=things_that_would_wrtie[index]+label_name_manage.front();
        labels_manage.pop_front();
        label_name_manage.pop_front();
    }
    else if(bracket_type.front()==2){
        int a;
        if(!for_iter_list.empty()){
            arithmetic(for_iter_list.front());
            for_iter_list.pop_front();
        }
        write("j "+ loop_name_manage.front(),a, false);
        loop_name_manage.pop_front();
        string label= label_generate();
        write(label+": ",a,false);
        label_name_manage.push_front(label);
        while(!break_index_write.empty()){
            things_that_would_wrtie[break_index_write.back()]+=label;
            int b=break_layer_num.back();
            break_layer_num.pop_back();
            break_index_write.pop_back();
            if(b!=break_layer_num.back()){
                for(int i=0;i<break_layer_num.size();i++){
                    break_layer_num[i]-=1;
                }
                break;
            }
        }
        bracket_type.pop_front();
        int index= labels_manage.front();
        things_that_would_wrtie[index]= things_that_would_wrtie[index]+label_name_manage.front();
        labels_manage.pop_front();
        label_name_manage.pop_front();
    }
    else if(bracket_type.front()==3){
        int a;
        bracket_type.pop_front();
        string_handle_para.pop_front();
        string label=label_generate();
        write(label+":",a,false);
        while(!return_list.empty()){
            things_that_would_wrtie[return_list.front()]+=label;
            return_list.pop_front();
        }
        recoverRegs();
        getRa();
        write("jr $ra",a,false);
    }
    else if(bracket_type.front()==4){
        int a;
        string label=label_generate();
        write(label+":",a,false);
        while(!return_list.empty()){
            things_that_would_wrtie[return_list.front()]+=label;
            return_list.pop_front();
        }
        write("addi $v0, $zero, 10",a,false);
        write("syscall",a,false);
    }
    else if(bracket_type.front()==5){
        int a;
        write("j "+ loop_name_manage.front(),a, false);
        loop_name_manage.pop_front();
        string label= label_generate();
        write(label+": ",a,false);
        label_name_manage.push_front(label);
        while(!break_index_write.empty()){
            things_that_would_wrtie[break_index_write.back()]+=label;
            int b=break_layer_num.back();
            break_layer_num.pop_back();
            break_index_write.pop_back();
            if(b!=break_layer_num.back()){
                for(int i=0;i<break_layer_num.size();i++){
                    break_layer_num[i]-=1;
                }
                break;
            }
        }
        bracket_type.pop_front();
        int index= labels_manage.front();
        things_that_would_wrtie[index]= things_that_would_wrtie[index]+label_name_manage.front();
        labels_manage.pop_front();
        label_name_manage.pop_front();
    }
    //if type 2: beq to loop label, pop loop list, generate a label
    delevel();
    return 0;
}

//the above deal with the label and bracket structure.
//the below deal with the math operation structure.
//tips: use tree to store order of arithmetic.


bool detect_func(string name){
    return function_name_manage.count(name)!=0;
}

int searchline(string &line){
    int a;
    for(int i=0;i<line.length();i++){
        for(int j=0;j<line.length()-i-1;j++){
            if(detect_func(line.substr(j,i))&&line[j+i]=='('){
                int q;
                for(int k=j;k<line.length();k++){
                    if(line[k]==')'){
                        q=k;
                        break;
                    }
                }
                string line1=line.substr(j,q-j+1);
                scan_key(line.substr(j,i),line1);
                if(q<line.length()-1)
                    line=line.substr(0,j)+function_stack_cal.front()+line.substr(q+1);
                else line=line.substr(0,j)+function_stack_cal.front();
                string new_reg=getReg(function_stack_cal.front());
                write("addi "+new_reg+", "+getReg("temp_func")+", 0",a,false);
                function_stack_cal.pop_front();
                //string l=line.substr(q+1);
                searchline(line);
                break;
            }
        }
    }
    return 0;
}

//split the equation into variables and values and signs, and store them into the list and vector
int equation_split_key(string &line, list<string> &words,vector<int> &layers){
    words.clear();
    string this_word="";
    int layer=0;
    int size=0;
    searchline(line);
    for(int i=0;i<line.length();i++){
        if(line.at(i)==';') break;
        if(line.at(i)=='('||line.at(i)==')'){
             if(line.at(i)=='(') layer+=1;
             else layer-=1;

         }
        else if(equation_key.count(line[i])==0 && line.at(i)!=')' && line.at(i)!='('){
            this_word+=line.at(i);
            if(i<line.length()-1&&(equation_key.count(line.at(i+1))!=0||line.at(i+1)==')')){
                words.push_back(this_word);
                size+=1;
                layers.push_back(layer);
                this_word="";
            }
            else if(i>=line.length()-1){
                words.push_back(this_word);
                size+=1;
                layers.push_back(layer);
                this_word="";
            }

        }

        else{
            this_word="";
            this_word+=line.at(i);
            words.push_back(this_word);
            layers.push_back(layer);
            size+=1;
            this_word="";
        }
    }
    return size;
}

//test it is variable or value
bool test_vari(string vari){
    for(int i=0;i<vari.length();i++){
        if(number_set.count(vari.at(i))==0)
            return true;
    }
    return false;
}


//write instru and return the change of layers
// change of layer is for multiple calculation
double do_ARthimetic(string a_1,string op, string a_2,string temp__result){
    int a;
    string a1=getReg(a_1);
    string a2=getReg(a_2);
    string temp_result=getReg(temp__result);
    //two variable
    if(test_vari(a_1)&&test_vari(a_2)){
    if(op=="+"){
        write("add "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="-"){
        write("sub "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="&"){
        write("and "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="|"){
        write("or "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        write("mul "+temp_result+", "+a1+", "+a2,a,false);
        return -0.3;
    }
    if(op=="/"){
        write("div "+a1+", "+a2,a,false);
        write("mflo "+temp_result,a,false);
        return -0.3;
    }
    if(op=="%"){
        write("div "+a1+", "+a2,a,false);
        write("mfhi "+temp_result,a,false);
        return -0.3;
    }
    }
    //first variable, second is number
    else if(test_vari(a_1)&&!test_vari(a_2)){
        a2=a_2;
    if(op=="+"){
        write("addi "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="-"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("sub "+temp_result+", "+a1+", "+tp,a,false);
        free_reg(tp);
        return 0;
    }
    if(op=="&"){
        write("andi "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="|"){
        write("ori "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    //need to handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("mul "+temp_result+", "+a1+", "+tp,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("div "+a1+", "+tp,a,false);
        write("mflo "+temp_result,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="%"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("div "+a1+", "+tp,a,false);
        write("mfhi "+temp_result,a,false);
        free_reg(tp);
        return -0.3;
    }
    }
    //first is number, second is variable
    else if(!test_vari(a_1)&&test_vari(a_2)){
        a1=a_1;
    if(op=="+"){
        write("addi "+temp_result+", "+a2+", "+a1,a,false);
        return 0;
    }
    if(op=="-"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("sub "+temp_result+", "+tp+", "+a2,a,false);
        free_reg(tp);
        return 0;
    }
    if(op=="&"){
        write("andi "+temp_result+", "+a2+", "+a1,a,false);
        return 0;
    }
    if(op=="|"){
        write("ori "+temp_result+", "+a2+", "+a1,a,false);
        return 0;
    }
    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("mul "+temp_result+", "+a2+", "+tp,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("div "+tp+", "+a2,a,false);
        write("mflo "+temp_result,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="%"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("div "+tp+", "+a2,a,false);
        write("mfhi "+temp_result,a,false);
        free_reg(tp);
        return -0.3;
    }
    }
    //two number
    else if(!test_vari(a_1)&&!test_vari(a_2)){
        a1=a_1;
        a2=a_2;
    if(op=="+"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+temp_result+", "+tp+", "+a2,a,false);
        return 0;
    }
    if(op=="-"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("sub "+temp_result+", "+tp+", "+tp2,a,false);
        free_reg(tp);
        free_reg(tp2);
        return 0;
    }
    if(op=="&"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("andi "+temp_result+", "+tp+", "+a2,a,false);
        return 0;
    }
    if(op=="|"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("ori "+temp_result+", "+tp+", "+a2,a,false);
        return 0;
    }
    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("mul "+temp_result+", "+tp+", "+tp2,a,false);
        free_reg(tp);
        free_reg(tp2);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("div "+tp+", "+tp2,a,false);
        write("mflo "+temp_result,a,false);
        free_reg(tp);
        free_reg(tp2);
        return -0.3;
    }
    if(op=="%"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("div "+tp+", "+tp2,a,false);
        write("mfhi "+temp_result,a,false);
        free_reg(tp);
        free_reg(tp2);
        return -0.3;
    }
    }
}

list<string> answer_poly;

string store2_do2(string *array, double *array_cor,int *indicator,int size,int nnum){
    stringstream mm;
    string name0;
    string sub_result;
    if(size>1){
        for(int i=0;i<size;i++){
            if(equation__key.count(array[i])==0){
                if((array_cor[i]-array_cor[i+1]>0.5&&array_cor[i]-array_cor[i-1]>0.5&&i<size-1)||(i>=size-1&&array_cor[i]-array_cor[i-1]>0.5)){
                    array_cor[i]-=1;
                    indicator[i]=0;
                }
            }
            //cout<<"show info:"<<array[i]<<"show indi:"<<indicator[i]<<endl;
            if(array[i]=="*"||array[i]=="/"||array[i]=="%"){
                if(array_cor[i-1]-array_cor[i]>0.5||array_cor[i+1]-array_cor[i]>0.5)
                    continue;
                if(indicator[i-1]!=1){
                indicator[i-1]=1;
                array_cor[i-1]+=0.3;
                }
                if(indicator[i]!=1){
                array_cor[i]+=0.3;
                indicator[i]=1;
                }
                if(indicator[i+1]!=1){
                array_cor[i+1]+=0.3;
                indicator[i+1]=1;
                }
            }
        }
        nnum++;
        for(int i=0;i<size;i++){
            if(equation__key.count(array[i])==0){
                if(array_cor[i]==array_cor[i+1]&&array_cor[i]==array_cor[i+2]){

                    mm<<nnum;
                    mm>>name0;
                    sub_result="value_of"+name0;
                    double change=do_ARthimetic(array[i],array[i+1],array[i+2],sub_result);
                    array_cor[i]+=change;
                    array[i]= "value_of"+name0;
                    if(i<size-3){
                    for(int j=i+1;j<size-2;j++){
                        array_cor[j]=array_cor[j+2];
                        array[j]=array[j+2];
                    }}
                    size-=2;
                    break;
                }
            }
        }//size-=1;
        double array_cor2[size];
        int indicator2[size];
        for(int i=0;i<size;i++){
            if(array_cor!=0){
                array_cor2[i]=int(array_cor[i]);
            }
            else{
                array_cor2[i]=0;
            }
            indicator2[i]=0;
        }
        nnum++;
        store2_do2(array,array_cor2,indicator2,size,nnum);
    }
    else{
        answer_poly.push_front(array[0]);
    }
    return sub_result;
}

//it shall return the register name of the final result;
string store_doing_Tree(string line){
    list<string> words;
    vector<int> layers;
    int size=equation_split_key(line,words,layers);
    string array[size];
    string sub_result;
    static int nnum=0;
    stringstream mm;
    string name0;
    double array_cor[size];
    int indicator[size];
    for(int i=0;i<size;i++){
        indicator[i]=0;
        array_cor[i]=0;
    }
    int now_info=size;
    int single_indicate=0;
    int alpha_indicate=0;
    for(int j=0;j<line.length();j++){
        if(equation_key.count(line.at(j))!=0&&line.at(j)!='=')
            single_indicate++;
        if(number_set.count(line.at(j))==0)
            alpha_indicate++;
    }
    if(single_indicate==0&&alpha_indicate==0){
        static int cons_value=0;
        int a;
        stringstream qq;
        qq<<cons_value;
        string cons_value_name;
        qq>>cons_value_name;
        string name="cons_value"+cons_value_name;
        string temp_reg=getReg(name);
        write("addi "+temp_reg+", $zero , "+line,a,false);
        return temp_reg;
    }
    else if(single_indicate==0&&alpha_indicate!=0)
        return getReg(line);
    while(words.empty()==false){
        now_info--;
        array[now_info]=words.back();
        array_cor[now_info]=layers.back();
        words.pop_back();
        layers.pop_back();
    }

    for(int i=0;i<size;i++){
        if(equation__key.count(array[i])==0){
            if((array_cor[i]-array_cor[i+1]>0.5&&array_cor[i]-array_cor[i-1]>0.5&&i<size-1)||(i>=size-1&&array_cor[i]-array_cor[i-1]>0.5)){
                array_cor[i]-=1;
                indicator[i]=0;
            }
        }
        if(array[i]=="*"||array[i]=="/"||array[i]=="%"){
            if(array_cor[i-1]-array_cor[i]>0.5||array_cor[i+1]-array_cor[i]>0.5)
                continue;
            if(indicator[i-1]!=1){
            indicator[i-1]=1;
            array_cor[i-1]+=0.3;
            }
            if(indicator[i]!=1){
            array_cor[i]+=0.3;
            indicator[i]=1;
            }
            if(indicator[i+1]!=1){
            array_cor[i+1]+=0.3;
            indicator[i+1]=1;
            }
        }
    }

    if(size>1){
        for(int i=0;i<size;i++){
            if(equation__key.count(array[i])==0){
                if((array_cor[i]-array_cor[i+1]>0.5&&array_cor[i]-array_cor[i-1]>0.5&&i<size-1)||(i>=size-1&&array_cor[i]-array_cor[i-1]>0.5)){
                    array_cor[i]-=1;
                    indicator[i]=0;
                }
            }
            if(array[i]=="*"||array[i]=="/"||array[i]=="%"){
                if(array_cor[i-1]-array_cor[i]>0.5||array_cor[i+1]-array_cor[i]>0.5)
                    continue;
                if(indicator[i-1]!=1){
                indicator[i-1]=1;
                array_cor[i-1]+=0.3;
                }
                if(indicator[i]!=1){
                array_cor[i]+=0.3;
                indicator[i]=1;
                }
                if(indicator[i+1]!=1){
                array_cor[i+1]+=0.3;
                indicator[i+1]=1;
                }
            }
        }

        nnum++;
        for(int i=0;i<size;i++){
            if(equation__key.count(array[i])==0){
                if(array_cor[i]==array_cor[i+1]&&array_cor[i]==array_cor[i+2]){
                    nnum+=1;
                    mm<<nnum;
                    mm>>name0;
                    sub_result="value_of"+name0;
                    double change=do_ARthimetic(array[i],array[i+1],array[i+2],sub_result);
                    if(i==0) array_cor[i]=array_cor[i+3];
                    else if(i==size-3) array_cor[i]=array_cor[i-1];
                    else{
                        if(array_cor[i-1]>array_cor[i+3]) array_cor[i]=array_cor[i-1];
                        else array_cor[i]=array_cor[i+3];
                    }

                    array[i]= "value_of"+name0;

                    if(i<size-3){
                    for(int j=i+1;j<size-2;j++){
                        array_cor[j]=array_cor[j+2];
                        array[j]=array[j+2];
                    }}
                    size-=2;
                    break;
                }
            }
        }//size-=1;
        double array_cor2[size];
        int indicator2[size];
        for(int i=0;i<size;i++){
            array_cor2[i]=0;
            indicator2[i]=0;
        }
        nnum++;
        store2_do2(array,array_cor,indicator2,size,nnum);
    }
    string final_answer=answer_poly.front();
    answer_poly.pop_front();
    return getReg(final_answer);

}



//split equation into assignment part and calculate part
int arithmetic(string line){
    string vari="";
    int posi;
    //0:none.1:++,2:--,3:+=,4:-=,5:*=,6:/=
    int self_doing=0;
    for(int i=0;i<line.length();i++){
        if(line.substr(i,2)=="++"){
            self_doing=1;
            break;
        }
        else if(line.substr(i,2)=="--"){
            self_doing=2;
            break;
        }
        else if(line.substr(i,2)=="+="){
            self_doing=3;
            break;
        }
        else if(line.substr(i,2)=="-="){
            self_doing=4;
            break;
        }
        else if(line.substr(i,2)=="*="){
            self_doing=5;
            break;
        }
        else if(line.substr(i,2)=="/="){
            self_doing=6;
            break;
        }
        self_doing=0;
    }

    for(int i=0;i<line.length();i++){
        if(line[i]!=' '&&line[i]!='='&&equation_key.count(line[i])==0) vari+=line[i];
        else{
            posi=i;
            break;}
    }
    string assign_reg=getReg(vari);
    int a;
    if(self_doing==0)
        write("add "+assign_reg+", $zero, "+store_doing_Tree(line.substr(posi+1)),a,false);
    else if(self_doing==1)
        write("addi "+assign_reg+", "+assign_reg+", 1",a,false);
    else if(self_doing==2)
        write("addi "+assign_reg+", "+assign_reg+", -1",a,false);
    else if(self_doing==3)
        write("add "+assign_reg+", "+assign_reg+", "+store_doing_Tree(line.substr(posi+2)),a,false);
    else if(self_doing==4)
        write("sub "+assign_reg+", "+assign_reg+", "+store_doing_Tree(line.substr(posi+2)),a,false);
    else if(self_doing==5)
        write("mul "+assign_reg+", "+assign_reg+", "+store_doing_Tree(line.substr(posi+2)),a,false);
    else if(self_doing==6)
        write("div "+assign_reg+", "+assign_reg+", "+store_doing_Tree(line.substr(posi+2)),a,false);


    return 0;
}

//handle comparision kinds of calculate and return the true/flase by equationR
int arithmetic(string line, string equationR){
    //0: assignment 1:< 2:> 3:<= 4:>= 5:== 6:!=
    //if equation is true it will give 0;
    int mode,posi,lengt;
    string word="";
    for(int i=0;i<line.length();i++){

        if(set_key.count(line.at(i))!=0&&i<line.length()-1){
            word+=line.at(i);
            posi=i;
            lengt=1;
            if(i<line.length()-2 && line.at(i+1)!='=') break;
            else if(i<line.length()-2){
                word+=line.at(i+1);
                lengt=2;
                break;
            }
        }
    }
    mode=mark_key.find(word)->second;
    if(mode==0){
        string vari="";
        for(int i=0;i<posi;i++){
            if(line[i]!=' ') vari+=line[i];
            else break;
        }
        //variable_detect(line);
        string temp=getReg(vari);
        string final_result;
        final_result= store_doing_Tree(line.substr(posi+lengt));
        int a;
        write("add "+temp+", "+"$zero, "+final_result,a,false);
    }
    else if(mode==1){
        string tL,tR;
        int a;
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tL+", "+tR,a,false);
        //result 0 means a<b;
    }
    else if(mode==2){
        string tL,tR;
        int a;
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tR+", "+tL,a,false);
        //result 0 means a>b;
    }
    else if(mode==3){
        string tL,tR;
        int a;
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tR+", "+tL,a,false);
        write("xori "+equationR+", "+equationR+", "+"1",a,false);
        //result 0 means a<=b;
    }
    else if(mode==4){
        string tL,tR;
        int a;
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tL+", "+tR,a,false);
        write("xori "+equationR+", "+equationR+", "+"1",a,false);
        //result 0 means a>=b;
    }
    else if(mode==5){
        string tL,tR;
        int a;
        string temp=getReg("temp_mode5");
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tL+", "+tR,a,false);
        write("xori "+equationR+", "+equationR+", "+"1",a,false);
        write("slt "+temp+", "+tR+", "+tL,a,false);
        write("xori "+temp+", "+temp+", "+"1",a,false);
        write("and "+equationR+", "+temp+", "+equationR,a,false);
        //result 0 means a==b;
    }
    else if(mode==5){
        string tL,tR;
        int a;
        string temp=getReg("temp_mode5");
        tL=store_doing_Tree(line.substr(0,posi));
        tR=store_doing_Tree(line.substr(posi+lengt));
        write("slt "+equationR+", "+tL+", "+tR,a,false);
        write("xori "+equationR+", "+equationR+", "+"1",a,false);
        write("slt "+temp+", "+tR+", "+tL,a,false);
        write("xori "+temp+", "+temp+", "+"1",a,false);
        write("and "+equationR+", "+temp+", "+equationR,a,false);
        write("xori "+equationR+", "+equationR+", "+"1",a,false);
        //result 0 means a!=b;
    }
    return 0;
}





//sentence part

int sentence_if(string line){
    int bracket_count=0;
    int end_bracket,begin_bracket;
    bracket_type.push_front(1);
    for(int i=1;i<line.length();i++){
        if(line.at(i)=='('){

            bracket_count++;
            if(bracket_count==1) begin_bracket=i+1;
        }
        else if (line.at(i)==')'&&bracket_count==1){
            end_bracket=i;
        }
    }
    uplevel();
    string beql=getReg("ifl");
    int label_index;
    //comared type arithmetic will automatically give corresponding MIPS instruction;
    //like a<b will give slt $s1, a, b;
    //then bne can be used to judge if it fits the condition.
    arithmetic(line.substr(begin_bracket,end_bracket-begin_bracket),beql);
    write("beq " +beql+", $zero"+" ,",label_index,true);



    if(judge_else()!=1) {
        free_reg(beql);
    }
    return 0;
}

int sentence_else(string line){
    int label_index;
    uplevel();
    string beql=getReg("ifl");
    write("bne "+beql+", $zero"+" ,",label_index,true);
    free_reg(beql);
    bracket_type.push_front(1);
    return 0;
}

int sentence_elseif(string line){}

int sentence_while(string line){
    int bracket_count=0;
    int end_bracket,begin_bracket;
    bracket_type.push_front(5);
    //read the condition of loop, use arithmetic to get result
    //compare arithmetic will autometically give corresponding MIPS code like slt;
    for(int i=1;i<line.length();i++){
        if(line.at(i)=='('){

            bracket_count++;
            if(bracket_count==1) begin_bracket=i+1;
        }
        else if (line.at(i)==')'&&bracket_count==1){
            end_bracket=i;
        }
    }
    uplevel();
    string beql=getReg("whilel");
    int label_index;
    string label_x=label_generate();
    loop_name_manage.push_front(label_x);
    write(label_x+":",label_index,false);
    arithmetic(line.substr(begin_bracket,end_bracket-begin_bracket),beql);
    //the condtional result is true then no jump
    write("beq "+beql+", $zero"+" ,",label_index,true);
    free_reg(beql);
    return 0;
}

int sentence_for(string line){
    static int for_counter=0;
    int bracket_count=0;
    int end_bracket,begin_bracket;
    bracket_type.push_front(2);
    for(int i=1;i<line.length();i++){
        if(line.at(i)=='('){

            bracket_count++;
            if(bracket_count==1) begin_bracket=i+1;
        }
        else if (line.at(i)==')'&&bracket_count==1){
            end_bracket=i;
        }
    }
    uplevel();
    string beql=getReg("forr");
    int label_index;
    string label_x=label_generate();
    //label should not be writen here as for has some pre-instruction
    loop_name_manage.push_front(label_x);
    //check iterator
    //check beq
    //do iteration
    string sentence=line.substr(begin_bracket,end_bracket-begin_bracket);
    string word="";
    list<string> words;

    int part=0;
    for(int i=0;i<end_bracket-begin_bracket;i++){
        if(i==end_bracket-begin_bracket-1){
            word+=sentence[i];
            words.push_back(word);
            break;
        }
        if(sentence.at(i)==';'){
            words.push_back(word);
            word="";
        }
        else{
            word+=sentence[i];
        }
    }
    string line1=words.front();
    //do int i=0
    string wordd="";
    for(int i=0;i<line1.size();i++){
        if(line1[i]==' ') break;
        else wordd+=line1[i];
    }
    scan_key(wordd,line1);
    words.pop_front();
    //
    line1=words.front();

    //every time start here first
    write(label_x+":",label_index,false);
    stringstream for_num;
    for_num<<for_counter;
    string for_n;
    for_num>>for_n;
    string iter=getReg("iter_"+for_n);

    //get the true/false determine if loop going on
    arithmetic(line1,iter);
    words.pop_front();

    //go iteration of this loop
    for_iter_list.push_front(words.front());

    //judge if jump out
    int a;
    write("beq "+iter+", $zero ,",a,true);
    words.pop_front();
    free_reg(beql);
    return 0;
}

void split(const string delimiter, string context, list<string> & list_w){
    size_t pos = 0;
    string token;
    while ((pos = context.find(delimiter)) != std::string::npos) {
        token = context.substr(0, pos);
        list_w.push_back(token);
        context.erase(0, pos + delimiter.length());
    }
    std::string::iterator end_pos = std::remove(context.begin(), context.end(), ' ');
    context.erase(end_pos, context.end());
    list_w.push_back(context);
}

int sentence_cin(string line){
    list<string> cin_list;
    string word="";
    int a;

    split(">>", line, cin_list);
    cin_list.pop_front();
    while(!cin_list.empty()){
        word=cin_list.front();
        if(variabels_map[word]=="int"){
            write("addi $v0, $zero, 5",a,false);
            write("syscall",a,false);
            write("add "+getReg(word)+", $zero, $v0",a,false);
        }
        else if(variabels_map[word]=="double"){
            write("addi $v0, $zero, 7",a,false);
            write("syscall",a,false);
            write("add "+getReg(word)+", $zero, $v0",a,false);
        }
        else if(variabels_map[word]=="string"){
            write("addi $v0, $zero, 8",a,false);
            int addr = adressAllocater(400);
            dataSecAdress[word] = addr;
            write("lui $a0, 80",a, false);
            write("addi $a0, $a0, "+to_string(dataSecAdress[word]-0x500000),a,false);
            string reserved;
            for(int i=0;i<399;i++){
                reserved += '/';
            }
            data_section.push_back(word+": .asciiz \""+reserved+"\"");
            write("addi $a1, $zero, 400", a, false);
            write("syscall",a,false);
        }
        else if(variabels_map[word]=="char"){
            write("addi $v0, $zero, 12",a,false);
            write("syscall",a,false);
            write("add "+getReg(word)+", $zero, $v0",a,false);
        }

        cin_list.pop_front();
    }
    //need to do sth with string
    return 0;
}

int sentence_cout(string line){
    list<string> cout_list;
    string word="";
    int a;
    split("<<", line, cout_list);
    cout_list.pop_front();
    while(!cout_list.empty()){
        word=cout_list.front();
        if(variabels_map[word]=="int"){
            write("addi $v0, $zero, 1",a,false);
            write("add $a0, $zero, "+getReg(word), a, false);
        }
        else if(variabels_map[word]=="double")
            write("addi $v0, $zero, 7",a,false);
        else if(variabels_map[word]=="string"){
            write("addi $v0, $zero, 4",a,false);
            write("lui $a0, 80",a, false);
            write("addi $a0, $a0, "+to_string(dataSecAdress[word]-0x500000),a,false);
//            write("addi $a0, $zero, "+to_string(dataSecAdress[word]), a, false);
        }
        else if(variabels_map[word]=="char")
            write("addi $v0, $zero, 11",a,false);
        else{
            write("add $v0, $zero, "+store_doing_Tree(word),a,false);
        }
        write("syscall",a,false);
        cout_list.pop_front();
    }
    //need to do sth to string
    return 0;
}

int sentence_break(){
    int a;
    write("j ",a,false);
    break_index_write.push_back(a);
    break_layer_num.push_back(bracket_type.size());
    return 0;
}


int sentence_continue(){
    int a;
    write("j "+ loop_name_manage.front(),a, false);
    return 0;
}


int sentence_int(string line){
    list<string> list_w;
    list<int> list_i;
    bool op_indi=false;
    bool func_indi=false;
    string word="";
    int len=line.length();
    string varname;
    for(int i=0;i<len;i++){
        if(i<len-1&&line[i]==' '){
            list_w.push_back(word);
            word="";
            list_i.push_back(i);
        }
        //else if(line[i]!=';') word+=line[i];
        else{
            word+=line[i];
            list_w.push_back(word);
        }
        if(line.at(i)=='='){
            varname = list_w.back();
            varname = varname.substr(0, varname.length()-1);
            op_indi=true;
        }
        if(line.at(i)=='{') func_indi=true;
    }
    string tyype=list_w.front();
    list_w.pop_front();
    if(!op_indi&&!func_indi){
        varname = list_w.back();
    }
    if(!func_indi){
        variabels_map[varname]="int";
        string regname=getReg(varname,true);
    }
    if(op_indi){
        arithmetic(line.substr(list_i.front()+1,len-list_i.front()-1));
    }
    if(func_indi){
        int a;
        uplevel();
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string function_name=line.substr(4,begin_bracket-4);
        if(line.substr(begin_bracket-4,4)=="main"){
            write("mainfunc: ",a,false);
            write("lui $gp, 96", a, false);
            write("lui $at, 112", a, false);
            bracket_type.push_front(4);
            return 0;
        }
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<paras.length();i++){
            if(paras[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==paras.length()-1){
                word+=paras[i];
                words.push_back(word);
            }
            else word+=paras[i];
        }
        vector<string> typ,name,name2;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(getparareg(ins.substr(blank+1)));
            name2.push_back(ins.substr(blank+1));
            words.pop_front();
            variabels_map[ins.substr(blank+1)] = "int";
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        v1.push_back(name2);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        saveRa();
        storeRegs();
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
        string_handle_para.push_front(function_name);
    }
}

int adressAllocater(int leng){
    static int init_address = 0x00500000;
    //to make sure the space is the multiple of 4
    int oldaddr = init_address;
    int extra_sapce = (leng%4) ? 1 : 0;
    init_address += (leng/4 + extra_sapce)*4;
    dataSecCount++;
    return  oldaddr;
}

int sentence_string(string line){
    list<string> list_w;
    list<int> list_i;
    bool op_indi=false;
    bool func_indi=false;
    string word="";
    int len=line.length();
    string varname;
    for(int i=0;i<len;i++){
        if(i<len-1&&line[i]==' '){
            list_w.push_back(word);
            word="";
            list_i.push_back(i);
        }
        //else if(line[i]!=';') word+=line[i];
        else{
            word+=line[i];
            list_w.push_back(word);
        }
        if(line.at(i)=='=') {
            varname = list_w.back();
            varname = varname.substr(0, varname.length()-1);
            op_indi=true;
        }
        if(line.at(i)=='{') func_indi=true;
    }
    if(!op_indi&&!func_indi){
        varname = list_w.back();
    }
    if(!func_indi){
        variabels_map[varname]="string";
        string regname=getReg(list_w.front(),true);
    }
    if(op_indi){
        string context;
        int start = line.find("\"");
        int end = line.rfind("\"");
        context = line.substr(start+1, end-start-1) + "\0";
        int addr = adressAllocater(end-start);
        int a=0;
        data_section.push_back(varname+": .asciiz \""+context+"\"");
//        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .asciiz \""+context+"\"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        uplevel();
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<paras.length();i++){
            if(paras[i]==','){
                words.push_back(word);
                word="";
            }
            else if(i==paras.length()-1){
                word+=line[i];
                words.push_back(word);
            }
            else{
                word+=paras[i];
            }
        }
        vector<string> typ,name,name2;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(getparareg(ins.substr(blank+1)));
            name2.push_back(ins.substr(blank+1));
            words.pop_front();
            variabels_map[ins.substr(blank+1)] = "string";
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        v1.push_back(name2);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        saveRa();
        storeRegs();
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
        string_handle_para.push_front(function_name);
    }
}

int sentence_char(string line){
    list<string> list_w;
    list<int> list_i;
    bool op_indi=false;
    bool func_indi=false;
    string word="";
    int len=line.length();
    string varname;
    for(int i=0;i<len;i++){
        if(i<len-1&&line[i]==' '){
            list_w.push_back(word);
            word="";
            list_i.push_back(i);
        }
        //else if(line[i]!=';') word+=line[i];
        else{
            word+=line[i];
            list_w.push_back(word);
        }
        if(line.at(i)=='=') {
            varname = list_w.back();
            varname = varname.substr(0, varname.length()-1);
            op_indi=true;
        }
        if(line.at(i)=='{') func_indi=true;
    }
    string tyype=list_w.front();
    list_w.pop_front();
    if(!op_indi&&!func_indi){
        varname = list_w.back();
    }
    if(!func_indi){
        variabels_map[varname]="char";
        string regname=getReg(varname,true);
    }
    if(op_indi){
        string context;
        int start = line.find("\"");
        int end = line.rfind("\"");
        context = line.substr(start+1, end-start-1);
        int addr = adressAllocater(end-start-1);
        varname = varname.substr(0, varname.length()-2);
        data_section.push_back(varname+": .ascii \""+context+"\"");
//        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .ascii \""+context+"\"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        uplevel();
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<paras.length();i++){
            if(paras[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==paras.length()-1){
                word+=paras[i];
                words.push_back(word);
            }
            else word+=paras[i];
        }
        vector<string> typ,name,name2;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(getparareg(ins.substr(blank+1)));
            name2.push_back(ins.substr(blank+1));
            words.pop_front();
            variabels_map[ins.substr(blank+1)] = "char";
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        v1.push_back(name2);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        saveRa();
        storeRegs();
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
        string_handle_para.push_front(function_name);
    }
}

int sentence_short(string line){
    list<string> list_w;
    list<int> list_i;
    bool op_indi=false;
    bool func_indi=false;
    string word="";
    int len=line.length();
    string varname;
    int posiOfEqual;
    for(int i=0;i<len;i++){
        if(i<len-1&&line[i]==' '){
            list_w.push_back(word);
            word="";
            list_i.push_back(i);
        }
        //else if(line[i]!=';') word+=line[i];
        else{
            word+=line[i];
            list_w.push_back(word);
        }
        if(line.at(i)=='=') {
            varname = list_w.back();
            varname = varname.substr(0, varname.length()-1);
            posiOfEqual = i;
            op_indi=true;
        }
        if(line.at(i)=='{') func_indi=true;
    }
    string tyype=list_w.front();
    list_w.pop_front();
    if(!op_indi&&!func_indi){
        varname = list_w.back();
    }
    if(!func_indi){
        variabels_map[varname]="short";
        string regname=getReg(list_w.front(),true);
    }
    if(op_indi){
        stringstream data(line.substr(posiOfEqual+1));
        short shortnum;
        data >> shortnum;
        int addr = adressAllocater(4);
        data_section.push_back(varname+": .half "+ to_string(shortnum) +"");
//        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .half "+ to_string(shortnum) +"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        uplevel();
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<paras.length();i++){
            if(paras[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==paras.length()-1){
                word+=paras[i];
                words.push_back(word);
            }
            else word+=paras[i];
        }
        vector<string> typ,name,name2;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(getparareg(ins.substr(blank+1)));
            name2.push_back(ins.substr(blank+1));
            words.pop_front();
            variabels_map[ins.substr(blank+1)] = "short";
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        v1.push_back(name2);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        saveRa();
        storeRegs();
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
        string_handle_para.push_front(function_name);
    }
}

int sentence_return(string line){
    list<string> words;
    string word="";
    for(int i=0;i<line.length();i++){
        if(line[i]==' '){
            words.push_back(word);
            word="";
        }
        else if(i==line.length()-1){
            word+=line[i];
            words.push_back(word);
        }
        else{
            word+=line[i];
        }
    }
    int a;
    string current_return=getReg("temp_func");
    arithmetic("temp_func="+words.back());
    write("j ",a,false);
    return_list.push_front(a);
    return 0;
}

int do_function(string name,string line){
    int a;
    int begin_bracket,end_bracket;
    begin_bracket=line.find("(");
    end_bracket=line.find(")");
    string word;
    list<string> words;
    string content=line.substr(begin_bracket+1,end_bracket-begin_bracket-1);
    for(int i=0;i<content.length();i++){
        if(content[i]==','){
            words.push_back(word);
            word="";
        }
        else if(i==content.length()-1){
            word+=content[i];
            words.push_back(word);
        }
        else{
            word+=content[i];
        }
    }
    int c=0;
    //parameter transmit
    list<string> words2;
    while(!words.empty()){
        arithmetic("value_par_cal="+words.front());
        write("add "+getReg(words.front())+", $zero, "+getReg("value_par_cal"),a,false);
        write("add "+getReg("teempk1")+", $zero, $k1",a,false);
        write("add "+getReg("teempsp")+", $zero, $sp",a,false);
        write("add "+getReg("teempa3")+", $zero, $a3",a,false);
        if(getReg(words.front())=="$k1"||getReg(words.front())=="$sp"||getReg(words.front())=="$a3"){
            write("add "+getReg(words.front(),true,true)+", $zero, "+getReg(words.front()),a,false);
        }
        string aword = words.front();
        words2.push_front(aword);
        words.pop_front();
    }

    while(!words2.empty()){
        string a_name=function_par_and_type[name][1][c];
        write("add "+a_name+", $zero, "+getReg(words2.front(),true,true),a,false);
        words2.pop_front();
        c++;
    }
    //go to function
    write("jal "+function_name_manage[name],a,false);
    write("add $k1, $zero, "+getReg("teempk1"),a,false);
    write("add $sp, $zero, "+getReg("teempsp"),a,false);
    write("add $a3, $zero, "+getReg("teempa3"),a,false);
    return 0;
}

int read_main(){
    int a;
    write("j mainfunc",a,false);
    return 0;
}

int scan_key(string firw, string line){
    string secw;
    static int order_scan=0;
    if(order_scan==0){
        read_main();
        order_scan++;
    }
    static int funcount=0;
    if(line[line.length()-1]==';'){
        line = line.substr(0, line.length()-1);
    }
    //when you meet function calling
    if(firw[0]=='#') return 0;
    if(firw=="using") return 0;
    if(function_name_manage.count(firw)!=0){
        funcount++;
        stringstream fu;
        fu << funcount;
        string n1;
        fu>>n1;
        function_stack_cal.push_front(firw+n1);
        do_function(firw,line);
    }
    else if(instruction_key_word.count(firw)==0){
        arithmetic(line);
    }
    else if(firw=="if") sentence_if(line);

    else if(firw=="else"){

        if(secw=="if") sentence_elseif(line);
        else sentence_else(line);
    }

    else if(firw=="while") sentence_while(line);

    else if(firw=="for") sentence_for(line);

    else if(firw=="cin") sentence_cin(line);

    else if(firw=="cout") sentence_cout(line);

    else if(firw=="int") sentence_int(line);

    else if(firw=="char") sentence_char(line);

    else if(firw=="string") sentence_string(line);

    else if(firw=="short") sentence_short(line);

    else if(firw=="break") sentence_break();

    else if(firw=="continue") sentence_continue();

    else if(firw=="}") {
        write_bracket();
    }

    else if(firw=="{") write_bracket_front();

    else if(firw=="return") sentence_return(line);

    return 0;
}

int compileCode(string f_path, string e_path){
    string m_path = "tmp\\temp.txt";
    readFileFromPath(f_path);
    fstream fp;
    fp.open(m_path,ios::in);
    int c = 0;
    int c1 = 0;
    for (int line_num = 0; c != EOF; line_num++){
        c = 0;
        char line[100] = {0};
        for (int i = 0; c != EOF && i < 200; i++){
            c=fp.get();
            if(c=='('){
                line[i]=c;
                c1=fp.get();
                while(c1!=')'){
                   i++;
                   line[i]=c1;
                   c1=fp.get();
                }
                i++;
                line[i]=c1;
                c1=fp.get();
                if(c1=='c'||c1=='b'||c1==';'){
                    if(c1==';'){
                        i++;
                        line[i]=c1;
                        break;
                    }
                    else{
                        i++;
                        line[i]=c1;
                        c1=fp.get();
                        while(c1!=';'){
                            i++;
                            line[i]=c1;
                            c1=fp.get();
                        }
                        i++;
                        line[i]=c1;
                        break;
                    }
                }
                else{
                    while(c1!='{'&&c1!=';'){
                        i++;
                        if(c1=='\n'){
                            i--;
                            c1=fp.get();
                            continue;
                        }
                        line[i]=c1;
                        c1=fp.get();
                    }
                    i++;
                    line[i]=c1;
                    break;
                }
            }
            else if (c==';'||c=='{'||c=='}')
            {
                line[i]=c;
                break;
            }
            else if(c=='\n'){
                i--;
            }
            else{
                line[i]=c;
            }
        }
        if(c==EOF) break;
        string sen=line;
        if (sen=="using namespace std;") continue;
        sen.erase(0,sen.find_first_not_of(" "));
        char *tokenPtr6=strtok(line,";");
        char *tokenPtr5=strtok(tokenPtr6,"{");
        char *tokenPtr4=strtok(tokenPtr5,">");
        char *tokenPtr3=strtok(tokenPtr4,"=");
        char *tokenPtr2=strtok(tokenPtr3,"(");
        char *tokenPtr1=strtok(tokenPtr2," ");
        char *tokenPtr=strtok(tokenPtr1,"<");
        scan_key(tokenPtr,sen);
    }
    things_that_would_wrtie.insert(things_that_would_wrtie.begin(), ".text");
    for(int i = data_section.size()-1; i>=0; i--){
        things_that_would_wrtie.insert(things_that_would_wrtie.begin(), data_section[i]);
    }
    things_that_would_wrtie.insert(things_that_would_wrtie.begin(), ".data");

    //clean the origin asm file
    ofstream createFile(e_path, ios::out|ios::trunc);
    createFile.close();

    fstream outp(e_path, ios::out);
    for(int i=0;i< things_that_would_wrtie.size();i++){
        outp << things_that_would_wrtie[i]<<endl;
    }

    outp << endl;
    fp.close();
    outp.close();

    return 0;

}

/*
int main(){
//    compileCode("C:\\Users\\HP\\Desktop\\new10\\new8\\test.cpp", "C:\\Users\\HP\\Desktop\\new10\\new8\\test.asm");
//    compileCode("C:\\Users\\10514\\Desktop\\new8(6)\\new8\\test.cpp", "C:\\Users\\10514\\Desktop\\new8(6)\\new8\\test.asm");
    compileCode("D:\\CSC3002\\project\\Compiler\\new8777\\new86\\new8\\test.cpp", "D:\\CSC3002\\project\\Compiler\\new8777\\new86\\new8\\test.asm");
    cout << "code is compiled successfully" <<endl;
    return 0;
}
*/
