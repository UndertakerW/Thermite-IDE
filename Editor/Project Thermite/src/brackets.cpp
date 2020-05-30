#include <iostream>
#include <string>
#include <stack>
#include <set>
#include <fstream>
#include "brackets.h"

using namespace std;

int LINENUMBER = 0;

stack<char> LeftBracket;
stack<int> BracketNumber;

const set<char> bracket = {'(', ')', '{', '}'};

bool brackets_check(string f_path, string of_path) {
    LINENUMBER = 0;
    while (!LeftBracket.empty()) {
        LeftBracket.pop();
    }
    while (!BracketNumber.empty()) {
        BracketNumber.pop();
    }
    fstream fp;
    fp.open(f_path, ios::in);
    ofstream ofp;
    ofp.open(of_path, ios::out);
    string line;
    while (getline(fp, line)) {
        LINENUMBER += 1;
        line.erase(0, line.find_first_not_of(" "));
        string::size_type position1;
        position1 = line.find_last_not_of(" ");
        line = line.substr(0, position1 + 1);
        if (line.substr(0, 2) == "/*") continue;
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

        int length = line.size();
        for (int i = 0; i < length; i++) {
            if (bracket.count(line[i])) {
                if (line[i] == '(') {
                    LeftBracket.push('(');
                    BracketNumber.push(LINENUMBER);
                }
                else if (line[i] == '{') {
                    LeftBracket.push('{');
                    BracketNumber.push(LINENUMBER);
                }
                else if (line[i] == ')') {
                    if (LeftBracket.empty()) {
                        ofp << LINENUMBER << ", " << "no matching '(' for ')'" << endl;
                        return false;
                    }
                    else if (LeftBracket.top() != '(') {
                        int number = BracketNumber.top();
                        ofp << number << ", " << "no matching ')' for '('" << endl;
                        return false;
                    }
                    else {
                        LeftBracket.pop();
                        BracketNumber.pop();
                    }
                }
                else if (line[i] == '}') {
                    if (LeftBracket.empty()) {
                        ofp << LINENUMBER << ", " << "no matching '{' for '}'" << endl;
                        return false;
                    }
                    else if (LeftBracket.top() != '{') {
                        int number = BracketNumber.top();
                        ofp << number << ", " << "no matching '}' for '{'" << endl;
                        return false;
                    }
                    else {
                        LeftBracket.pop();
                        BracketNumber.pop();
                    }
                }
            }
        }
    }
    if (!LeftBracket.empty()) {
        int number = BracketNumber.top();
        char type = LeftBracket.top();
        if (type == '(') {
            ofp << number << ", " << "no matching ')' for '('" << endl;
            return false;
        }
        else {
            ofp << number << ", " << "no matching '}' for '{'" << endl;
            return false;
        }
    }
    fp.close();
    ofp.close();
    return true;
}

