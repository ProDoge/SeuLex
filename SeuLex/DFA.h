#pragma once
#include "NFA.h"
class DFA
{
private:
	int							stateNum;
	vector<map<char, int>>		sigma;		//所有的跳转动作,当前state为sigma的下标
	vector<char>				alphabet;	//字母表集合
	vector<int>					F;			//终态集合
	map<int, string>			actionTB;	//终态动作集map
public:
	DFA(NFA n);								//NFA转DFA并最小化
	void DFAmin();							//最小化DFA
};