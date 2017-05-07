#pragma once
#include "NFA.h"
class DFA
{
private:
	int							stateNum;
	vector<map<char, int>>		sigma;		//���е���ת����,��ǰstateΪsigma���±�
	vector<char>				alphabet;	//��ĸ����
	vector<int>					F;			//��̬����
	map<int, string>			actionTB;	//��̬������map
public:
	DFA(NFA n);								//NFAתDFA����С��
	void DFAmin();							//��С��DFA
};