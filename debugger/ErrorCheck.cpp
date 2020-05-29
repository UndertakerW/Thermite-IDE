#include "VariableCheck.h"
#include "Semicolon.h"
#include <string>

using namespace std;

int ErrorCheck(string f_path, string of_path)
{
    bool judge = semicolon_check(f_path, of_path);
    if (judge) {
        variable_check("/Users/liuyi/Desktop/new86/new8/new8/test.txt", "/Users/liuyi/Desktop/new86/new8/new8/empty.txt");
    }
    return 0;
}

