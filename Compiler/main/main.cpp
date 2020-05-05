#include<iostream>
#include<set>
#include<string>
#include<map>
#include<vector>
#include<list>
#include<fstream>
#include<sstream>

using namespace std;

const set<string> instruction_key_word={"if","else","while","for","cin","cout","int","char","string","{","}", "short"};

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

string getReg(string var){
    return "$"+var;
}

string free_reg(string var){}


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
void uplevel(){}
//register part
void delevel(){}




//write the code of MIPS, and also add label if the requirement is ok
//once this line is beq or bne instruction which should be put a label
//then just assign the paramenter last to be true.
//then it can handle to put the label at right place
int write(string MIPS,int &get_index,bool haslabel){
    static int counter=0;
    cout<<"mips:    "<<MIPS<<endl;
    //static int label_need=0;
    things_that_would_wrtie.push_back(MIPS);
    if(haslabel){
        labels_manage.push_front(counter);
    }
    //cout<<labels_manage.size()<<endl;
    //cout<<label_name_manage.size()<<endl;
    if(labels_manage.size()==label_name_manage.size()){
        while (!labels_manage.empty()) {
            int index= labels_manage.front();
            things_that_would_wrtie[index]= things_that_would_wrtie[index]+label_name_manage.back();
            labels_manage.pop_front();
            label_name_manage.pop_back();
        }
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
    delevel();
    //if type 1: generate a label
    if(bracket_type.front()==1){
        string label=label_generate();
        int a;
        label_name_manage.push_front(label);
        write(label+": ",a,false);
        bracket_type.pop_front();
    }
    if(bracket_type.front()==2){
        int a;
        write("j "+ loop_name_manage.front(),a, false);
        loop_name_manage.pop_front();
        string label= label_generate();
        write(label+": ",a,false);
        label_name_manage.push_front(label);
        bracket_type.pop_front();
    }
    if(bracket_type.front()==3){
        int a;
        write("jr $ra",a,false);
    }
    //if type 2: beq to loop label, pop loop list, generate a label
    return 0;
}

//the above deal with the label and bracket structure.
//the below deal with the math operation structure.
//tips: use tree to store order of arithmetic.





//split the equation into variables and values and signs, and store them into the list and vector
int equation_split_key(string line, list<string> &words,vector<int> &layers){
    words.clear();
    string this_word="";
    int layer=0;
    int size=0;
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
    cout<<"do arthimetic"<<endl;
    int a;
    string a1=getReg(a_1);
    string a2=getReg(a_2);
    string temp_result=getReg(temp__result);
    cout<<test_vari(a_1)<<test_vari(a_2)<<endl;
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

    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        write("mult "+temp_result+", "+a1+", "+a2,a,false);
        return -0.3;
    }
    if(op=="/"){
        write("div "+temp_result+", "+a1+", "+a2,a,false);
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

    //need to handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("mult "+temp_result+", "+a1+", "+tp,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a2,a,false);
        write("div "+temp_result+", "+a1+", "+tp,a,false);
        free_reg(tp);
        return -0.3;
    }
    }
    //first is number, second is variable
    else if(!test_vari(a_1)&&test_vari(a_2)){
        a1=a_1;
    if(op=="+"){
        write("addi "+temp_result+", "+a1+", "+a2,a,false);
        return 0;
    }
    if(op=="-"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("sub "+temp_result+", "+a2+", "+tp,a,false);
        free_reg(tp);
        return 0;
    }

    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("mult "+temp_result+", "+a2+", "+tp,a,false);
        free_reg(tp);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("div "+temp_result+", "+a2+", "+tp,a,false);
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

    //handle the hi & lo and assign to temp_result


    if(op=="*"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("mult "+temp_result+", "+tp+", "+tp2,a,false);
        free_reg(tp);
        free_reg(tp2);
        return -0.3;
    }
    if(op=="/"){
        string tp=getReg("temmp");
        string tp2=getReg("temmp2");
        write("addi "+tp+", $zero, "+a1,a,false);
        write("addi "+tp2+", $zero, "+a2,a,false);
        write("div "+temp_result+", "+tp+", "+tp2,a,false);
        free_reg(tp);
        free_reg(tp2);
        return -0.3;
    }
    }
}




