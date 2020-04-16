#ifndef REGISTERS_H
#define REGISTERS_H

#include <string>

//用来获取寄存器名字，返回string， 就像"$t0","$s2"这样的
//注意，要是没有寄存器用了的话，返回的 string 是 "full"
std::string getReg(std:: string var);

//用来增加变量的local等级，要是进入一个更local里的大括号里面
//就直接调用这个函数
void uplevel();

//用来减少变量的local等级，退出一个local的大括号时候就调用一次
void delevel();

#endif // REGISTERS_H
