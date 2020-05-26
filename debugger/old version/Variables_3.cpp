#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>

using namespace std;

int line_number = 0;

map<string, string> variable_type;
map<string, bool> variable;

const set<char> operation = {'=', '+', '-', '*', '/', '%', '|', '&'};

bool AllisNum(string str) {
    int length = str.size();
    for (int i = 0; i < length; i++) {
        int tmp = (int) str[i];
        if ((tmp >= 48) && (tmp <= 57)) continue;
        else {
            return false;
        }
    }
    return true;
}

void int_sentence(string line, ostream & os) {
    line = line.substr(3);
    line.erase(0, line.find_first_not_of(" "));
    string::size_type position1;
    string::size_type position2;
    string variable_name;
    string variable_value;
    position1 = line.find_first_of("=");
    if (position1 == line.npos) {
        position2 = line.find_first_of("(");
        if (position2 == line.npos) {
            variable_name = line;
            if (variable_type.count(variable_name)) {
                os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
            }
            else {
                variable_type[variable_name] = "int";
            }
        }
        else {
            string::size_type position3;
            position3 = line.find_last_of(")");
            line = line.substr(position2 + 1, position3 - position2 - 1);
            string::size_type position4;
            vector<string> parameters;
            string parameter;
            position4 = line.find_first_of(",");
            if (position4 == line.npos) {
                parameters.push_back(line);
            }
            else {
                parameter = line.substr(0, position4);
                parameters.push_back(parameter);
                string::size_type position5;
                position5 = line.find_first_of(",", position4 + 1);
                while (position5 != line.npos) {
                    parameter = line.substr(position4 + 1, position5 - position4 - 1);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    parameters.push_back(parameter);
                    position4 = position5;
                    position5 = line.find_first_of(",", position4 + 1);
                }
                parameter = line.substr(position4 + 1);
                parameter.erase(0, parameter.find_first_not_of(" "));
                parameters.push_back(parameter);
            }
            int length = parameters.size();
            for (int i = 0; i < length; i++) {
                parameter = parameters[i];
                if (parameter.substr(0, 3) == "int") {
                    parameter = parameter.substr(3);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "int";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 6) == "string") {
                    parameter = parameter.substr(6);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "string";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 4) == "char") {
                    parameter = parameter.substr(4);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "char";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 5) == "short") {
                    parameter = parameter.substr(5);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "short";
                    variable[parameter] = true;
                }
            }
        }
    }
    else {
        variable_name = line.substr(0, position1);
        string::size_type position3;
        position3 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position3 + 1);
        if (variable_type.count(variable_name)) {
            os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
        }
        else {
            variable_type[variable_name] = "int";
            variable_value = line.substr(position1 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            if (AllisNum(variable_value)) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "int") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
}

void string_sentence(string line, ostream & os) {
    line = line.substr(6);
    line.erase(0, line.find_first_not_of(" "));
    string::size_type position1;
    string::size_type position2;
    string variable_name;
    string variable_value;
    position1 = line.find_first_of("=");
    if (position1 == line.npos) {
        position2 = line.find_first_of("(");
        if (position2 == line.npos) {
            variable_name = line;
            if (variable_type.count(variable_name)) {
                os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
            }
            else {
                variable_type[variable_name] = "string";
            }
        }
        else {
            string::size_type position3;
            position3 = line.find_last_of(")");
            line = line.substr(position2 + 1, position3 - position2 - 1);
            string::size_type position4;
            vector<string> parameters;
            string parameter;
            position4 = line.find_first_of(",");
            if (position4 == line.npos) {
                parameters.push_back(line);
            }
            else {
                parameter = line.substr(0, position4);
                parameters.push_back(parameter);
                string::size_type position5;
                position5 = line.find_first_of(",", position4 + 1);
                while (position5 != line.npos) {
                    parameter = line.substr(position4 + 1, position5 - position4 - 1);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    parameters.push_back(parameter);
                    position4 = position5;
                    position5 = line.find_first_of(",", position4 + 1);
                }
                parameter = line.substr(position4 + 1);
                parameter.erase(0, parameter.find_first_not_of(" "));
                parameters.push_back(parameter);
            }
            int length = parameters.size();
            for (int i = 0; i < length; i++) {
                parameter = parameters[i];
                if (parameter.substr(0, 3) == "int") {
                    parameter = parameter.substr(3);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "int";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 6) == "string") {
                    parameter = parameter.substr(6);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "string";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 4) == "char") {
                    parameter = parameter.substr(4);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "char";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 5) == "short") {
                    parameter = parameter.substr(5);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "short";
                    variable[parameter] = true;
                }
            }
        }
    }
    else {
        variable_name = line.substr(0, position1);
        string::size_type position3;
        position3 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position3 + 1);
        if (variable_type.count(variable_name)) {
            os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
        }
        else {
            variable_type[variable_name] = "string";
            variable_value = line.substr(position1 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            int length = variable_value.size();
            if ((variable_value[0] == '"') && (variable_value[length - 1] == '"')) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "string") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
}

