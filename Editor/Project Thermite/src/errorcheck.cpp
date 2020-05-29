#include "variables.h"
#include "semicolon.h"
#include <string>

using namespace std;

int ErrorCheck(string f_path, string of_path)
{
    bool judge = semicolon_check(f_path, of_path);
    if (judge) {
        variable_check(f_path, of_path);
    }
    return 0;
}

