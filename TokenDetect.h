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
	int Type;     // 1Ϊ�����֣�2Ϊ��ʶ����3Ϊ�޷���������4Ϊ�������,9Ϊ��������
	int Line;     //���ִ�����Ҫ����������δ���ִ��������䱣��Ϊ0
	string Item;
	Token() { Line = 0; }
};

void Judge();
void Judge1();
void Judge2();
void Judge3();
void Initiation(); //��ʼ������
bool IsReserved(); //�ж��Ƿ�Ϊ������
bool IsChar();     //�ж��Ƿ�Ϊ�����ַ�
bool IsID();       //�ж��Ƿ�Ϊ��ʶ��
bool IsNumber();   //�ж��Ƿ�Ϊ����
void getToken();
void getToken_s(string);
bool CheckStr();   //���Token�б����Ƿ��д���ĵ���
void OutputToken();
string OutputToken_s();