void char_sentence(string line, ostream & os) {
    line = line.substr(4);
    line.erase(0, line.find_first_not_of(" "));
    string::size_type position1;
    string::size_type position2;
    string variable_name;
    string variable_value;
    position1 = line.find_first_of("=");
    if (position1 == line.npos) {
        position2 = line.find_first_of("(");
        if (position2 == line.npos) {
            variable_name = line;
            if (variable_type.count(variable_name)) {
                os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
            }
            else {
                variable_type[variable_name] = "char";
            }
        }
        else {
            string::size_type position3;
            position3 = line.find_last_of(")");
            line = line.substr(position2 + 1, position3 - position2 - 1);
            string::size_type position4;
            vector<string> parameters;
            string parameter;
            position4 = line.find_first_of(",");
            if (position4 == line.npos) {
                parameters.push_back(line);
            }
            else {
                parameter = line.substr(0, position4);
                parameters.push_back(parameter);
                string::size_type position5;
                position5 = line.find_first_of(",", position4 + 1);
                while (position5 != line.npos) {
                    parameter = line.substr(position4 + 1, position5 - position4 - 1);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    parameters.push_back(parameter);
                    position4 = position5;
                    position5 = line.find_first_of(",", position4 + 1);
                }
                parameter = line.substr(position4 + 1);
                parameter.erase(0, parameter.find_first_not_of(" "));
                parameters.push_back(parameter);
            }
            int length = parameters.size();
            for (int i = 0; i < length; i++) {
                parameter = parameters[i];
                if (parameter.substr(0, 3) == "int") {
                    parameter = parameter.substr(3);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "int";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 6) == "string") {
                    parameter = parameter.substr(6);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "string";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 4) == "char") {
                    parameter = parameter.substr(4);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "char";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 5) == "short") {
                    parameter = parameter.substr(5);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "short";
                    variable[parameter] = true;
                }
            }
        }
    }
    else {
        variable_name = line.substr(0, position1);
        string::size_type position3;
        position3 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position3 + 1);
        if (variable_type.count(variable_name)) {
            os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
        }
        else {
            variable_type[variable_name] = "char";
            variable_value = line.substr(position1 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            int length = variable_value.size();
            if ((length == 3) && (!AllisNum(variable_value)) && (variable_value[0] != '"') && (variable_value[length - 1] != '"')) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "char") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
}

