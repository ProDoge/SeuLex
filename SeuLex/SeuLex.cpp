#include<iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include "DFA.h"
using namespace std;
#define ERROR -1//����ERROR��״̬���Ϊ-1
#define BEGIN 1//����BEGIN��״̬���Ϊ-1
#define END 2//����END��״̬���Ϊ-1
#define LAYER 3//����LAYER��״̬���Ϊ-1
ifstream ifile;//���������ļ���
ofstream ofile;//��������ļ���
int state;//�������ͱ�����ʾ״̬���
int lineno;
map<string, string> RETable;//�洢������ʽ
map<string, string> reaction;//�洢ģʽ�붯��
vector<string> actionTable;//������Ŷ����ı��
int check(char ch);//��⵱ǰλ�ڡ�%����ķ���������״̬���
bool TransformRE(string &re);//������ת��Ϊ������ʽ
void DefinitionOutput(char &ch);//�������岿�����ݽ�������
void RuleAnalyze(char &ch);//���򲿷ֵĽ�������
void UserProOutput(char &ch);//�û��ӳ��������������

int main()
{
	cout << "SeuLex����ʼ����!" << endl;
	ifile.open("lex4.l", ios::in);//�����ļ���·��
	ofile.open("lex.cpp", ios::out);//����ļ���·��
	if (!ifile) {//�����ļ����ܴ�
		cerr << "Input file could not be opened" << endl;
		exit(1);
	}
	if (!ofile) {//�����ļ����ܴ�
		cerr << "Output file could not be opened" << endl;
		exit(1);
	}
	char ch;//�����ַ�������������ȡ�������ļ��������ַ�
	DefinitionOutput(ch);//���ø������岿�����ݽ�������
	RuleAnalyze(ch);//���ù��򲿷ֵĽ�������

	UserProOutput(ch);//�����û��ӳ��������������
	ifile.close();//�ر������ļ�
	ofile.close();//�ر�����ļ�
	cout << "SeuLex�����������!" << endl;
	return 0;
}


int check(char ch)//��⵱ǰλ�ڡ�%����ķ���������״̬���
{
	if (ch == '{') {//�ǡ�{������ʾBEGIN
		return BEGIN;
	}
	else if (ch == '}') {//�ǡ�%}������ʾEND
		return END;
	}
	else if (ch == '%') {//�ǡ�%������ʾ���
		return LAYER;
	}
	else //�����������ž�������
	{
		ifile.seekg(-1, ios::cur);//���ļ��Ķ�ָ�뵹��һ��
	}
}




bool TransformRE(string &re)//������ת��Ϊ������ʽ
{
	string exp, temp;
	int len, offset, intcount;
	char ch;
	for (int i = 0; i < re.length(); i++) {
		ch = re[i];
		if (ch == '[') {
			exp.append(1, '(');
		}
		else if (ch == ']') {
			exp.append(1, ')');
		}
		else if (ch == '-') {
			char start, finish;
			start = re[i - 1];
			finish = re[i + 1];
			i++;
			start++;
			if ((re[i - 2] <= '9'&&re[i - 2] >= '0') || (re[i - 2] <= 'z'&&re[i - 2] >= 'a') || (re[i - 2] <= 'Z'&&re[i - 2] >= 'A'))
			{
				exp.erase(exp.length(), 1);
			}
			while (start <= finish) {
				exp.append(1, '|');
				exp.append(1, start);
				start++;
			}
		}
		else if (ch == '|') {
			exp.append(1, '|');
		}
		else if (ch == '{') {
			offset = re.find_first_of('}', i);
			i++;
			while (offset > i) {
				temp.append(1, re[i]);
				i++;
			}
			map<string, string>::iterator iter;//����������map�ĵ�����
			iter = RETable.find(temp);
			intcount = RETable.count(temp);//����temp���ֵĴ���
			if (intcount <= 0) {//û�г��ֹ����ͷ���false
				return false;
			}
			else if (iter != RETable.end()) {//endָ�����һ��Ԫ�ص���һ��Ԫ��
				exp.append(iter->second);
			}
			temp.clear();//�м��ַ������
			i = offset;//����������Ϊ��}����λ��
		}
		else if (ch == '+') {//��ʾ�ǿձհ�
			len = exp.length();
			offset = len;
			while (exp[len - 1] != '(') {
				len--;
			}//�˳�whileʱ���len�����������ڵ�λ��
			temp = exp.substr(len - 1, exp.length());
			exp.append(temp);
			exp.append(1, '*');//�ǿձհ����������ټ���һ���ɿձհ�
		}
		else {
			if (i >= 1 && ((re[i - 1] <= '9'&&re[i - 1] >= '0') || (re[i - 1] <= 'z'&&re[i - 1] >= 'a') || (re[i - 1] <= 'Z'&&re[i - 1] >= 'A')))
			{
				exp.append(1, '|');
				exp.append(1, ch);//ֱ�ӽ��ַ���ӵ�������ʽĩβ
			}
			else
				exp.append(1, ch);//ֱ�ӽ��ַ���ӵ�������ʽĩβ
		}
	}
	re.clear();
	re = exp;
	return true;
}


