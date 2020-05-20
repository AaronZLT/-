#define _CRT_SECURE_NO_DEPRECATE
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


class Tree {
public:
	string Item;           //存放字符串
	vector<Tree*> Children;
	Tree(string S) { Item = S; }
	void add(Tree* T) {
		Children.push_back(T);
	}
};

void Display(Tree* T, int depth, vector<int>* v, bool buttom);//显示语法树
void Display_s(Tree* T, int depth, vector<int>* v, bool buttom);
string _Display(Tree* T, int depth, vector<int>* v, bool buttom);
void UseToken();
void ERROROUTPUT(string S);
int ErrCount();
string _ERROROUTPUT();
Tree* MatchReserved(string S); // 匹配保留字 返回子节点
Tree* MatchID();               // 匹配标识符
Tree* MatchNumber();           // 匹配无符号整数
Tree* MatchChar(char S);     // 匹配字符
/*****语法分析程序*****/
Tree* Program();
Tree* ProgramHead();
Tree* DeclarePart();
Tree* ProgramBody();
//ProgramHead
//Tree* ProgramName(); 
//DeclarePart
Tree* TypeDecPart();
Tree* VarDecPart();
Tree* ProcDecpart();
//TypeDecPart
Tree* TypeDec();
Tree* TypeDecList();
Tree* TypeID();
Tree* TypeDef();
Tree* TypeDecMore();
//TypeDef
Tree* BaseType();
Tree* StructureType();
Tree* ArrayType();
Tree* RecType();
Tree* FieldDecList();
Tree* IDList();
Tree* FieldDecMore();
Tree* IDMore();
//VarDecPart
Tree* VarDec();
Tree* VarDecList();
Tree* VarIDList();
Tree* VarDecMore();
Tree* VarIDMore();
//ProcDecpart
Tree* ProcDec();
//Tree* ProcName();
Tree* ParamList();
Tree* ProcDecPart();
Tree* ProcBody();
Tree* ProcDeclaration();
//ParamList
Tree* ParamDecList();
Tree* Param();
Tree* ParamMore();
Tree* FormList();
Tree* FIDMore();
//ProgramBody
Tree* StmList();
//StmList
Tree* Stm();
Tree* StmMore();
//Stm
Tree* ConditionalStm();
Tree* LoopStm();
Tree* InputStm();
Tree* OutputStm();
Tree* ReturnStm();
Tree* AssCall();
Tree* AssignmentRest();
Tree* CallStmRest();
//AssignmentRest
Tree* VariMore();
Tree* Exp();
//ConditionalStm
Tree* RelExp();
//InputStm
Tree* Invar();
//CallStmRest
Tree* ActParamList();
Tree* ActParamMore();
//RelExp
Tree* OtherRelE();
Tree* CmpOp();
//Exp
Tree* Term();
Tree* OtherTerm();
Tree* AddOp();
//Term
Tree* Factor();
Tree* OtherFactor();
Tree* MultOp();
//Factor
Tree* Variable();
Tree* FieldVar();
Tree* FieldVarMore();

