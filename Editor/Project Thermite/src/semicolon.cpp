#include <iostream>
#include <string>
#include <fstream>
#include "semicolon.h"

using namespace std;

int LineNumber = 0;

bool semicolon_check(string f_path, string of_path) {
    LineNumber = 0;
    fstream fp;
    fp.open(f_path, ios::in);
    ofstream ofp;
    ofp.open(of_path, ios::out);
    string line;
    while (getline(fp, line)) {
        LineNumber += 1;
        line.erase(0, line.find_first_not_of(" "));
        string::size_type position1;
        position1 = line.find_last_not_of(" ");
        line = line.substr(0, position1 + 1);
        if (line[0] == '#') continue;
        else if (line == "") continue;
        else if (line == "}") continue;
        else if (line == "{") continue;
        else if (line.substr(0, 2) == "if") continue;
        else if (line.substr(0, 4) == "else") continue;
        else if (line.substr(0, 5) == "while") continue;
        else if (line.substr(0, 2) == "/*") continue;
        else if (line[0] == '*') continue;
        else if (line.find("//") != line.npos) {
            string::size_type position2;
            position2 = line.find("//");
            if (position2 == 0) continue;
            else {
                line = line.substr(0, position2);
                string::size_type position3;
                position3 = line.find_last_not_of(" ");
                line = line.substr(0, position3 + 1);
            }
        }
        else if (line.substr(0, 3) == "int") {
            string::size_type position2;
            position2 = line.find("=");
            if (position2 == line.npos) {
                string::size_type position3;
                position3 = line.find("(");
                if (position3 != line.npos) {
                    continue;
                }
            }
        }
        else if (line.substr(0, 6) == "string") {
            string::size_type position2;
            position2 = line.find("=");
            if (position2 == line.npos) {
                string::size_type position3;
                position3 = line.find("(");
                if (position3 != line.npos) {
                    continue;
                }
            }
        }
        else if (line.substr(0, 4) == "char") {
            string::size_type position2;
            position2 = line.find("=");
            if (position2 == line.npos) {
                string::size_type position3;
                position3 = line.find("(");
                if (position3 != line.npos) {
                    continue;
                }
            }
        }
        else if (line.substr(0, 5) == "short") {
            string::size_type position2;
            position2 = line.find("=");
            if (position2 == line.npos) {
                string::size_type position3;
                position3 = line.find("(");
                if (position3 != line.npos) {
                    continue;
                }
            }
        }

        int length = line.size();
        if (line.substr(0, 3) == "for") {
            string::size_type position2;
            position2 = line.find(";");
            if (position2 == line.npos) {
                ofp << LineNumber << ", " << "incomplete statement (expected a semicolon)" << endl;
                return false;
            }
            else {
                string::size_type position3;
                position3 = line.find(";", position2 + 1);
                if (position3 == line.npos) {
                    ofp << LineNumber << ", " << "incomplete statement (expected a semicolon)" << endl;
                    return false;
                }
            }
        }
        else if (line[length - 1] != ';') {
            ofp << LineNumber << ", " << "incomplete statement (expected a semicolon)" << endl;
            return false;
        }
    }
    fp.close();
    ofp.close();
    return true;
}
