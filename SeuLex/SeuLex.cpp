#include<iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include "DFA.h"
using namespace std;
#define ERROR -1//定义ERROR的状态编号为-1
#define BEGIN 1//定义BEGIN的状态编号为-1
#define END 2//定义END的状态编号为-1
#define LAYER 3//定义LAYER的状态编号为-1
ifstream ifile;//定义输入文件流
ofstream ofile;//定义输出文件流
int state;//定义整型变量表示状态编号
int lineno;
map<string, string> RETable;//存储正则表达式
map<string, string> reaction;//存储模式与动作
vector<string> actionTable;//用来存放动作的表格
int check(char ch);//检测当前位于‘%’后的符号来决定状态编号
bool TransformRE(string &re);//将规则转换为正则表达式
void DefinitionOutput(char &ch);//辅助定义部分内容解析函数
void RuleAnalyze(char &ch);//规则部分的解析函数
void UserProOutput(char &ch);//用户子程序内容输出函数

int main()
{
	cout << "SeuLex程序开始运行!" << endl;
	ifile.open("lex4.l", ios::in);//输入文件的路径
	ofile.open("lex.cpp", ios::out);//输出文件的路径
	if (!ifile) {//输入文件不能打开
		cerr << "Input file could not be opened" << endl;
		exit(1);
	}
	if (!ofile) {//输入文件不能打开
		cerr << "Output file could not be opened" << endl;
		exit(1);
	}
	char ch;//定义字符变量，用来读取从输入文件读出的字符
	DefinitionOutput(ch);//调用辅助定义部分内容解析函数
	RuleAnalyze(ch);//调用规则部分的解析函数

	UserProOutput(ch);//调用用户子程序内容输出函数
	ifile.close();//关闭输入文件
	ofile.close();//关闭输出文件
	cout << "SeuLex程序结束运行!" << endl;
	return 0;
}


int check(char ch)//检测当前位于‘%’后的符号来决定状态编号
{
	if (ch == '{') {//是‘{’，表示BEGIN
		return BEGIN;
	}
	else if (ch == '}') {//是‘%}’，表示END
		return END;
	}
	else if (ch == '%') {//是‘%’，表示层次
		return LAYER;
	}
	else //以上三个符号均都不是
	{
		ifile.seekg(-1, ios::cur);//将文件的读指针倒退一个
	}
}




bool TransformRE(string &re)//将规则转换为正则表达式
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
			map<string, string>::iterator iter;//定义适用于map的迭代器
			iter = RETable.find(temp);
			intcount = RETable.count(temp);//返回temp出现的次数
			if (intcount <= 0) {//没有出现过，就返回false
				return false;
			}
			else if (iter != RETable.end()) {//end指向最后一个元素的下一个元素
				exp.append(iter->second);
			}
			temp.clear();//中间字符串清空
			i = offset;//将索引更新为‘}’的位置
		}
		else if (ch == '+') {//表示非空闭包
			len = exp.length();
			offset = len;
			while (exp[len - 1] != '(') {
				len--;
			}//退出while时候的len就是左括号在的位置
			temp = exp.substr(len - 1, exp.length());
			exp.append(temp);
			exp.append(1, '*');//非空闭包就是自身再加上一个可空闭包
		}
		else {
			if (i >= 1 && ((re[i - 1] <= '9'&&re[i - 1] >= '0') || (re[i - 1] <= 'z'&&re[i - 1] >= 'a') || (re[i - 1] <= 'Z'&&re[i - 1] >= 'A')))
			{
				exp.append(1, '|');
				exp.append(1, ch);//直接将字符添加到正则表达式末尾
			}
			else
				exp.append(1, ch);//直接将字符添加到正则表达式末尾
		}
	}
	re.clear();
	re = exp;
	return true;
}


