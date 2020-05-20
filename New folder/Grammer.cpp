#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
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

#include "Grammer.h"
#include "GlobleVar.h"
#include "TokenDetect.h"

using namespace std;

extern queue<Token>   TokenTable;
extern string CurrentString;
extern int    CurrentLine;
extern int ERRCount;
extern string GrammerTreeString;
extern string Errorinfo;

void UseToken() {
	CurrentString = TokenTable.front().Item;
	CurrentLine = TokenTable.front().Line;
	TokenTable.pop();
}
void ERROROUTPUT(string S) {
	ERRCount++;
	cout << "error in line: " << CurrentLine << endl;
	cout << S << endl;
	Errorinfo += "error in line: ";
	Errorinfo += to_string(CurrentLine);
	Errorinfo += "\r\n";
	Errorinfo += S;
	Errorinfo += "\r\n";
	//exit(0);
}

int ErrCount()
{
	return ERRCount;
}

string _ERROROUTPUT()
{
	if (ERRCount > 0) {
		return Errorinfo;
	}
}

/***匹配并返回相应叶节点****/
Tree* MatchReserved(string S) {
	UseToken();
	if (IsReserved() && !strcmpi(CurrentString.c_str(), S.c_str())) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	string s = "Reserved word error! " + S + " exepected";
	ERROROUTPUT(s);
	return NULL;
}
Tree* MatchID() {
	UseToken();
	if (IsID()) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	ERROROUTPUT("Identifier error! ");
	return NULL;
}
Tree* MatchNumber() {
	UseToken();
	if (IsNumber()) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	ERROROUTPUT("Number error! ");
	return NULL;
}
Tree* MatchChar(char S) {
	UseToken();
	string str = "0";
	str[0] = S;
	if (IsChar() && CurrentString[0] == S)
	{
		if (CurrentString.length() == 2 && CurrentString[1] == '=')
		{
			if (CurrentString[1] == '=')
			{
				Tree* t = new Tree(CurrentString);
				return t;
			}
		}
		else {
			Tree* t = new Tree(CurrentString);
			return t;
		}
	}
	string s = "Character error! " + str + " exepected";
	ERROROUTPUT(s);
	return NULL;
}