//it shall return the register name of the final result;
string store_doing_Tree(string line){
    list<string> words;
    vector<int> layers;
    int size=equation_split_key(line,words,layers);
    string array[size];
    string sub_result;
    stringstream mm;
    string name0;
    double array_cor[size];
    int indicator[size];
    int now_info=size;
    int single_indicate=0;
    int alpha_indicate=0;
    for(int j=0;j<line.length();j++){
        if(equation_key.count(line.at(j))!=0&&line.at(j)!='=')
            single_indicate++;
        if(number_set.count(line.at(j))==0)
            alpha_indicate++;
    }
    cout<<alpha_indicate<<endl;
    if(single_indicate==0&&alpha_indicate==0){
        static int cons_value=0;
        int a;
        stringstream qq;
        qq<<cons_value;
        string cons_value_name;
        qq>>cons_value_name;
        string name="cons_value"+cons_value_name;
        string temp_reg=getReg(name);
        write("addi "+temp_reg+", $zero, "+line,a,false);
        return temp_reg;
    }
    else if(single_indicate==0&&!alpha_indicate==0)
        return getReg(line);
    cout<<"new"<<endl;
    while(words.empty()==false){
        now_info--;
        cout<<words.back()<<" "<<layers.back()<<endl;
        array[now_info]=words.back();
        array_cor[now_info]=layers.back();
        words.pop_back();
        layers.pop_back();
    }cout<<size<<endl<<endl;
    while(size>1){
        for(int i=0;i<size;i++){
            if(equation__key.count(array[i])==0){
                cout<<array[i]<<endl;
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
        static int nnum=0;
        nnum++;
        cout<<"new2"<<endl;
        for(int i=0;i<size;i++){
            cout<<"222  "<<array[i]<<endl;
            cout<<"333  "<<array_cor[i]<<endl;
        }
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
                    for(int i=0;i<size;i++){
                        cout<<"layer:"<<array_cor[i]<<endl;
                        cout<<"thing:"<<array[i]<<endl;
                    }
                    size-=2;
                    cout<<"size "<<size<<endl;
                    break;
                }
            }
        }//size-=1;
    }
    return getReg(sub_result);

}



//split equation into assignment part and calculate part
int arithmetic(string line){
    cout<<"arithmetic"<<endl;
    cout<<line<<endl;
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
    cout<<vari<<endl;
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
        write("mult "+assign_reg+", "+assign_reg+", "+store_doing_Tree(line.substr(posi+2)),a,false);
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
    cout<<line<<endl;
    for(int i=0;i<line.length();i++){

        if(set_key.count(line.at(i))!=0&&i<line.length()-1){
            cout<<1;
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
    cout<<"arithmetic doing0 "<<mode<<endl;
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
        write("or "+equationR+", "+temp+", "+equationR,a,false);
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
    cout<<"arithmetic doing1"<<endl;
    return 0;
}





//sentence part

int sentence_if(string line){
    cout<<"if doing"<<endl;
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
    string beql=getReg("ifl");
    int label_index;
    //comared type arithmetic will automatically give corresponding MIPS instruction;
    //like a<b will give slt $s1, a, b;
    //then bne can be used to judge if it fits the condition.
    arithmetic(line.substr(begin_bracket,end_bracket-begin_bracket),beql);

    cout<<"working"<<endl;
    write("bne " +beql+", 0"+" ,",label_index,true);



    if(judge_else()!=1) {
        free_reg(beql);
    }
    bracket_type.push_front(1);
    return 0;
}

int sentence_else(string line){
    int label_index;
    bracket_type.push_front(1);
    string beql=getReg("ifl");
    write("beq "+beql+", 0"+" ,",label_index,true);
    free_reg(beql);
    bracket_type.push_front(1);
    return 0;
}

int sentence_elseif(string line){}

int sentence_while(string line){
    int bracket_count=0;
    int end_bracket,begin_bracket;
    bracket_type.push_front(2);
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
    string beql=getReg("whilel");
    int label_index;
    string label_x=label_generate();
    loop_name_manage.push_front(label_x);
    write(label_x+":",label_index,false);
    arithmetic(line.substr(begin_bracket,end_bracket-begin_bracket),beql);
    //the condtional result is true then no jump
    write("bne "+beql+", 0"+" ,",label_index,true);
    free_reg(beql);
    bracket_type.push_front(2);
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
            words.push_front(word);
            break;
        }
        if(sentence.at(i)==';'){
            words.push_front(word);
            cout<<word<<endl;
            word="";
        }
        else{
            word+=sentence[i];
        }
    }
    string line1=words.front();
    uplevel();
    //do int i=0
    arithmetic(line1);
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
    arithmetic(words.front());

    //judge if jump out
    int a;
    write("bne "+iter+", 0 ,",a,true);
    words.pop_front();

    /*
    while(!words.empty()){
        int a;
        string iter=getReg("iter");
        if(part==0){


            string vari="";
            string value_assign="";
            string line1=words.front();
            for(int i=0;i<line1.length();i++){
                if(line1.substr(i,3)=="int"){
                    for(int j=i+3;j<line1.length()-i-3;j++){
                        if(line1.at(j)==' ') continue;
                        if(line1.at(j)!='=') vari+=line1.at(j);
                        else break;
                    }
                    break;
                }
            }
            cout<<"line1:"<<line1<<endl;
            for(int i=0;i<line1.length();i++){
                if(line1[i]=='='){
                    for(int j=i+1;j<line1.length()-i-1;j++){
                        if(line1.at(j)==' '||line1.at(j)==';') continue;
                        if(number_set.count(line1[j]!=0)) value_assign+=line1.at(j);
                        else break;
                    }
                    break;
                }
            }
            string regis_assign=getReg(vari);
            //read part0 sentence->words.front
            write("addi "+regis_assign+", $zero, "+value_assign,a,false);
            write("add "+iter+", $zero, "+regis_assign,a,false);




            words.pop_front();
            part++;
        }

        if(part==1){

        }



    }
    */
    free_reg(beql);
    bracket_type.push_front(2);
    return 0;
}

