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

string Reserved[24];      //������
char Character[19];          //�����
queue<string>  IDTable;      //��ʶ��
queue<int>     NumTable;     //���ֱ�
queue<Token>   TokenTable;      //Token��
char   CurrentChar = '\0';   //��ǰ�ַ�
string CurrentString = "";   //��ǰ����
int    CurrentLine = 1;      //��ǰ����

Token _TK;                   //��ǰtoken

ifstream fin;    // ������

Tree* GrammerTree;        //�﷨��
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
	TokenTable = queue<Token>();      //Token��

}
