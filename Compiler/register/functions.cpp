#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <fstream>
#include "functions.h"
using namespace  std;

/*function name:removecomments
 * useage:removeComments(input_file,output_file)
 * ------------------------------------------------
 * description: recongnize the comment context in the file
 *              write the uncomment file in the output_file
 */
void removeComments(istream & is, ostream & os) {
    char ch,ch1;
       while(is.get(ch))
       {
           if (ch=='/')
           {
               is.get(ch1);
               if (ch1=='/')
               {
                   while(is.get(ch))
                   {
                       if (ch=='\n')
                       {
                           os.put(ch);
                           break;
                       }
                   }
               }
               else if (ch1=='*')
               {
                 while(is.get(ch))
                 {
                     if (ch=='/')
                     {
                        break;
                     }
                 }
               }
           }
           else if (ch=='\n')
           {
              os.put(ch);
           }
           else
           {
               os.put(ch);
           }
       }
}

/*function name:readFileFrompath1
 * usage: readFileFrompath(f_path)
 *----------------------------------
 * description:access input file and creatr output file and check whether user's input is right.
 */
void readFileFromPath(string f_path) {
    fstream file;
    file.open(f_path,ios::in);
    ofstream outfile;
    outfile.open("C:\\Users\\HP\\Desktop\\test1.txt",ios::out);
    if (!file)
    {
      cout<<"no file to be found!\n";
     }
    else {
        removeComments(file,outfile);
    }
}
