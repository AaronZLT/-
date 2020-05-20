#pragma once
#include"iostream"
#include"string"
#include"fstream"
#include"queue"
#include"math.h"
#include"string.h"
#include"vector"
#include"stack"

using namespace std;

class Token
{
public:
	int Type;     // 1为保留字，2为标识符，3为无符号整数，4为特殊符号,9为错误类型
	int Line;     //出现错误需要保留行数，未出现错误则让其保持为0
	string Item;
	Token() { Line = 0; }
};

void Judge();
void Judge1();
void Judge2();
void Judge3();
void Initiation(); //初始化函数
bool IsReserved(); //判断是否为保留字
bool IsChar();     //判断是否为特殊字符
bool IsID();       //判断是否为标识符
bool IsNumber();   //判断是否为数字
void getToken();
void getToken_s(string);
bool CheckStr();   //检查Token列表中是否有错误的单词
void OutputToken();
string OutputToken_s();

