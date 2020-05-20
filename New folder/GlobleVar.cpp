#define _CRT_SECURE_NO_DEPRECATE

#include"iostream"
#include"string"
#include"fstream"
#include"queue"
#include"math.h"
#include"string.h"
#include"vector"
#include"stack"
#include"pch.h"

#include "GlobleVar.h"
#include"Grammer.h"
#include"TokenDetect.h"

using namespace std;

string Reserved[24];      //保留字
char Character[19];          //特殊符
queue<string>  IDTable;      //标识符
queue<int>     NumTable;     //数字表
queue<Token>   TokenTable;      //Token表
char   CurrentChar = '\0';   //当前字符
string CurrentString = "";   //当前单词
int    CurrentLine = 1;      //当前行数

Token _TK;                   //当前token

ifstream fin;    // 输入流

Tree* GrammerTree;        //语法树
int ERRCount = 0;
int ERRFlag = 0;

string GrammerTreeString = "";
string Errorinfo = "";

void ResetAll()
{
	CurrentChar = '\0';
	CurrentString = "";
	CurrentLine = 1;
	ERRCount = 0;
	ERRFlag = 0;
	GrammerTreeString = "";
	Errorinfo = "";

	IDTable = queue<string>();
	NumTable = queue<int>();
	TokenTable = queue<Token>();      //Token表

}
