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
	//��ԭ״̬����λ��
	n1.stateMove(1);
	int size = n1.S.size();
	n2.stateMove(1 + size);
	//״̬�ϲ�
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	//�����
	newNFA.insert(0, 1, NULL);
	newNFA.insert(0, size + 1, NULL);
	//�ϲ���ĸ�����̬����
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
	//ÿ�ΰ��µ�epsilon�߼ӽ���
	do {
		size = I.size();
		for (int i = 0; i < size; i++)
		{
			//I[i]״̬��epsilon��
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
	//ÿ�ΰ��µ�ch�߼ӽ���
	for (int i = 0; i < size; i++)
	{
		//I[i]״̬��epsilon��
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
		string Postffix;//���׺���ʽ
		this->InsertPot(Infix);//����׺���ʽ�м����'.'
		this->InfixToPostfix(Infix, Postffix);//����׺���ʽתΪ��׺���ʽ 
		stack<NFA> nfaStack; //�����ݴ��м�����ɵ�NFAջ
		int i = 0;   //����ָʾɨ��Postffix�����ַ���λ��
		char ch;
		ch = Postffix[i];//ch����Postffix��ɨ�赽���ַ�
		if (ch == '\"')								//�����ڲ��Ĳ���
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
			{//˳�����׺���ʽ��ÿ���ַ�
				if (ch == '.')
				{//������Ϊ'.'
					NFA nfa1, nfa2;//����ջ��������NFA
					nfa2 = nfaStack.top();
					nfaStack.pop();
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpLink(nfa1, nfa2));//���ԡ�.�����ӺõĴ��NFAѹ��ջ��
					ch = Postffix[++i];
				}
				else if (ch == '|')
				{//������Ϊ'|'
					NFA nfa1, nfa2;//����ջ��������NFA
					nfa2 = nfaStack.top();
					nfaStack.pop();
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpOR(nfa1, nfa2));//���ԡ�|�����ӺõĴ��NFAѹ��ջ��
					ch = Postffix[++i];
				}
				else if (ch == '*')
				{//������Ϊ'*'
					NFA nfa1;//����ջ����1��NFA
					nfa1 = nfaStack.top();
					nfaStack.pop();
					nfaStack.push(OpStar(nfa1));//���ñհ�����Opstar����������֮���NFAѹ��ջ
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
			cout << "����NFA��������!������ʽ���ڶ��" << endl;
		}
		NFA returnNFA = nfaStack.top();
		returnNFA.insertF(returnNFA.tail, action);
		return returnNFA;
}


bool NFA::InsertPot(string &infix)//����׺���ʽ�м����'.'
{
	infix.append("#");//����׺���ʽ��ӽ�������־��#��
	int size = infix.length();//��ȡ��׺���ʽ�ĳ���
	string t;//����һ����ʱ��������������ż����֮��ı��ʽ
	char current = 0;//�浱ǰȡ�õ��ַ�
	char next = 0;//��ǰ�ַ�����һ���ַ�
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
		t.append(1, next);//������һ���ַ�
		infix.clear();//��յ�ǰ������ʽ(��׺���ʽ��
		infix = t;//�������ı��ʽ����infix
	}
	return true;
}


bool NFA::IsletNumOp(char &ch)//�жϵ�ǰ������ַ�c�Ƿ�����ĸ�������ֻ��߲�����
{
	if (isalpha(ch))//�ж��Ƿ�Ϊ��ĸ
		return true;
	else if (isdigit(ch))//�ж��Ƿ�Ϊ����
		return true;
	else if ( (ch == '+') || (ch == '-') || (ch == '/' )|| (ch == '=') )
		return true;
	else
		return false;


}

bool NFA::InfixToPostfix(string &Infix, string &Postfix)//��׺���ʽת��׺���ʽ
{
	// ��������ʽ��(��׺���ʽ)Infix(��'#'��Ϊ�������)ת��Ϊ����Postfix��׺���ʽ
	if (Infix[0] == '\"')
	{
		Postfix = Infix.substr(0, Infix.size() - 1);
	}
	else
	{
		stack<char> opStack;//���ڴ��������ջ
		opStack.push('#');  //����#��ѹ��ջ��
		int i = 0;   //iָʾInfix���ַ�λ��
		Postfix = "";//��׺���ʽ��ʼ��Ϊ��
		for (int i = 0; i < Infix.length();)
		{
			if (Infix[i] == '#')//ɨ�赽��#����Ϊ�������
				break;
			else
			{
				if (Infix[i] == ' ' || Infix[i] == '\t' || Infix[i] == ';' )//�ո񲻴���ֱ�ӽ���ɨ����һ��
					i++;
				else if (Infix[i] == '(')
				{//��ǰɨ�������Ϊ�����ţ���ֱ��ѹջ
					opStack.push(Infix[i]);
					i++;
				}
				else if (Infix[i] == ')')
				{//ɨ�赽������')'��ʹ��������������γ�ջ�������������ŵ������д��Postfix��
					while (opStack.top() != '(')
					{
						Postfix.append(1, opStack.top());//���������ŵ������д��Postfix��
						opStack.pop();//ʹ��������������γ�ջ��ջ
					}
					opStack.pop();//�����ų�ջ
					i++;
				}
				else if (Infix[i] == '.' || Infix[i] == '|' || Infix[i] == '*')
				{
					while (priority(opStack.top()) >= priority(Infix[i]))
					{//��ջ��������ȼ����ڵ�ǰ������ȼ�����ջ������д�뵽Postfix��
						Postfix.append(1, opStack.top());//д�뵽Postfix��
						opStack.pop();//��ջ
					}
					opStack.push(Infix[i]);//����ǰ�����ѹ��ջ��
					i++;
				}
				else
				{
					Postfix.append(1, Infix[i]);//������ֱ��д��Postfix��
					i++;
				}
			}
		}
		
			//ɨ����������������ջ�е�ʣ�������
			while (opStack.top() != '#') //��ջ�е�Ԫ��ȫ��������д��Postfix��
			{
				Postfix.append(1,opStack.top());//д��Postfix��
				opStack.pop();//��ջ
			}
			opStack.pop();//��#����ջ
			Postfix.append("$");//�ԡ�$�����Ž���Postfix
			return true;
	}
}

int NFA::priority(char &ch)//ʶ������������ȼ��Ⱥ�
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
	//��ԭ״̬����λ��
	int size = n1.S.size();
	n2.stateMove(size);
	newNFA.head = n1.head;
	newNFA.tail = n2.tail;
	//״̬�ϲ�
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	newNFA.insert(n1.tail, n2.head, NULL);
	//�ϲ���ĸ�����̬����
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
	//��ԭ״̬����λ��
	n1.stateMove(1);
	int size1 = n1.S.size(), size2 = n2.S.size(), size = size1 + size2 + 1;
	n2.stateMove(1 + size1);
	//״̬�ϲ�
	newNFA.S.insert(newNFA.S.end(), n1.S.begin(), n1.S.end());
	newNFA.S.insert(newNFA.S.end(), n2.S.begin(), n2.S.end());
	//��̬����
	newNFA.insert(0, n1.head, NULL);
	newNFA.insert(0, n2.head, NULL);
	newNFA.insert(n1.tail, size, NULL);
	newNFA.insert(n2.tail, size, NULL);
	newNFA.head = 0;
	newNFA.tail = size;
	//�ϲ���ĸ�����̬����
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
