#include "NFA.h"
#include<algorithm>
#include<iostream>
#include<string>
#include<stack>

NFA::NFA(char ch)
{
	NFAstate n1(0), n2(1);
	head = 0;
	tail = 1;
	insert(0, 1, ch);
}



void NFA::insert(int start, int end, char alpha)
{
	while (max(start, end) >= S.size())
	{
		NFAstate newState(S.size());
		S.push_back(newState);
	}
	S[start].insert(alpha, end);
	vector<char>::iterator it;
	it = find(alphabet.begin(), alphabet.end(), alpha);
	if (it == alphabet.end())
		alphabet.push_back(alpha);
}



void NFA::insertF(int state)
{
	vector<int>::iterator it;
	it = find(F.begin(), F.end(), state);
	if (it == F.end())
		F.push_back(state);
}

void NFA::insertF(int state, string action)
{
	vector<int>::iterator it;
	it = find(F.begin(), F.end(), state);
	if (it == F.end())
		F.push_back(state);
	actionTB.insert(pair<int, string>(state, action));
}

NFA NFA::merge(NFA n1, NFA n2)
{
	NFA newNFA;
	//对原状态集做位移
	n1.stateMove(1);
	int size = n1.S.size();
	n2.stateMove(1 + size);
	//状态合并
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	//插入边
	newNFA.insert(0, 1, NULL);
	newNFA.insert(0, size + 1, NULL);
	//合并字母表和终态集合
	map<int, string>::iterator it0 = n2.actionTB.begin();
	while (it0 != n2.actionTB.end())
	{
		n1.actionTB.insert(pair<int, string>(it0->first, it0->second));
		it0++;
	}
	newNFA.actionTB = n1.actionTB;
	set_union(n1.alphabet.begin(), n1.alphabet.end(), n2.alphabet.begin(), n2.alphabet.end(), back_inserter(newNFA.alphabet));
	set_union(n1.F.begin(), n1.F.end(), n2.F.begin(), n2.F.end(), back_inserter(newNFA.F));
	return newNFA;
}

NFA::NFA()
{
	NFAstate newState(S.size());
	S.push_back(newState);
	head = 0;
	tail = 1;
}

vector<int> NFA::closure(int i)
{
	vector<int> I;
	I.push_back(i);
	return closure(I);
}

vector<int> NFA::closure(vector<int> I)
{
	vector<int> newI, part;
	vector<int>::iterator it;
	int	size;
	//每次把新的epsilon边加进来
	do {
		size = I.size();
		for (int i = 0; i < size; i++)
		{
			//I[i]状态的epsilon边
			part = S[I[i]].nextState(NULL);
			if (part.size())
			{
				newI.clear();
				set_union(I.begin(), I.end(), part.begin(), part.end(), back_inserter(newI));
				sort(newI.begin(), newI.end());
				I = newI;
			}
		}
	} while (size != I.size());
	sort(I.begin(), I.end());
	return I;
}

vector<int> NFA::move(int i, char ch)
{
	vector<int> I;
	I.push_back(i);
	I = closure(I);
	I = move(I, ch);
	return I;
}

vector<int> NFA::move(vector<int> I, char ch)
{
	vector<int> newI, part,newI2;
	vector<int>::iterator it;
	int	size = I.size();
	sort(I.begin(), I.end());
	//每次把新的ch边加进来
	for (int i = 0; i < size; i++)
	{
		//I[i]状态的epsilon边
		part = S[I[i]].nextState(ch);
		if (part.size())
		{
			newI2.clear();
			set_union(newI.begin(), newI.end(), part.begin(), part.end(), back_inserter(newI2));
			sort(newI2.begin(), newI2.end());
			newI = newI2;
		}
	}
	I = closure(newI);
	return I;
}

void NFA::stateMove(int steps)
{
	for (int i = 0; i < F.size(); i++)
	{
		map<int,string>::iterator it = actionTB.find(F[i]);
		string ch = it->second;
		actionTB.erase(it);
		F[i] += steps;
		actionTB.insert(pair<int, string>(F[i], ch));
	}
	for (int i = 0; i < S.size(); i++)
		S[i].stateMove(steps);
	head += steps;
	tail += steps;

}

int NFA::judgeF(vector<int> I)
{
	vector<int>::iterator it;
	for (int i = 0; i < I.size(); i++)
	{
		it = find(F.begin(), F.end(), I[i]);
		if (it != F.end())
			return i+1;
	}
	return 0;
}


