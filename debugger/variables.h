#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

using namespace std;

void variable_check(string f_path, string of_path);

void operation_sentence(string line, ostream & os);

void for_sentence(string line, ostream & os);

void while_sentence(string line, ostream & os);

void else_sentence(string line, ostream & os);

void if_sentence(string line, ostream & os);

void cout_sentence(string line, ostream & os);

void short_sentence(string line, ostream & os);

void char_sentence(string line, ostream & os);

void string_sentence(string line, ostream & os);

void int_sentence(string line, ostream & os);

bool AllisNum(string str);

#endif // VARIABLE_H
