#include <iostream>
#include "VariableCheck.h"
#include "Semicolon.h"
#include "Brackets.h"
#include <string>

using namespace std;

int ErrorCheck(string f_path, string of_path)
{
    bool judge = semicolon_check(f_path, of_path);
    if (judge) {
        bool judge1 = brackets_check(f_path, of_path);
        if (judge1) {
            bool judge2 = FunctionCheck(f_path, of_path);
            if (judge2) {
                variable_check(f_path, of_path);
            }
        }
    }
    return 0;
}