NFA NFA::InPostfixCreateNFA(string &Infix,string action)
{
		string Postffix;//存后缀表达式
		this->InsertPot(Infix);//在中缀表达式中加入点'.'
		this->InfixToPostfix(Infix, Postffix);//将中缀表达式转为后缀表达式 
		stack<NFA> nfaStack; //定义暂存中间的生成的NFA栈
		int i = 0;   //用于指示扫描Postffix串的字符的位置
		char ch;
		ch = Postffix[i];//ch保存Postffix中扫描到的字符
		if (ch == '\"')								//引号内部的部分
		{
 			i++;
			ch = Postffix[i];
			NFA newNFA(ch);
			nfaStack.push(newNFA);
			i++;
			ch = Postffix[i];
			while (ch != '\"')
			{
				NFA nfa0(ch),nfa1=nfaStack.top();
				nfaStack.pop();
				nfaStack.push(OpLink(nfa1,nfa0));
				i++;
				ch = Postffix[i];
			}
		}
		else
			while (ch != '$')
			{//顺序处理后缀表达式的每个字符
				if (ch == '.')
				{//操作符为'.'
					NFA nfa1, nfa2;//弹出栈顶的两个NFA
					nfa2 = nfaStack.top();
					nfaStack.pop();
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpLink(nfa1, nfa2));//将以‘.’连接好的大的NFA压入栈中
					ch = Postffix[++i];
				}
				else if (ch == '|')
				{//操作符为'|'
					NFA nfa1, nfa2;//弹出栈顶的两个NFA
					nfa2 = nfaStack.top();
					nfaStack.pop();
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpOR(nfa1, nfa2));//将以‘|’连接好的大的NFA压入栈中
					ch = Postffix[++i];
				}
				else if (ch == '*')
				{//操作符为'*'
					NFA nfa1;//弹出栈顶的1个NFA
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpStar(nfa1));//调用闭包函数Opstar，并将处理之后的NFA压入栈
					ch = Postffix[++i];
				}
				else
				{
					nfaStack.push(NFA(ch));
					if (ch == '9')
					{
						int i = 0;
					}
					ch = Postffix[++i];
				}
			}
		if (nfaStack.size() > 1)
		{
			cout << "创建NFA出现问题!正则表达式存在多段" << endl;
		}
		NFA returnNFA = nfaStack.top();
		returnNFA.insertF(returnNFA.tail, action);
		return returnNFA;
}


bool NFA::InsertPot(string &infix)//在中缀表达式中加入点'.'
{
	infix.append("#");//给中缀表达式添加结束符标志‘#’
	int size = infix.length();//获取中缀表达式的长度
	string t;//创建一个临时变量数组用来存放加入点之后的表达式
	char current = 0;//存当前取得的字符
	char next = 0;//当前字符的下一个字符
	int i;
	int Pre = 0;
	if (infix[0] == '\"')
	{

	}
	else
	{
		for (i = 0; i < size - 1; i++)
		{
			current = infix[i];
			next = infix[i + 1];
			t.append(1, current);
			if (current != '(' &&current != '*' && current != '|' && current != ')')
				Pre++;
			else if (current == '*' || current == ')')
				if ((next != '*' || next == ')') && Pre&&next != '#')
				{
					Pre--;
					t.append(1, '.');
				}
			if ((!(next == '*' || next == '|' || next == ')')) && next != '#')
			{
				if (Pre)
				{
					Pre--;
					t.append(1, '.');
				}
			}
			else if (next == '|')
				Pre--;
		}
		t.append(1, next);//添加最后一个字符
		infix.clear();//清空当前正规表达式(中缀表达式）
		infix = t;//将处理后的表达式赋给infix
	}
	return true;
}


bool NFA::IsletNumOp(char &ch)//判断当前读入的字符c是否是字母或者数字或者操作符
{
	if (isalpha(ch))//判断是否为字母
		return true;
	else if (isdigit(ch))//判断是否为数字
		return true;
	else if ( (ch == '+') || (ch == '-') || (ch == '/' )|| (ch == '=') )
		return true;
	else
		return false;


}

