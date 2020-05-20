#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable: 4996)

#include"iostream"
#include"string"
#include"fstream"
#include"queue"
#include"math.h"
#include"string.h"
#include"vector"
#include"stack"
#include"pch.h"

#include "TokenDetect.h"
#include "GlobleVar.h"

using namespace std;

extern queue<Token>   TokenTable;
extern Token _TK;
extern char   CurrentChar;   //存放当前读入的字符
extern string Reserved[24];      //保留字
extern queue<int>     NumTable;     //数字表
extern queue<string>  IDTable;      //标识符表
extern ifstream fin;    // 文件输入流
extern queue<Token>   TokenTable;
extern string CurrentString;
extern int    CurrentLine;
extern int ERRCount;
extern char Character[19];          //特殊符号
extern string Errorinfo;
void OutputToken() {
	if (!CheckStr()) {
		queue<Token> TK;
		cout << "Token as follow:" << endl;
		cout << "Type Word" << endl;
		for (TK = TokenTable; TK.size() > 0; TK.pop()) {
			cout << TK.front().Type << "  " << TK.front().Item << endl;
		}
	}
}
string OutputToken_s() {
	string tokenresult;
	if (!CheckStr()) {
		queue<Token> TK;
		for (TK = TokenTable; TK.size() > 0; TK.pop()) {
			tokenresult += to_string(TK.front().Type);
			tokenresult += " ";
			tokenresult += TK.front().Item;
			tokenresult += "\r\n";
		}
	}
	else {
		tokenresult = "Token Error !";
	}
	return tokenresult;
}
void Judge()
{
	if ((CurrentChar >= 'A' && CurrentChar <= 'Z') ||     //判断是否为字母开头的单词
		(CurrentChar >= 'a' && CurrentChar <= 'z')) {
		Judge1();
	}
	else
		if (CurrentChar >= '0' && CurrentChar <= '9') {     //判断是否为数字
			Judge2();
		}
		else {                                             //判断是否为特殊字符或其他字符
			Judge3();
		}

}

void Judge1()                                      // 判断是否为保留字或标识符
{
	CurrentString.append(1, CurrentChar);
	while (((CurrentChar >= 'A' && CurrentChar <= 'Z') ||
		(CurrentChar >= 'a' && CurrentChar <= 'z') ||
		(CurrentChar >= '0' && CurrentChar <= '9')) &&
		!fin.eof())
	{
		fin >> noskipws >> CurrentChar;
		if ((CurrentChar >= 'A' && CurrentChar <= 'Z') ||
			(CurrentChar >= 'a' && CurrentChar <= 'z') ||
			(CurrentChar >= '0' && CurrentChar <= '9'))
			CurrentString.append(1, CurrentChar);
	}//while

	if (!fin.eof()) fin.seekg(-1, ios::cur); //读到了不是字母或数字的字符，退回源程序文件指针一位

	if (IsReserved()) {
		_TK.Type = 1;                    // 类型1为保留字
		_TK.Item = CurrentString;
		_TK.Line = CurrentLine;
		TokenTable.push(_TK);
	}
	else {
		_TK.Type = 2;                    // 类型2为标识符
		_TK.Item = CurrentString;
		_TK.Line = CurrentLine;
		TokenTable.push(_TK);               // 放入Token中
		IDTable.push(CurrentString);     // 将标识符放入IDTable中
	}//if
}

void Judge2()
{

	CurrentString.append(1, CurrentChar);
	while ((CurrentChar >= '0' && CurrentChar <= '9') &&
		!fin.eof())
	{
		fin >> noskipws >> CurrentChar;
		if (CurrentChar >= '0' && CurrentChar <= '9')
			CurrentString.append(1, CurrentChar);
	}//while

	if (!fin.eof()) fin.seekg(-1, ios::cur);//读到了不是数字的字符，退回源程序文件指针一位

	int num = 0;
	for (int i = 0; i < CurrentString.length(); i++)
		num += ((int)CurrentString[CurrentString.length() - i - 1] - 48) * pow(10, i);

	NumTable.push(num);
	_TK.Type = 3;                    // 类型3为无符号整数
	_TK.Item = CurrentString;
	_TK.Line = CurrentLine;
	TokenTable.push(_TK);
}