void short_sentence(string line, ostream & os) {
    line = line.substr(5);
    line.erase(0, line.find_first_not_of(" "));
    string::size_type position1;
    string::size_type position2;
    string variable_name;
    string variable_value;
    position1 = line.find_first_of("=");
    if (position1 == line.npos) {
        position2 = line.find_first_of("(");
        if (position2 == line.npos) {
            variable_name = line;
            if (variable_type.count(variable_name)) {
                os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
            }
            else {
                variable_type[variable_name] = "short";
            }
        }
        else {
            string::size_type position3;
            position3 = line.find_last_of(")");
            line = line.substr(position2 + 1, position3 - position2 - 1);
            string::size_type position4;
            vector<string> parameters;
            string parameter;
            position4 = line.find_first_of(",");
            if (position4 == line.npos) {
                parameters.push_back(line);
            }
            else {
                parameter = line.substr(0, position4);
                parameters.push_back(parameter);
                string::size_type position5;
                position5 = line.find_first_of(",", position4 + 1);
                while (position5 != line.npos) {
                    parameter = line.substr(position4 + 1, position5 - position4 - 1);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    parameters.push_back(parameter);
                    position4 = position5;
                    position5 = line.find_first_of(",", position4 + 1);
                }
                parameter = line.substr(position4 + 1);
                parameter.erase(0, parameter.find_first_not_of(" "));
                parameters.push_back(parameter);
            }
            int length = parameters.size();
            for (int i = 0; i < length; i++) {
                parameter = parameters[i];
                if (parameter.substr(0, 3) == "int") {
                    parameter = parameter.substr(3);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "int";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 6) == "string") {
                    parameter = parameter.substr(6);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "string";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 4) == "char") {
                    parameter = parameter.substr(4);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "char";
                    variable[parameter] = true;
                }
                else if (parameter.substr(0, 5) == "short") {
                    parameter = parameter.substr(5);
                    parameter.erase(0, parameter.find_first_not_of(" "));
                    variable_type[parameter] = "short";
                    variable[parameter] = true;
                }
            }
        }
    }
    else {
        variable_name = line.substr(0, position1);
        string::size_type position3;
        position3 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position3 + 1);
        if (variable_type.count(variable_name)) {
            os << line_number << ", " << "redefinition of variable: " << variable_name << endl;
        }
        else {
            variable_type[variable_name] = "short";
            variable_value = line.substr(position1 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            if (AllisNum(variable_value)) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "short") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
}

void cout_sentence(string line, ostream & os) {
    line = line.substr(4);
    string::size_type position1;
    string variable_name;
    position1 = line.find_first_of("<<");
    while (true) {
        string::size_type position2;
        position2 = position1;
        if (position2 != line.npos) {
            position1 = line.find_first_of("<<", position2 + 2);
            if (position1 == line.npos) {
                variable_name = line.substr(position2 + 2);
                variable_name.erase(0, variable_name.find_first_not_of(" "));
                if (!variable_type.count(variable_name)) {
                    os << line_number << ", " << "undefined variable: " << variable_name << endl;
                }
                else if (!variable.count(variable_name)) {
                    os << line_number << ", " << "no specified value for " << variable_name << endl;
                }
            }
            else {
                variable_name = line.substr(position2 + 2, position1 - position2 - 2);
                variable_name.erase(0, variable_name.find_first_not_of(" "));
                string::size_type position3;
                position3 = variable_name.find_last_not_of(" ");
                variable_name = variable_name.substr(0, position3 + 1);
                if (!variable_type.count(variable_name)) {
                    os << line_number << ", " << "undefined variable: " << variable_name << endl;
                }
                else if (!variable.count(variable_name)) {
                    os << line_number << ", " << "no specified value for " << variable_name << endl;
                }
            }
        }
        else break;
    }
}