int sentence_cin(string line){
    list<string> cin_list;
    string word="";
    int a;
    for(int i=0;i<line.length();i++){
        if(line.substr(i,2)==">>"){

            cin_list.push_front(">>");
            for(int j=i+2;j<line.length()-i-2;j++){
                if(i<line.length()-1&&(line.at(i+1)==' '||line.at(i+1)=='>'||line.at(i+1)==';')) cin_list.push_back(word);
                else word+= line.at(i);

            }
        }
    }
    //not done yet!!!!!!!!!!!!!
    while(!cin_list.empty()){
        word=cin_list.front();
        if(variabels_map[word]=="int")
            write("addi $v0, $zero, 5",a,false);
        else if(variabels_map[word]=="double")
            write("addi $v0, $zero, 7",a,false);
        else if(variabels_map[word]=="string")
            write("addi $v0, $zero, 8",a,false);
        else if(variabels_map[word]=="char")
            write("addi $v0, $zero, 12",a,false);
        write("syscall",a,false);
        cin_list.pop_front();
    }
    //need to do sth with string
    return 0;
}

int sentence_cout(string line){
    list<string> cout_list;
    string word="";
    int a;
    for(int i=0;i<line.length();i++){
        if(line.substr(i,2)==">>"){

            cout_list.push_front(">>");
            for(int j=i+2;j<line.length()-i-2;j++){
                if(i<line.length()-1&&(line.at(i+1)==' '||line.at(i+1)=='<'||line.at(i+1)==';')) cout_list.push_front(word);
                else word+= line.at(i);

            }
        }
    }
    //not done yet!!!!!!!!!!!!!!!
    while(!cout_list.empty()){
        word=cout_list.front();
        if(variabels_map[word]=="int")
            write("addi $v0, $zero, 5",a,false);
        else if(variabels_map[word]=="double")
            write("addi $v0, $zero, 7",a,false);
        else if(variabels_map[word]=="string")
            write("addi $v0, $zero, 8",a,false);
        else if(variabels_map[word]=="char")
            write("addi $v0, $zero, 12",a,false);
        write("syscall",a,false);
        cout_list.pop_front();
    }
    //need to do sth to string
    return 0;
}



int sentence_int(string line){
    list<string> list_w;
    list<int> list_i;
    bool op_indi=false;
    bool func_indi=false;
    string word="";
    int len=line.length();
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
        if(line.at(i)=='=') op_indi=true;
        if(line.at(i)=='{') func_indi=true;
    }
    string tyype=list_w.front();
    list_w.pop_front();
    variabels_map[list_w.front()]=tyype;
    string regname=getReg(list_w.front());
    if(op_indi){
        arithmetic(line.substr(list_i.front()+1,len-list_i.front()-1));
    }
    if(func_indi){
        int a;
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==line.length()-1){
                words.push_back(word);
            }
        }
        vector<string> typ,name;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(ins.substr(blank+1));
            words.pop_front();
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
    }
}

static int dataSecCount=0;