/********语法分析程序********/
Tree* Program() {
	Tree* T = new Tree("Program");
	T->add(ProgramHead());
	T->add(DeclarePart());
	T->add(ProgramBody());
	return T;
}
Tree* ProgramHead() {
	Tree* T = new Tree("ProgramHead");
	T->add(MatchReserved("program"));
	T->add(MatchID());
	return T;
}
Tree* DeclarePart() {
	Tree* T = new Tree("DeclarePart");
	T->add(TypeDecPart());
	T->add(VarDecPart());
	T->add(ProcDecpart());
	return T;
}
Tree* ProgramBody() {
	Tree* T = new Tree("ProgramBody");
	T->add(MatchReserved("begin"));
	T->add(StmList());
	T->add(MatchReserved("end"));
	return T;
}
//ProgramHead
//Tree* ProgramName(){Tree* T = new Tree();return T;}   到ProgramHead中
//DeclarePart
Tree* TypeDecPart() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "type")) {
		T = new Tree("TypeDecPart");
		T->add(TypeDec());
	}
	return T;
}
Tree* VarDecPart() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "var")) {
		T = new Tree("VarDecPart");
		T->add(VarDec());
	}
	return T;
}
Tree* ProcDecpart() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "procedure")) {
		T = new Tree("ProcDecpart");
		T->add(ProcDec());
	}
	return T;
}
//TypeDecPart
Tree* TypeDec() {
	Tree* T = new Tree("TypeDec");
	T->add(MatchReserved("type"));
	T->add(TypeDecList());
	return T;
}
Tree* TypeDecList() {
	Tree* T = new Tree("TypeDecList");
	T->add(TypeID());
	T->add(MatchChar('='));
	T->add(TypeDef());
	T->add(MatchChar(';'));
	T->add(TypeDecMore());
	return T;
}
Tree* TypeID() {
	Tree* T = new Tree("TypeID");
	T->add(MatchID());
	return T;
}
Tree* TypeDef() {
	Tree* T = new Tree("TypeDef");
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char")) {
		T->add(BaseType());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "array") ||
		!strcmp(TokenTable.front().Item.c_str(), "record")) {
		T->add(StructureType());
		return T;
	}
	T->add(MatchID());
	return T;
}
Tree* TypeDecMore() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (IsID()) {
		T = new Tree("TypeDecMore");
		T->add(TypeDecList());
	}
	CurrentString = Temp;
	return T;
}
//TypeDef
Tree* BaseType() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer")) {
		T = new Tree("BaseType");
		T->add(MatchReserved("integer"));
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "char")) {
		T = new Tree("BaseType");
		T->add(MatchReserved("char"));
		return T;
	}
	UseToken();
	ERROROUTPUT(CurrentString + " is not a base type! ");
	return T;
}
Tree* StructureType() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "array")) {
		T = new Tree("StructureType");
		T->add(ArrayType());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "record")) {
		T = new Tree("BaseType");
		T->add(RecType());
		return T;
	}
	return T;
}
Tree* ArrayType() {
	Tree* T = new Tree("ArrayType");
	T->add(MatchReserved("array"));
	T->add(MatchChar('['));
	T->add(MatchNumber());
	T->add(MatchChar('.'));
	T->add(MatchChar('.'));
	T->add(MatchNumber());
	T->add(MatchChar(']'));
	T->add(MatchReserved("of"));
	T->add(BaseType());
	return T;
}
Tree* RecType() {
	Tree* T = new Tree("RecType");
	T->add(MatchReserved("record"));
	T->add(FieldDecList());
	T->add(MatchReserved("end"));
	return T;
}
Tree* FieldDecList() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char")) {
		T = new Tree("FieldDecList");
		T->add(BaseType());
		T->add(IDList());
		T->add(MatchChar(';'));
		T->add(FieldDecMore());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "array")) {
		T = new Tree("FieldDecList");
		T->add(ArrayType());
		T->add(IDList());
		T->add(MatchChar(';'));
		T->add(FieldDecMore());
		return T;
	}
	ERROROUTPUT("Field declare list error.");
	return T;
}
Tree* IDList() {
	Tree* T = new Tree("IDList");
	T->add(MatchID());
	T->add(IDMore());
	return T;
}
Tree* FieldDecMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char") ||
		!strcmp(TokenTable.front().Item.c_str(), "array")) {
		T = new Tree("FieldDecMore");
		T->add(FieldDecList());
	}
	return T;
}
Tree* IDMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), ",")) {
		T = new Tree("IDMore");
		T->add(MatchChar(','));
		T->add(IDList());
	}
	return T;
}
//VarDecPart
Tree* VarDec() {
	Tree* T = new Tree("VarDec");
	T->add(MatchReserved("var"));
	T->add(VarDecList());
	return T;
}
Tree* VarDecList() {
	Tree* T = new Tree("VarDecList");
	T->add(TypeDef());
	T->add(VarIDList());
	T->add(MatchChar(';'));
	T->add(VarDecMore());
	return T;
}
Tree* VarIDList() {
	Tree* T = new Tree("VarIDList");
	T->add(MatchID());
	T->add(VarIDMore());
	return T;
}
Tree* VarDecMore() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char") ||
		!strcmp(TokenTable.front().Item.c_str(), "array") ||
		!strcmp(TokenTable.front().Item.c_str(), "record") ||
		IsID()) {
		T = new Tree("VarDecMore");
		T->add(VarDecList());
	}
	CurrentString = Temp;
	return T;
}
Tree* VarIDMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), ",")) {
		T = new Tree("VarIDMore");
		T->add(MatchChar(','));
		T->add(VarIDList());
	}
	return T;
}
//ProcDecpart
Tree* ProcDec() {
	Tree* T = new Tree("ProcDec");
	T->add(MatchReserved("procedure"));
	T->add(MatchID());
	T->add(MatchChar('('));
	T->add(ParamList());
	T->add(MatchChar(')'));
	T->add(MatchChar(';'));
	T->add(ProcDecPart());
	T->add(ProcBody());
	T->add(ProcDecpart());
	return T;
}
//Tree* ProcName();    直接变成ID
Tree* ParamList() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char") ||
		!strcmp(TokenTable.front().Item.c_str(), "array") ||
		!strcmp(TokenTable.front().Item.c_str(), "record") ||
		!strcmp(TokenTable.front().Item.c_str(), "var") ||
		IsID()) {
		T = new Tree("ParamList");
		T->add(ParamDecList());
	}
	CurrentString = Temp;
	return T;
}
Tree* ProcDecPart() {
	Tree* T = new Tree("ProcDecPart");
	T->add(DeclarePart());
	return T;
}
Tree* ProcBody() {
	Tree* T = new Tree("ProcBody");
	T->add(ProgramBody());
	return T;
}
//ParamList
Tree* ParamDecList() {
	Tree* T = new Tree("ParamDecList");
	T->add(Param());
	T->add(ParamMore());
	return T;
}
Tree* Param() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (!strcmp(TokenTable.front().Item.c_str(), "integer") ||
		!strcmp(TokenTable.front().Item.c_str(), "char") ||
		!strcmp(TokenTable.front().Item.c_str(), "array") ||
		!strcmp(TokenTable.front().Item.c_str(), "record") ||
		IsID()) {
		T = new Tree("Param");
		T->add(TypeDef());
		T->add(FormList());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "var")) {
		T = new Tree("Param");
		T->add(MatchReserved("var"));
		T->add(TypeDef());
		T->add(FormList());
		return T;
	}
	CurrentString = Temp;
	ERROROUTPUT("Param error! ");
	return T;
}
Tree* ParamMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), ";")) {
		T = new Tree("ParamMore");
		T->add(MatchChar(';'));
		T->add(ParamDecList());
	}
	return T;
}
Tree* FormList() {
	Tree* T = new Tree("FormList");
	T->add(MatchID());
	T->add(FIDMore());
	return T;
}
Tree* FIDMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), ",")) {
		T = new Tree("FIDMore");
		T->add(MatchChar(','));
		T->add(FormList());
	}
	return T;
}