bool NFA::InfixToPostfix(string &Infix, string &Postfix)//中缀表达式转后缀表达式
{
	// 将正规表达式串(中缀表达式)Infix(以'#'作为结束标记)转换为存于Postfix后缀表达式
	if (Infix[0] == '\"')
	{
		Postfix = Infix.substr(0, Infix.size() - 1);
	}
	else
	{
		stack<char> opStack;//用于存运算符的栈
		opStack.push('#');  //将‘#’压入栈底
		int i = 0;   //i指示Infix中字符位置
		Postfix = "";//后缀表达式初始化为空
		for (int i = 0; i < Infix.length();)
		{
			if (Infix[i] == '#')//扫描到“#”即为处理结束
				break;
			else
			{
				if (Infix[i] == ' ' || Infix[i] == '\t' || Infix[i] == ';' )//空格不处理，直接进行扫描下一个
					i++;
				else if (Infix[i] == '(')
				{//当前扫描运算符为左括号，则直接压栈
					opStack.push(Infix[i]);
					i++;
				}
				else if (Infix[i] == ')')
				{//扫描到右括号')'，使括号内运算符依次出栈，并将非左括号的运算符写入Postfix中
					while (opStack.top() != '(')
					{
						Postfix.append(1, opStack.top());//将非左括号的运算符写入Postfix中
						opStack.pop();//使括号内运算符依次出栈出栈
					}
					opStack.pop();//左括号出栈
					i++;
				}
				else if (Infix[i] == '.' || Infix[i] == '|' || Infix[i] == '*')
				{
					while (priority(opStack.top()) >= priority(Infix[i]))
					{//若栈顶算符优先级大于当前算符优先级，出栈，并且写入到Postfix中
						Postfix.append(1, opStack.top());//写入到Postfix中
						opStack.pop();//出栈
					}
					opStack.push(Infix[i]);//将当前的算符压入栈中
					i++;
				}
				else
				{
					Postfix.append(1, Infix[i]);//操作数直接写入Postfix中
					i++;
				}
			}
		}
		
			//扫描结束，处理运算符栈中的剩余运算符
			while (opStack.top() != '#') //将栈中的元素全部弹出，写入Postfix中
			{
				Postfix.append(1,opStack.top());//写入Postfix中
				opStack.pop();//出栈
			}
			opStack.pop();//‘#’出栈
			Postfix.append("$");//以‘$’符号结束Postfix
			return true;
	}
}

int NFA::priority(char &ch)//识别操作符的优先级先后
{
	if (ch == '#')
		return 0;
	else if (ch == '.')
		return 1;
	else if (ch == '|')
		return 2;
	else if (ch == '*')
		return 3;
	else 
		return -1;
}



NFA NFA::OpLink(NFA n1, NFA n2)
{
	NFA newNFA;
	newNFA.S.pop_back();
	//对原状态集做位移
	int size = n1.S.size();
	n2.stateMove(size);
	newNFA.head = n1.head;
	newNFA.tail = n2.tail;
	//状态合并
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	newNFA.insert(n1.tail, n2.head, NULL);
	//合并字母表和终态集合
	map<int, string>::iterator it0 = n2.actionTB.begin();
	while (it0 != n2.actionTB.end())
	{
		n1.actionTB.insert(pair<int, string>(it0->first, it0->second));
		it0++;
	}
	newNFA.actionTB = n1.actionTB;
	set_union(n1.alphabet.begin(), n1.alphabet.end(), n2.alphabet.begin(), n2.alphabet.end(), back_inserter(newNFA.alphabet));
	set_union(n1.F.begin(), n1.F.end(), n2.F.begin(), n2.F.end(), back_inserter(newNFA.F));
	return newNFA;
}
NFA NFA::OpOR(NFA n1, NFA n2)
{
	NFA newNFA;
	//对原状态集做位移
	n1.stateMove(1);
	int size1 = n1.S.size(), size2 = n2.S.size(), size = size1 + size2 + 1;
	n2.stateMove(1 + size1);
	//状态合并
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	//初态插入
	newNFA.insert(0, n1.head, NULL);
	newNFA.insert(0, n2.head, NULL);
	newNFA.insert(n1.tail, size, NULL);
	newNFA.insert(n2.tail, size, NULL);
	newNFA.head = 0;
	newNFA.tail = size;
	//合并字母表和终态集合
	map<int, string>::iterator it0 = n2.actionTB.begin();
	while (it0 != n2.actionTB.end())
	{
		n1.actionTB.insert(pair<int, string>(it0->first, it0->second));
		it0++;
	}
	newNFA.actionTB = n1.actionTB;
	set_union(n1.alphabet.begin(), n1.alphabet.end(), n2.alphabet.begin(), n2.alphabet.end(), back_inserter(newNFA.alphabet));
	set_union(n1.F.begin(), n1.F.end(), n2.F.begin(), n2.F.end(), back_inserter(newNFA.F));
	return newNFA;
}

NFA NFA::OpStar(NFA n)
{
	n.insert(n.tail, n.head, NULL);
	n.tail = n.head;
	return n;
}