void DefinitionOutput(char &ch)//�������岿�����ݽ�������
{
	cout << "�������岿�������ʼ!" << endl;
	//���ȰѸ������岿�����뵽�ļ���
	ch = ifile.get();//�������ļ�����һ���ַ�
	if (ch != '%') {//�����ļ����ַ����ǡ�%�����ţ�����ʾ�����ļ��ĸ�ʽ����ȷ
		cerr << "Input File don't have the correct format!" << endl;
		state = ERROR;//״̬ΪERROR����Ϊ-1
		exit(1);//�˳���ǰmain����
	}
	else {//�����ļ����ַ�Ϊ��%��
		ch = ifile.get();//��ȡ�����ļ�����һ���ַ�
		state = check(ch);//���ݵ�ǰ��ȡ���ַ�������state
		if (state != 1) {//���state����ʾBEGIN
			cerr << "Input File don't have the correct format!" << endl;
			state = ERROR;//״̬ΪERROR����Ϊ-1
			exit(1);//�˳���ǰmain����
		}
		else;
	}
	while (!ifile.eof() && state != 2) {//�����ļ�û�ж�����β����state������2����Ϊ��ΪEND״̬��%}��
		ch = ifile.get();//��ȡ�����ļ�����һ���ַ�
		if (ch == '%') {//��ǰ������ַ�Ϊ��%��
			char nextch = ifile.get();//��ȡ�����ļ�����һ���ַ�
			state = check(nextch);//���ݵ�ǰ��ȡ���ַ�������state
		}
		else ofile.put(ch);//����ȡ���ַ���������ļ���
	}
	cout << "�������岿��������!" << endl;
}

void KeywordToRe()//�ؼ�����������ʽ���ֵĴ���
{
	state = BEGIN;
	string id, re;
	while (!ifile.eof() && state != 3)
	{
		ifile >> id >> re;
		if (id == "%%")//��ɨ�赽Ϊ�ֲ��־��ʱ��״̬��ʶ��ΪLAYER�������˳�ѭ��
		{
			state = LAYER;
			break;
		}
		TransformRE(re);//ת����������ʽ
		RETable.insert(pair<string, string>(id, re));//��������ʽ����map��
	}

	map<string, string>::iterator iter;//������
	for (iter = RETable.begin(); iter != RETable.end(); iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}
}

void RuleAnalyze(char &ch)//���򲿷ֵĽ�������
{
	cout << "ʶ����򲿷ֽ�����ʼ!" << endl;
	KeywordToRe();//���ùؼ�����������ʽ���ֵĴ���
	//���濪ʼ����ģʽ�붯����������
	state = BEGIN;
	ifile.seekg(-3, ios::cur);
	while (!ifile.eof() && state != 3)
	{
		ch = ifile.get();
		if (ch == '\n' )
			continue;
		else
		{
			ifile.seekg(-1, ios::cur);//��ǰ�ļ�ɨ��ָ���˻�һ��
		}
		string str;
		getline(ifile, str);//��ȡÿһ������
		if (str == "%%")//��ɨ�赽Ϊ�ֲ��־��ʱ��״̬��ʶ��ΪLAYER�������˳�ѭ��
		{
			state = LAYER;
			break;
		}
		string re, action;
		int offset;
		for (offset = 0; offset < str.length(); offset++)
		{
			if (str[offset] == '\t'&&str[offset + 1] == '\t') {
				break;
			}
			else;
		}//offset������ȡģʽ�붯��֮��Ŀո��λ�ã��Ա㽫ģʽ��ո�ֿ�
		re = str.substr(0, offset);//ģʽ��ÿһ�е�ǰoffset���ַ�
		while (str[offset] == '\t') offset++;//��ָ�����������Ŀ�ʼ����Ϊ�ǿո�
		action = str.substr(offset, str.size() - offset);//��ȡ�����Ӵ�
		int len = re.size();
		if (re[0] == '{'&&re[len-1]=='}')//���ģʽ�����û������ˣ�����REtable���Ӧ��������ʽ����re
		{
			re=re.substr(1, len - 2);//�����������ڲ����ַ���
			map<string, string>::iterator iter;//����������map�ĵ�����
			iter = RETable.find(re);
			re = iter->second;
			reaction.insert(pair<string, string>(re, action));//���򲿷ְ�ģʽ�Ͷ����ֱ�洢��reaction�У���pair����ʽ
		}
		else//���ģʽ����û���û����ţ���ֱ����re
		{
			reaction.insert(pair<string, string>(re, action));//���򲿷ְ�ģʽ�Ͷ����ֱ�洢��reaction�У���pair����ʽ
		}
		actionTable.push_back(re);//��������ʽѹջ
	}
	NFA nfa, newNFA;
	string Infix;//����׺���ʽ
	string str;
	map<string, string>::iterator iter;//������
	iter = reaction.begin();
	str = iter->first;
	//cout << iter->first << "     " << iter->second << endl;
	nfa = nfa.InPostfixCreateNFA(str,iter->second);
	iter++;
	cout << "YES" << endl;
	for (; iter != reaction.end(); iter++)
	{
		//cout << iter->first << "     " << iter->second << endl;
		Infix = iter->first;
		newNFA = nfa.InPostfixCreateNFA(Infix, iter->second);
		nfa = nfa.merge(nfa, newNFA);
	}//��re��action�Ķ�ȡ
	DFA dfa0(nfa);
	dfa0.DFAmin();
	cout << "ʶ����򲿷ֽ�������!" << endl;
}

void UserProOutput(char &ch)//�û��ӳ��������������
{
	cout << "�û��ӳ��򲿷������ʼ!" << endl;
	//��������û��ӳ��򲿷�����
	while (!ifile.eof())//���ļ�����δ����ʱ
	{
		ch = ifile.get();//��ȡ�ַ�
		ofile.put(ch);//����ȡ���ַ���������ļ���
	}
	cout << "�û��ӳ��򲿷��������!" << endl;
}