void DefinitionOutput(char &ch)//辅助定义部分内容解析函数
{
	cout << "辅助定义部分输出开始!" << endl;
	//首先把辅助定义部分输入到文件中
	ch = ifile.get();//读输入文件的下一个字符
	if (ch != '%') {//输入文件首字符不是‘%’符号，则显示输入文件的格式不正确
		cerr << "Input File don't have the correct format!" << endl;
		state = ERROR;//状态为ERROR，即为-1
		exit(1);//退出当前main函数
	}
	else {//输入文件首字符为‘%’
		ch = ifile.get();//读取输入文件的下一个字符
		state = check(ch);//根据当前读取的字符来决定state
		if (state != 1) {//如果state不表示BEGIN
			cerr << "Input File don't have the correct format!" << endl;
			state = ERROR;//状态为ERROR，即为-1
			exit(1);//退出当前main函数
		}
		else;
	}
	while (!ifile.eof() && state != 2) {//输入文件没有读到结尾并且state不等于2，即为不为END状态（%}）
		ch = ifile.get();//读取输入文件的下一个字符
		if (ch == '%') {//当前读入的字符为‘%’
			char nextch = ifile.get();//读取输入文件的下一个字符
			state = check(nextch);//根据当前读取的字符来决定state
		}
		else ofile.put(ch);//将读取的字符存入输出文件中
	}
	cout << "辅助定义部分输出完毕!" << endl;
}

void KeywordToRe()//关键词与正则表达式部分的处理
{
	state = BEGIN;
	string id, re;
	while (!ifile.eof() && state != 3)
	{
		ifile >> id >> re;
		if (id == "%%")//当扫描到为分层标志符时将状态标识赋为LAYER，并且退出循环
		{
			state = LAYER;
			break;
		}
		TransformRE(re);//转换成正则表达式
		RETable.insert(pair<string, string>(id, re));//把正则表达式存在map中
	}

	map<string, string>::iterator iter;//迭代器
	for (iter = RETable.begin(); iter != RETable.end(); iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}
}

void RuleAnalyze(char &ch)//规则部分的解析函数
{
	cout << "识别规则部分解析开始!" << endl;
	KeywordToRe();//调用关键词与正则表达式部分的处理
	//下面开始处理模式与动作部分内容
	state = BEGIN;
	ifile.seekg(-3, ios::cur);
	while (!ifile.eof() && state != 3)
	{
		ch = ifile.get();
		if (ch == '\n' )
			continue;
		else
		{
			ifile.seekg(-1, ios::cur);//当前文件扫描指针退回一个
		}
		string str;
		getline(ifile, str);//获取每一行内容
		if (str == "%%")//当扫描到为分层标志符时将状态标识赋为LAYER，并且退出循环
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
		}//offset用来获取模式与动作之间的空格的位置，以便将模式与空格分开
		re = str.substr(0, offset);//模式是每一行的前offset个字符
		while (str[offset] == '\t') offset++;//将指针移至动作的开始，即为非空格处
		action = str.substr(offset, str.size() - offset);//获取动作子串
		int len = re.size();
		if (re[0] == '{'&&re[len-1]=='}')//如果模式部分用花括号了，则用REtable里对应的正则表达式代替re
		{
			re=re.substr(1, len - 2);//保留花括号内部的字符串
			map<string, string>::iterator iter;//定义适用于map的迭代器
			iter = RETable.find(re);
			re = iter->second;
			reaction.insert(pair<string, string>(re, action));//规则部分把模式和动作分别存储在reaction中，以pair的形式
		}
		else//如果模式部分没有用花括号，则直接用re
		{
			reaction.insert(pair<string, string>(re, action));//规则部分把模式和动作分别存储在reaction中，以pair的形式
		}
		actionTable.push_back(re);//将正则表达式压栈
	}
	NFA nfa, newNFA;
	string Infix;//存中缀表达式
	string str;
	map<string, string>::iterator iter;//迭代器
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
	}//对re和action的读取
	DFA dfa0(nfa);
	dfa0.DFAmin();
	cout << "识别规则部分解析结束!" << endl;
}

void UserProOutput(char &ch)//用户子程序内容输出函数
{
	cout << "用户子程序部分输出开始!" << endl;
	//下面读入用户子程序部分内容
	while (!ifile.eof())//当文件读入未结束时
	{
		ch = ifile.get();//读取字符
		ofile.put(ch);//将读取的字符放入输出文件中
	}
	cout << "用户子程序部分输出结束!" << endl;
}