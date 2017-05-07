#pragma once
#include "NFAstate.h"
#include <algorithm>
#include <vector>
#include<string>
#include <map>
class NFA
{
public:
	int head;
	int tail;
	vector<NFAstate>		S;
	vector<char>			alphabet;
	vector<int>				F;
	map<int, string>			actionTB;
	void insert(int start, int end, char alpha);	//插入边
	void insertF(int state);						//插入终态
	void insertF(int state, string action);			//插入终态
	NFA merge(NFA n1, NFA n2);
	NFA();
	NFA(char ch);
	vector<int> closure(int i);						//i的epsilon闭包
	vector<int> closure(vector<int> I);				//I的epsilon闭包
	vector<int> move(int i, char ch);				//找到i的ch闭包
	vector<int> move(vector<int> I, char ch);		//找到I的ch闭包
	void stateMove(int steps);
	int  judgeF(vector<int> I);						//判断集合I中是否含有终态
	NFA InPostfixCreateNFA(string &Infix,string action);			//由正则表达式经过后缀表达式转成NFA
	bool InsertPot(string &Infix);					//在中缀表达式中加入点'.' 
	bool IsletNumOp(char &ch);						//判断当前读入的字符c是否是字母或者数字或者操作符
	bool InfixToPostfix(string &Infix, string &Postfix);//中缀表达式转后缀表达式
	int priority(char &ch);							//识别操作符的优先级先后
	NFA OpLink(NFA n1, NFA n2);
	NFA NFA::OpOR(NFA n1, NFA n2);
	NFA NFA::OpStar(NFA n);
};