void Judge3()
{
	if (!IsChar()) {

		CurrentString.append(1, CurrentChar);
		_TK.Type = 9;                               // 类型9为错误类型
		_TK.Item = CurrentString;
		_TK.Line = CurrentLine;
		TokenTable.push(_TK);
		return;
	}
	if (CurrentChar == ' ' || CurrentChar == '\n' || CurrentChar == '\r') return;  //不需要将空格回车换行录入Token中
	if (CurrentChar == ':' && !fin.eof())          //如果是冒号还要判断是否是冒号等于
	{
		CurrentString.append(1, CurrentChar);
		fin >> noskipws >> CurrentChar;
		if (CurrentChar == '=')
		{
			CurrentString.append(1, CurrentChar);
			_TK.Type = 4;                    // 类型4为特殊符号
			_TK.Item = CurrentString;
			_TK.Line = CurrentLine;
			TokenTable.push(_TK);
			return;
		}
	}

	CurrentString.append(1, CurrentChar);
	_TK.Type = 4;                    // 类型4为特殊符号
	_TK.Item = CurrentString;
	_TK.Line = CurrentLine;
	TokenTable.push(_TK);
}

bool IsReserved()
{
	for (int i = 0; i < 24; i++)
		if (!stricmp(CurrentString.c_str(), Reserved[i].c_str())) // 判断保留字
			return true;
	return false;
}
bool IsChar()
{
	for (int i = 0; i < 19; i++)
		if (CurrentChar == Character[i]) return true;              //判断是否为特殊字符
	return false;
}
bool IsID() {
	queue<string> Q = IDTable;
	for (; Q.size() > 0; Q.pop())
		if (CurrentString == Q.front()) return true;
	return false;
}
bool IsNumber() {
	int num = 0;
	for (int i = 0; i < CurrentString.length(); i++)
		num += ((int)CurrentString[CurrentString.length() - i - 1] - 48) * pow(10, i);
	queue<int> Q = NumTable;
	for (; Q.size() > 0; Q.pop())
		if (num == Q.front()) return true;
	return false;
}
void Initiation() {     //设置保留字、特殊符号
	Reserved[0] = "program";
	Reserved[1] = "id";
	Reserved[2] = "type";
	Reserved[3] = "integer";
	Reserved[4] = "char";
	Reserved[5] = "array";
	Reserved[6] = "of";
	Reserved[7] = "intc";
	Reserved[8] = "record";
	Reserved[9] = "begin";
	Reserved[10] = "end";
	Reserved[11] = "var";
	Reserved[12] = "procedure";
	Reserved[13] = "if";
	Reserved[14] = "then";
	Reserved[15] = "else";
	Reserved[16] = "fi";
	Reserved[17] = "while";
	Reserved[18] = "do";
	Reserved[19] = "endwh";
	Reserved[20] = "read";
	Reserved[21] = "write";
	Reserved[22] = "return";
	Reserved[23] = "EOF";

	Character[0] = '+';
	Character[1] = '-';
	Character[2] = '*';
	Character[3] = '/';
	Character[4] = '<';
	Character[5] = '=';
	Character[6] = '(';
	Character[7] = ')';
	Character[8] = '[';
	Character[9] = ']';
	Character[10] = '{';
	Character[11] = '}';
	Character[12] = '.';
	Character[13] = ';';
	Character[14] = ':';
	Character[15] = '\'';
	Character[16] = ' ';
	Character[17] = '\n';
	Character[18] = ',';

	Errorinfo = "";
	ERRCount = 0;
}

void getToken()
{
	while (fin.peek() != EOF)
	{
		CurrentString = "";
		fin >> noskipws >> CurrentChar;
		if (CurrentChar == '\n') CurrentLine++; //换行 行数+1
		Judge();
	}
}
void getToken_s(string s) {
	int length = s.length();
	for (int i = 0; i < length; i++ ) {
		CurrentString = "";
		if (s[i] == ' '||s[i] == '\n') {
			continue;
		}
		CurrentChar = s[i];
		if (CurrentChar == '\r') {
			CurrentLine++; //换行 行数+1
			CurrentChar = '\n';
		}
		Judge();
	}
}
bool CheckStr() {
	queue<Token> TK = TokenTable;
	int flag = 0;
	string temp;
	for (TK = TokenTable; TK.size() > 0; TK.pop()) {
		if (TK.front().Type == 9) {
			temp = TK.front().Item;
			cout << "Error:line " << TK.front().Line << "  \"" << TK.front().Item << "\" is not a valid character." << endl;
			Errorinfo += "Error:line ";
			Errorinfo += to_string(TK.front().Line);
			Errorinfo += "  \"";
			Errorinfo += TK.front().Item;
			Errorinfo += "\" is not a valid character.";
			Errorinfo += "\r\n";
			flag = 1;
		}
	}
	if (flag) return true;
	return false;
}