Tree* StmList() {
	Tree* T = new Tree("StmList");
	T->add(Stm());
	T->add(StmMore());
	return T;
}
//StmList
Tree* Stm() {
	Tree* T = new Tree("Stm");
	if (!strcmp(TokenTable.front().Item.c_str(), "if")) {
		T->add(ConditionalStm());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "while")) {
		T->add(LoopStm());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "read")) {
		T->add(InputStm());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "write")) {
		T->add(OutputStm());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "return")) {
		T->add(ReturnStm());
		return T;
	}
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (IsID()) {
		T->add(MatchID());
		T->add(AssCall());
	}
	CurrentString = Temp;
	delete T;
	return NULL;
}
Tree* StmMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), ";")) {
		T = new Tree("StmMore");
		T->add(MatchChar(';'));
		T->add(StmList());
	}
	return T;
}
//Stm
Tree* ConditionalStm() {
	Tree* T = new Tree("ConditionalStm");
	T->add(MatchReserved("if"));
	T->add(Exp());
	if (!strcmp(TokenTable.front().Item.c_str(), "<"))
	{
		T->add(MatchChar('<'));
	}
	else if (!strcmp(TokenTable.front().Item.c_str(), "="))
	{
		T->add(MatchChar('='));
	}
	else
	{
		ERROROUTPUT("condition error\n");
	}
	T->add(Exp());
	T->add(MatchReserved("then"));
	T->add(StmList());
	T->add(MatchReserved("else"));
	T->add(StmList());
	T->add(MatchReserved("fi"));
	return T;
}
Tree* LoopStm() {
	Tree* T = new Tree("LoopStm");
	T->add(MatchReserved("while"));
	T->add(Exp());
	if (!strcmp(TokenTable.front().Item.c_str(), "<"))
	{
		T->add(MatchChar('<'));
	}
	else if (!strcmp(TokenTable.front().Item.c_str(), "="))
	{
		T->add(MatchChar('='));
	}
	else
	{
		ERROROUTPUT("condition error\n");
	}
	T->add(Exp());
	T->add(MatchReserved("do"));
	T->add(StmList());
	T->add(MatchReserved("endwh"));
	return T;
}
Tree* InputStm() {
	Tree* T = new Tree("InputStm");
	T->add(MatchReserved("read"));
	T->add(MatchChar('('));
	T->add(MatchID());
	T->add(MatchChar(')'));
	return T;
}
Tree* OutputStm() {
	Tree* T = new Tree("OutputStm");
	T->add(MatchReserved("write"));
	T->add(MatchChar('('));
	T->add(Exp());
	T->add(MatchChar(')'));
	return T;
}
Tree* ReturnStm() {
	Tree* T = new Tree("ReturnStm");
	T->add(MatchReserved("return"));
	return T;
}
Tree* AssCall() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "[") ||
		!strcmp(TokenTable.front().Item.c_str(), ".") ||
		!strcmp(TokenTable.front().Item.c_str(), ":="))
	{
		T = new Tree("AssCall");
		T->add(AssignmentRest());
		return T;
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "("))
	{
		T = new Tree("AssCall");
		T->add(CallStmRest());
		return T;
	}
	ERROROUTPUT("ass call error\n");
	return T;
}
Tree* AssignmentRest() {
	Tree* T = new Tree("AssignmentRest");
	if (!strcmp(TokenTable.front().Item.c_str(), "[") ||
		!strcmp(TokenTable.front().Item.c_str(), "."))
	{
		T->add(VariMore());
	}
	T->add(MatchChar(':'));
	//T->add(MatchChar('='));
	T->add(Exp());
	return T;
}
Tree* CallStmRest() {
	Tree* T = new Tree("CallStmRest");
	T->add(MatchChar('('));
	T->add(ActParamList());
	T->add(MatchChar(')'));
	return T;
}
//AssignmentRest
Tree* VariMore() {
	Tree* T = NULL;
	if (!strcmp(TokenTable.front().Item.c_str(), "["))
	{
		T = new Tree("VariMore");
		T->add(MatchChar('['));
		T->add(Exp());
		T->add(MatchChar(']'));
	}
	if (!strcmp(TokenTable.front().Item.c_str(), "."))
	{
		T = new Tree("VariMore");
		T->add(MatchChar('.'));
		T->add(FieldVar());
	}
	return T;
}
Tree* Exp() {
	Tree* T = new Tree("Exp");
	T->add(Term());
	T->add(OtherTerm());
	return T;
}
//ConditionalStm
#if(0)Tree* RelExp() {
Tree* T = new Tree("RelExp");
T->add(Exp());
T->add(OtherRelE());
return T;
}
#endif
//LoopStm

