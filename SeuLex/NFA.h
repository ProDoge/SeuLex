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
	void insert(int start, int end, char alpha);	//�����
	void insertF(int state);						//������̬
	void insertF(int state, string action);			//������̬
	NFA merge(NFA n1, NFA n2);
	NFA();
	NFA(char ch);
	vector<int> closure(int i);						//i��epsilon�հ�
	vector<int> closure(vector<int> I);				//I��epsilon�հ�
	vector<int> move(int i, char ch);				//�ҵ�i��ch�հ�
	vector<int> move(vector<int> I, char ch);		//�ҵ�I��ch�հ�
	void stateMove(int steps);
	int  judgeF(vector<int> I);						//�жϼ���I���Ƿ�����̬
	NFA InPostfixCreateNFA(string &Infix,string action);			//��������ʽ������׺���ʽת��NFA
	bool InsertPot(string &Infix);					//����׺���ʽ�м����'.' 
	bool IsletNumOp(char &ch);						//�жϵ�ǰ������ַ�c�Ƿ�����ĸ�������ֻ��߲�����
	bool InfixToPostfix(string &Infix, string &Postfix);//��׺���ʽת��׺���ʽ
	int priority(char &ch);							//ʶ������������ȼ��Ⱥ�
	NFA OpLink(NFA n1, NFA n2);
	NFA NFA::OpOR(NFA n1, NFA n2);
	NFA NFA::OpStar(NFA n);
};