void if_sentence(string line, ostream & os) {
    line = line.substr(2);
    string::size_type position1;
    string::size_type position2;
    position1 = line.find_first_of("(");
    position2 = line.find_first_of(")");
    line = line.substr(position1 + 1, position2 - position1 - 1);
    string variable_name;
    string variable_value;
    string::size_type position3;
    if (line.find("==") != line.npos) {
        position3 = line.find("==");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find(">=") != line.npos) {
        position3 = line.find(">=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find(">") != line.npos) {
        position3 = line.find(">");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 1);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("<=") != line.npos) {
        position3 = line.find("<=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("<") != line.npos) {
        position3 = line.find("<");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 1);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("!=") != line.npos) {
        position3 = line.find("!=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
}

void else_sentence(string line, ostream & os) {
    line = line.substr(4);
    line.erase(0, line.find_first_not_of(" "));
    if (line.substr(0, 2) == "if") if_sentence(line, os);
}

void while_sentence(string line, ostream & os) {
    line = line.substr(5);
    string::size_type position1;
    string::size_type position2;
    position1 = line.find_first_of("(");
    position2 = line.find_first_of(")");
    line = line.substr(position1 + 1, position2 - position1 - 1);
    string variable_name;
    string variable_value;
    string::size_type position3;
    if (line.find("==") != line.npos) {
        position3 = line.find("==");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find(">=") != line.npos) {
        position3 = line.find(">=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find(">") != line.npos) {
        position3 = line.find(">");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 1);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("<=") != line.npos) {
        position3 = line.find("<=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("<") != line.npos) {
        position3 = line.find("<");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 1);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("!=") != line.npos) {
        position3 = line.find("!=");
        variable_name = line.substr(0, position3);
        string::size_type position4;
        position4 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position4 + 1);
        variable_value = line.substr(position3 + 2);
        variable_value = variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
}

void for_sentence(string line, ostream & os) {
    line = line.substr(3);
    string::size_type position1;
    string::size_type position2;
    position1 = line.find_first_of("(");
    position2 = line.find_first_of(")");
    line = line.substr(position1 + 1, position2 - position1 - 1);
    string::size_type position3;
    string::size_type position4;
    position3 = line.find_first_of(";");
    position4 = line.find_first_of(";", position3 + 1);
    string statement1;
    string statement2;
    statement1 = line.substr(0, position3);
    statement2 = line.substr(position3 + 1, position4 - position3 - 1);
    statement2 = "while" + statement2;
    while_sentence(statement2, os);
    string variable_name;
    string variable_value;
    if (statement1.substr(0, 3) == "int") {
        statement1 = statement1.substr(3);
        statement1.erase(0, statement1.find_first_not_of(" "));
        if (statement1.find_first_of("=") == statement1.npos) {
            variable_name = statement1;
            os << line_number << ", " << "no specified value for " << variable_name << endl;
        }
        else {
            string::size_type position5;
            position5 = statement1.find_first_of("=");
            variable_name = statement1.substr(0, position5);
            string::size_type position6;
            position6 = variable_name.find_last_not_of(" ");
            variable_name = variable_name.substr(0, position6 + 1);
            variable_value = statement1.substr(position5 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            if (AllisNum(variable_value)) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "int") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
    else {
        string::size_type position5;
        position5 = statement1.find_first_of("=");
        if (position5 == statement1.npos) {
            variable_name = statement1;
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        else {
            variable_name = statement1.substr(0, position5);
            string::size_type position6;
            position6 = variable_name.find_last_not_of(" ");
            variable_name = variable_name.substr(0, position6 + 1);
            variable_value = statement1.substr(position5 + 1);
            variable_value.erase(0, variable_value.find_first_not_of(" "));
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
            }
            if (AllisNum(variable_value)) {
                variable[variable_name] = true;
            }
            else {
                if (!variable_type.count(variable_value)) {
                    os << line_number << ", " << "undefined variable: " << variable_value << endl;
                }
                else {
                    if (variable_type.find(variable_value)->second != "int") {
                        os << line_number << ", " << "wrong type of " << variable_value << endl;
                    }
                    else {
                        if (!variable.count(variable_value)) {
                            os << line_number << ", " << "no specified value for " << variable_value << endl;
                        }
                        else {
                            variable[variable_name] = true;
                        }
                    }
                }
            }
        }
    }
}

void operation_sentence(string line, ostream & os) {
    if (line.find("++") != line.npos) {
        string::size_type position1;
        position1 = line.find("++");
        string variable_name;
        if (position1 == 0) {
            variable_name = line.substr(2);
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        else {
            variable_name = line.substr(0, position1);
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
    }
    else if (line.find("--") != line.npos) {
        string::size_type position1;
        position1 = line.find("--");
        string variable_name;
        if (position1 == 0) {
            variable_name = line.substr(2);
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        else {
            variable_name = line.substr(0, position1);
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
    }
    else if (line.find("+=") != line.npos) {
        string::size_type position1;
        position1 = line.find("+=");
        string variable_name;
        string variable_value;
        variable_name = line.substr(0, position1);
        string::size_type position2;
        position2 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position2 + 1);
        variable_value = line.substr(position1 + 2);
        variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("-=") != line.npos) {
        string::size_type position1;
        position1 = line.find("-=");
        string variable_name;
        string variable_value;
        variable_name = line.substr(0, position1);
        string::size_type position2;
        position2 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position2 + 1);
        variable_value = line.substr(position1 + 2);
        variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("*=") != line.npos) {
        string::size_type position1;
        position1 = line.find("*=");
        string variable_name;
        string variable_value;
        variable_name = line.substr(0, position1);
        string::size_type position2;
        position2 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position2 + 1);
        variable_value = line.substr(position1 + 2);
        variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else if (line.find("/=") != line.npos) {
        string::size_type position1;
        position1 = line.find("/=");
        string variable_name;
        string variable_value;
        variable_name = line.substr(0, position1);
        string::size_type position2;
        position2 = variable_name.find_last_not_of(" ");
        variable_name = variable_name.substr(0, position2 + 1);
        variable_value = line.substr(position1 + 2);
        variable_value.erase(0, variable_value.find_first_not_of(" "));
        if (!AllisNum(variable_name)) {
            if (!variable_type.count(variable_name)) {
                os << line_number << ", " << "undefined variable: " << variable_name << endl;
            }
            else {
                if (variable_type.find(variable_name)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_name << endl;
                }
                else {
                    if (!variable.count(variable_name)) {
                        os << line_number << ", " << "no specified value for " << variable_name << endl;
                    }
                }
            }
        }
        if (!AllisNum(variable_value)) {
            if (!variable_type.count(variable_value)) {
                os << line_number << ", " << "undefined variable: " << variable_value << endl;
            }
            else {
                if (variable_type.find(variable_value)->second != "int") {
                    os << line_number << ", " << "wrong type of " << variable_value << endl;
                }
                else {
                    if (!variable.count(variable_value)) {
                        os << line_number << ", " << "no specified value for " << variable_value << endl;
                    }
                }
            }
        }
    }
    else {
        int length1 = line.size();
        vector<int> operation_index;
        for (int i = 0; i < length1; i++) {
            if (operation.count(line[i])) {
                operation_index.push_back(i);
            }
        }
        int length2 = operation_index.size();
        if (length2 > 1) {
            string variable_name_1;
            variable_name_1 = line.substr(0, operation_index[0]);
            string::size_type position1;
            position1 = variable_name_1.find_last_not_of(" ");
            variable_name_1 = variable_name_1.substr(0, position1 + 1);
            if (!AllisNum(variable_name_1)) {
                if (!variable_type.count(variable_name_1)) {
                    os << line_number << ", " << "undefined variable: " << variable_name_1 << endl;
                }
                else {
                    if (variable_type.find(variable_name_1)->second != "int") {
                        os << line_number << ", " << "wrong type of " << variable_name_1 << endl;
                    }
                }
            }
            variable[variable_name_1] = true;
            for (int i = 0; i < length2; i++) {
                string variable_name;
                if (i == length2 - 1) {
                    variable_name = line.substr(operation_index[length2 - 1] + 1);
                    variable_name.erase(0, variable_name.find_first_not_of(" "));
                    if (!AllisNum(variable_name)) {
                        if (!variable_type.count(variable_name)) {
                            os << line_number << ", " << "undefined variable: " << variable_name << endl;
                            variable.erase(variable_name_1);
                        }
                        else {
                            if (variable_type.find(variable_name)->second != "int") {
                                os << line_number << ", " << "wrong type of " << variable_name << endl;
                                variable.erase(variable_name_1);
                            }
                            else {
                                if (!variable.count(variable_name)) {
                                    os << line_number << ", " << "no specified value for " << variable_name << endl;
                                    variable.erase(variable_name_1);
                                }
                            }
                        }
                    }
                }
                else {
                    variable_name = line.substr(operation_index[i] + 1, operation_index[i + 1] - operation_index[i] - 1);
                    string::size_type position2;
                    position2 = variable_name.find_last_not_of(" ");
                    variable_name = variable_name.substr(0, position2 + 1);
                    variable_name.erase(0, variable_name.find_first_not_of(" "));
                    if (!AllisNum(variable_name)) {
                        if (!variable_type.count(variable_name)) {
                            os << line_number << ", " << "undefined variable: " << variable_name << endl;
                            variable.erase(variable_name_1);
                        }
                        else {
                            if (variable_type.find(variable_name)->second != "int") {
                                os << line_number << ", " << "wrong type of " << variable_name << endl;
                                variable.erase(variable_name_1);
                            }
                            else {
                                if (!variable.count(variable_name)) {
                                    os << line_number << ", " << "no specified value for " << variable_name << endl;
                                    variable.erase(variable_name_1);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (length2 == 1) {
            string variable_name_1;
            variable_name_1 = line.substr(0, operation_index[0]);
            string::size_type position1;
            position1 = variable_name_1.find_last_not_of(" ");
            variable_name_1 = variable_name_1.substr(0, position1 + 1);
            if (!variable_type.count(variable_name_1)) {
                os << line_number << ", " << "undefined variable: " << variable_name_1 << endl;
            }
            else {
                if (variable_type.find(variable_name_1)->second == "int") {
                    string variable_value;
                    variable_value = line.substr(operation_index[0] + 1);
                    variable_value.erase(0, variable_value.find_first_not_of(" "));
                    if (AllisNum(variable_value)) {
                        variable[variable_name_1] = true;
                    }
                    else if (variable_value.find("(") == variable_value.npos) {
                        if (!variable_type.count(variable_value)) {
                            os << line_number << ", " << "undefined variable: " << variable_value << endl;
                        }
                        else {
                            if (variable_type.find(variable_value)->second != "int") {
                                os << line_number << ", " << "wrong type of " << variable_value << endl;
                            }
                            else {
                                if (!variable.count(variable_value)) {
                                    os << line_number << ", " << "no specified value for " << variable_value << endl;
                                }
                                else {
                                    variable[variable_name_1] = true;
                                }
                            }
                        }
                    }
                }
                else if (variable_type.find(variable_name_1)->second == "string") {
                    string variable_value;
                    variable_value = line.substr(operation_index[0] + 1);
                    variable_value.erase(0, variable_value.find_first_not_of(" "));
                    int length = variable_value.size();
                    if ((variable_value[0] == '"') && (variable_value[length - 1] == '"')) {
                        variable[variable_name_1] = true;
                    }
                    else if (variable_value.find("(") == variable_value.npos) {
                        if (!variable_type.count(variable_value)) {
                            os << line_number << ", " << "undefined variable: " << variable_value << endl;
                        }
                        else {
                            if (variable_type.find(variable_value)->second != "string") {
                                os << line_number << ", " << "wrong type of " << variable_value << endl;
                            }
                            else {
                                if (!variable.count(variable_value)) {
                                    os << line_number << ", " << "no specified value for " << variable_value << endl;
                                }
                                else {
                                    variable[variable_name_1] = true;
                                }
                            }
                        }
                    }
                }
                else if (variable_type.find(variable_name_1)->second == "char") {
                    string variable_value;
                    variable_value = line.substr(operation_index[0] + 1);
                    variable_value.erase(0, variable_value.find_first_not_of(" "));
                    int length = variable_value.size();
                    if ((length == 3) && (!AllisNum(variable_value)) && (variable_value[0] != '"') && (variable_value[length - 1] != '"')) {
                        variable[variable_name_1] = true;
                    }
                    else if (variable_value.find("(") == variable_value.npos) {
                        if (!variable_type.count(variable_value)) {
                            os << line_number << ", " << "undefined variable: " << variable_value << endl;
                        }
                        else {
                            if (variable_type.find(variable_value)->second != "char") {
                                os << line_number << ", " << "wrong type of " << variable_value << endl;
                            }
                            else {
                                if (!variable.count(variable_value)) {
                                    os << line_number << ", " << "no specified value for " << variable_value << endl;
                                }
                                else {
                                    variable[variable_name_1] = true;
                                }
                            }
                        }
                    }
                }
                else if (variable_type.find(variable_name_1)->second == "short") {
                    string variable_value;
                    variable_value = line.substr(operation_index[0] + 1);
                    variable_value.erase(0, variable_value.find_first_not_of(" "));
                    if (AllisNum(variable_value)) {
                        variable[variable_name_1] = true;
                    }
                    else if (variable_value.find("(") == variable_value.npos) {
                        if (!variable_type.count(variable_value)) {
                            os << line_number << ", " << "undefined variable: " << variable_value << endl;
                        }
                        else {
                            if (variable_type.find(variable_value)->second != "short") {
                                os << line_number << ", " << "wrong type of " << variable_value << endl;
                            }
                            else {
                                if (!variable.count(variable_value)) {
                                    os << line_number << ", " << "no specified value for " << variable_value << endl;
                                }
                                else {
                                    variable[variable_name_1] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void variable_check(string f_path, string of_path) {
    variable_type.clear();
    variable.clear();
    line_number = 0;
    fstream fp;
    fp.open(f_path, ios::in);
    ofstream ofp;
    ofp.open(of_path, ios::out);
    string line;
    while (getline(fp, line)) {
        line_number += 1;
        line.erase(0, line.find_first_not_of(" "));
        int length1 = line.size();
        line = line.substr(0, length1 - 1);
        int length2 = line.size();
        if (line[length2 - 1] == ';') {
            line = line.substr(0, length2 - 1);
        }
        if (line == "using namespace std") continue;
        else if (line == "") continue;
        else if (line == "{") continue;
        else if (line == "}") continue;
        else if (line == "break") continue;
        else if (line == "continue") continue;
        else if (line[0] == '#') continue;
        else if (line.substr(0, 3) == "cin") continue;
        else if (line.substr(0, 6) == "return") continue;
        else if (line.substr(0, 2) == "/*") continue;
        else if (line.substr(0, 2) == "*/") continue;
        else if (line[0] == '*') continue;
        else if (line.find("//") != line.npos) {
            string::size_type position1;
            position1 = line.find("//");
            if (position1 == 0) continue;
            else {
                line = line.substr(0, position1);
                string::size_type position2;
                position2 = line.find_last_not_of(" ");
                line = line.substr(0, position2 + 1);
                length2 = line.size();
                if (line[length2 - 1] == ';') {
                    line = line.substr(0, length2 - 1);
                }
            }
        }

        if (line.substr(0, 3) == "int") int_sentence(line, ofp);
        else if (line.substr(0, 6) == "string") string_sentence(line, ofp);
        else if (line.substr(0, 4) == "char") char_sentence(line, ofp);
        else if (line.substr(0, 5) == "short") short_sentence(line, ofp);
        else if (line.substr(0, 4) == "cout") cout_sentence(line, ofp);
        else if (line.substr(0, 2) == "if") if_sentence(line, ofp);
        else if (line.substr(0, 4) == "else") else_sentence(line, ofp);
        else if (line.substr(0, 5) == "while") while_sentence(line, ofp);
        else if (line.substr(0, 3) == "for") for_sentence(line, ofp);
        else operation_sentence(line, ofp);
    }
    fp.close();
    ofp.close();
}