//save the address of the string, char, short variables
map<string, int> dataSecAdress;

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
    string tyype=list_w.front();
    list_w.pop_front();
    variabels_map[list_w.front()]=tyype;
    string regname=getReg(list_w.front());
    if(op_indi){
        string context;
        int start = line.find("\"");
        int end = line.rfind("\"");
        context = line.substr(start+1, end-start-1) + "\0";
        int addr = adressAllocater(end-start);
        int a=0;
        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .asciiz \""+context+"\"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==line.length()-1){
                words.push_back(word);
            }
        }
        vector<string> typ,name;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(ins.substr(blank+1));
            words.pop_front();
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
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
    cout << "-----test--------" << endl;
    for(string a : list_w){
        cout << a << endl;
    }
    cout << "-----------------" << endl;
    string tyype=list_w.front();
    list_w.pop_front();
    variabels_map[list_w.front()]=tyype;
    string regname=getReg(list_w.front());
    if(op_indi){
        string context;
        int start = line.find("\"");
        int end = line.rfind("\"");
        context = line.substr(start+1, end-start-1);
        int addr = adressAllocater(end-start-1);
        varname = varname.substr(0, varname.length()-2);
        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .ascii \""+context+"\"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==line.length()-1){
                words.push_back(word);
            }
        }
        vector<string> typ,name;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(ins.substr(blank+1));
            words.pop_front();
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
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
    variabels_map[list_w.front()]=tyype;
    string regname=getReg(list_w.front());
    if(op_indi){
        stringstream data(line.substr(posiOfEqual+1));
        short shortnum;
        data >> shortnum;
        int addr = adressAllocater(4);
        things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount-1, varname+" : .half "+ to_string(shortnum) +"");
        dataSecAdress[varname] = addr;
    }
    if(func_indi){
        int a;
        int begin_bracket,end_bracket;
        begin_bracket=line.find("(");
        end_bracket=line.find(")");
        string paras=line.substr(begin_bracket+1,end_bracket-1-begin_bracket);
        string word="";
        list<string> words;
        for(int i=0;i<line.length();i++){
            if(line[i]==','){
                words.push_back(word);
                word="";
            }
            if(i==line.length()-1){
                words.push_back(word);
            }
        }
        vector<string> typ,name;
        while(!words.empty()){
            string ins=words.front();
            int blank=ins.find(' ');
            typ.push_back(ins.substr(0,blank));
            name.push_back(ins.substr(blank+1));
            words.pop_front();
        }
        vector<vector<string>> v1;
        v1.push_back(typ);
        v1.push_back(name);
        string function_name=line.substr(4,begin_bracket-4);
        string label=label_generate();
        function_par_and_type[function_name]=v1;
        write(label+": ",a,false);
        function_name_manage[function_name]=label;
        bracket_type.push_front(3);
    }
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
        if(line[i]==','){
            words.push_back(word);
            word="";
        }
        if(i==line.length()-1){
            words.push_back(word);
        }
    }
    int c=0;
    //parameter transmit
    while(!words.empty()){
        string a_name=function_par_and_type[name][1][c];
        write("addi "+getReg(words.front())+", $zero, "+getReg(a_name),a,false);
        words.pop_front();
    }
    //go to function
    write("jal "+function_name_manage[name],a,false);
    return 0;
}

int scan_key(string firw, string line){
    string secw;
    //when you meet function calling
    if(function_name_manage.count(firw)!=0)
        do_function(firw,line);
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

    else if(firw=="}") write_bracket();

    else if(firw=="{") write_bracket_front();

    return 0;
}
int main(){
    /*
    scan_key("int","int a=2");
    scan_key("if","if(a==b){");
    scan_key("if","if(a==2){");
    scan_key("a","a=b+c*d");
    scan_key("}","}");
    scan_key("b","b=12+13");
    scan_key("}","}");*/

    scan_key("int","int testfunc(int test_0){");
    scan_key("test_0","test_0+=999");
    scan_key("}","}");
    scan_key("a","a=5");
    scan_key("testfunc","testfunc(a)");
    scan_key("string","string str1=\"Welcome to this grading mini program!\"");
    scan_key("string","string str2=\"Good bye!\"");
    scan_key("string","string str3=\"Please enter the student number (1-10):\"");
    scan_key("string","string str4=\"Please enter the grade (0-100):\"");
    scan_key("char","char e[]=\"abcd\"");
    scan_key("char","char f[]=\"ad\"");
    scan_key("char","char g[]=\"i\"");
    scan_key("short","short num=10000000");
    scan_key("short","short num2=10");

/*
    scan_key("for","for(int i=0;i<5;i++){");
    scan_key("g","g*=5");
    scan_key("while","while(a<1){");
    scan_key("if","if(test_a>=test_b){");
    scan_key("goo","goo=2+b");
    scan_key("}","}");
    scan_key("a","a+=1");
    scan_key("}","}");
    scan_key("j","j+=1");
*/
    things_that_would_wrtie.insert(things_that_would_wrtie.begin()+dataSecCount, ".text");
    things_that_would_wrtie.insert(things_that_would_wrtie.begin(), ".data");

    for(int i=0;i<things_that_would_wrtie.size();i++){
    cout<<things_that_would_wrtie[i]<<endl;}

    cout << endl;

    for(auto it: dataSecAdress){
        cout << hex << it.first << " : " << it.second << dec << endl;
    }


    return 0;
}