//InputStm
#if(0)
Tree* Invar() {
	Tree* T = new Tree();
	return T;
}
#endif
//OutputStm

//ReturnStm

//CallStmRest
Tree* ActParamList() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;
	if (!strcmpi(TokenTable.front().Item.c_str(), "(") ||
		IsNumber() ||
		IsID()) {
		T = new Tree("ActParamList");
		T->add(Exp());
		T->add(ActParamMore());
	}
	CurrentString = Temp;
	return T;
}
Tree* ActParamMore() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), ",")) {
		T = new Tree("ActParamMore");
		T->add(MatchChar(','));
		T->add(ActParamList());
	}
	return T;
}
//RelExp
//Tree* OtherRelE() { Tree* T = new Tree(); return T; }
//Tree* CmpOp() { Tree* T = new Tree(); return T; }
//Exp
Tree* Term() {
	Tree* T = new Tree("Term");
	T->add(Factor());
	T->add(OtherFactor());
	return T;
}
Tree* OtherTerm() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "+") ||
		!strcmpi(TokenTable.front().Item.c_str(), "-")) {
		Tree* T = new Tree("OtherTerm");
		T->add(AddOp());
		T->add(Exp());
	}
	return T;
}
Tree* AddOp() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "+"))
	{
		Tree* T = new Tree("AddOp");
		T->add(MatchChar('+'));
		return T;
	}
	if (!strcmpi(TokenTable.front().Item.c_str(), "-"))
	{
		Tree* T = new Tree("AddOp");
		T->add(MatchChar('-'));
		return T;
	}
	ERROROUTPUT("cmpop error\n");
	return T;
}
//Term
Tree* Factor() {
	Tree* T = new Tree("Factor");
	string Temp = CurrentString;
	CurrentString = TokenTable.front().Item;

	if (IsNumber())
	{
		T->add(MatchNumber());
		return T;
	}
	if (!strcmpi(TokenTable.front().Item.c_str(), "("))
	{
		T->add(MatchChar('('));
		T->add(Exp());
		T->add(MatchChar(')'));
		return T;
	}
	if (IsID())
	{
		T->add(Variable());
		return T;
	}
	cout << endl << CurrentString << endl << endl;
	CurrentString = Temp;
	delete T;
	cout << CurrentString << endl;
	ERROROUTPUT("factor error\n");
	return NULL;
}
Tree* OtherFactor() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "*") ||
		!strcmpi(TokenTable.front().Item.c_str(), "/"))
	{
		Tree* T = new Tree("OtherFactor");
		T->add(MultOp());
		T->add(Term());
	}
	return T;
}
Tree* MultOp() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "*"))
	{
		Tree* T = new Tree("MultOp");
		T->add(MatchChar('*'));
		return T;
	}
	if (!strcmpi(TokenTable.front().Item.c_str(), "/"))
	{
		Tree* T = new Tree("MultOp");
		T->add(MatchChar('/'));
		return T;
	}
	ERROROUTPUT("cmpop error\n");
	return T;
}
//Factor
Tree* Variable() {
	Tree* T = new Tree("Variable");
	T->add(MatchID());
	T->add(VariMore());
	return T;
}
Tree* FieldVar() {
	Tree* T = new Tree("FieldVar");
	T->add(MatchID());
	T->add(FieldVarMore());
	return T;
}
Tree* FieldVarMore() {
	Tree* T = NULL;
	if (!strcmpi(TokenTable.front().Item.c_str(), "["))
	{
		Tree* T = new Tree("FieldVarMore");
		T->add(MatchChar('['));
		T->add(Exp());
		T->add(MatchChar(']'));

	}
	return T;
}

