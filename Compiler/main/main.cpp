#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "functions.h"
using namespace  std;
int main()
{
   string f_path="C:\\Users\\HP\\Desktop\\test.txt";
   string e_path="C:\\Users\\HP\\Desktop\\test1.txt";
   readFileFromPath(f_path);
   fstream fp;
   fp.open(e_path,ios::in);
   char c = 0;
   for (int line_num = 0; c != EOF; line_num++){
          c = 0;
          char line[100] = {0};
          for (int i = 0; c != EOF && i < 200; i++){
              fp.get(c);
              if (c==';'||c=='{'||c=='}')
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
          string sen=line;
          sen.erase(0,sen.find_first_not_of(" "));
          cout<<sen<<endl;
          //后续
}
   return 0;
}