void Display(Tree* T, int depth, vector<int>* v, bool buttom) {
	if (T == NULL)return;
	cout << endl;
	int i;
	for (i = 0; i < depth; i++) {
		if (v->at(i) == 1)cout << " │";
		else printf("  ");
	}
	if (depth + 1 == (int)v->size())v->push_back(1);
	if (buttom) {
		cout << " └─" <<" "<<depth<<" "<< T->Item;
		v->at(depth) = 0;
		//v->at(depth + 1) = 0;
	}
	else {
		cout << " ├─" << " " << depth << " " << T->Item;
		//v->at(depth + 1) = 1;
	}
		//if (depth + 1 == (int)v->size())v->push_back(1);
		v->at(depth + 1) = 1;

	
	int len = T->Children.size();
	for (i = 0; i < len; i++) {
		if (i == len - 1)Display(T->Children.at(i), depth + 1, v, true);
		else Display(T->Children.at(i), depth + 1, v, false);
	}
}

void Display_s(Tree* T, int depth, vector<int>* v, bool buttom)
{
	if (T == NULL)return;
	GrammerTreeString += "\r\n";
	int i;
	for (i = 0; i < depth; i++) {
		if (v->at(i) == 1) {
			GrammerTreeString += " |";
		}
		else GrammerTreeString += "  ";
	}
	if (depth + 1 == (int)v->size())v->push_back(1);
	if (buttom) {
		GrammerTreeString += " '--";
		GrammerTreeString += " ";
		GrammerTreeString += to_string(depth);
		GrammerTreeString += " ";
		GrammerTreeString += T->Item;
		v->at(depth) = 0;
	}
	
	else {
		GrammerTreeString += " |--";
		GrammerTreeString += " ";
		GrammerTreeString += to_string(depth);
		GrammerTreeString += " ";
		GrammerTreeString += T->Item;
	}
	v->at(depth + 1) = 1;

	int len = T->Children.size();
	for (i = 0; i < len; i++) {
		if (i == len - 1)Display_s(T->Children.at(i), depth + 1, v, true);
		else Display_s(T->Children.at(i), depth + 1, v, false);
	}
}

string _Display(Tree* T, int depth, vector<int>* v, bool buttom)
{
	Display_s(T, depth, v, buttom);
	return GrammerTreeString;
}
