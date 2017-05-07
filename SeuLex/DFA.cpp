#include "DFA.h"
#include <iostream>
#include <queue>
using namespace std;
DFA::DFA(NFA n)
{
	alphabet = n.alphabet;
	vector<char>::iterator itr;
	itr = find(alphabet.begin(), alphabet.end(), NULL);
	while (itr != alphabet.end())
	{
		alphabet.erase(itr);
		itr = find(alphabet.begin(), alphabet.end(), NULL);
	}
	//NFA��ȷ����
	vector<int> I = n.closure(0),newI;
	vector<vector<int>> setI;
	queue<vector<int>> waitI;
	setI.push_back(I);
	waitI.push(I);
	stateNum = 0;
	//�ȴ����в�Ϊ��ʱѭ���������ʾ����
	while (waitI.size())
	{
		map<char, int> jump;
		I = waitI.front();
		waitI.pop();
		for (int i = 0; i < alphabet.size(); i++)
		{
			newI = n.move(I, alphabet[i]);
			if (newI.size())//�ж��Ƿ��
			{
				int id = setI.size();
				//�ж��ظ�
				bool flag = true;
				for (int j = 0; j < id; j++)
				{
					if((setI[j].size()==newI.size())&&(setI[j] == newI))
					{
						flag = false;
						id = j;
						break;
					}
				}
				//������ظ�
				if (flag)
				{
					setI.push_back(newI);
					jump.insert(pair<char, int>(alphabet[i], setI.size() - 1));
					waitI.push(newI);
					//�ж��¼����Ƿ���̬
					int state = n.judgeF(newI);
					if (state)
					{
						F.push_back(setI.size() - 1);
						actionTB.insert(pair<int,string>(setI.size() - 1, n.actionTB[newI[state-1]]));
					}
				}
				//�ظ�ʱ
				else
					jump.insert(pair<char, int>(alphabet[i], id));
			}
		}
		stateNum++;
		sigma.push_back(jump);
		cout << waitI.size()<<endl;
	}
}

void DFA::DFAmin()
{
	vector<vector<int>> PI, PInew;
	vector<int>::iterator iter;
	vector<int> G;
	for (int i = 0; i < stateNum; i++)
	{
		if(find(F.begin(),F.end(),i)==F.end())
			G.push_back(i);
	}
	//��ʼ���ַ���̬����̬����
	PI.push_back(G);
	PI.push_back(F);
	PInew = PI;
	do {
		PI = PInew;
		for (int i = 0; i < PI.size(); i++)						//i��PI�ڵ�ǰ���ϵ�ָ��
		{
			vector<int>::iterator it;
			int shifting = PInew.size() - i;
			for (int j = 0; j < alphabet.size(); j++)
			{
				vector<int> end;
				int currentEnd;
				for (int k = 0; k < PInew[i].size();)
				{
					bool flag = true;
					if (sigma[PInew[i][k]].count(alphabet[j]) > 0)										//�ж��Ƿ��и���ת
					{
						currentEnd = sigma[PInew[i][k]].find(alphabet[j])->second;						//PInew[i][k]״̬alphabet[j]��ת��End
						for (int l = 0; l < PInew.size(); l++)											//��PInew��ÿ��������End
						{
							if (find(PInew[l].begin(), PInew[l].end(), currentEnd) != PInew[l].end())		//���End�ڼ���PInew[l]�У�˵�����µ�PInew���ϣ�PInew[i][k]״̬alphabet[j]��ת��EndΪl
							{
								currentEnd = l;
								break;
							}
						}
					}
					else																			//û�и���תʱ,��currentEndΪ-1
						currentEnd = -1;
					for (int l = 0; l < end.size(); l++)
						if (currentEnd == end[l])													//���End�Ѿ���end�����У���λ����L
						{
							flag = false;
							if (l)																	//����end�ĵ�һ������ǰ���ϴ��ڶ����ת�յ�,��Ŀ��ӵ�ǰ����ȡ���ŵ����еļ�����
							{
								vector<int> temp;
								temp.push_back(PInew[i][k]);
								PInew.push_back(temp);
								PInew[i].erase(PInew[i].begin() + k);
							}
							else																	//�ǵ�һ��������
								k++;
						}
					if (flag)																		//���End jump���µļ���
						if (end.size())																//������ǵ�һ����ת
						{
							G.clear();
							end.push_back(currentEnd);
							G.push_back(PInew[i][k]);
							PInew.push_back(G);
							PInew[i].erase(PInew[i].begin() + k);
						}
						else
						{
							end.push_back(currentEnd);
							k++;
						}
				}
			}
		}
	} while (PI.size() != PInew.size());
	//��������PI�����µ���ת��
	//ѡ������ÿ����λ��Ϊ�ü��ϵĴ���
	vector<map<char, int>>		sigmaNew;
	map<char, int>				newMap;
	for (int i = 0; i < PI.size(); i++)
	{
		newMap.clear();
		for (int l = 0; l < alphabet.size(); l++)
		{
			for (int j = 0; j < PI.size(); j++)
			{
				if ((sigma[PI[i][0]].count(alphabet[l]))&&(find(PI[j].begin(), PI[j].end(), sigma[PI[i][0]].find(alphabet[l])->second) != PI[j].end()))
				{
					newMap.insert(pair<char, int>(alphabet[l], j));
					break;
				}

			}
		}
		sigmaNew.push_back(newMap);
	}
	sigma = sigmaNew;

	//�������̬Ϊ�ȶ�ȡ���Ĺ������ȼ���
	sort(F.begin(), F.end());
	map<int, string> newActTB;
	for (int i = 0; i < F.size(); i++)
	{
		int currentPos=-1;
		for (int j = 0; j < PI.size(); j++)				//�ҵ���̬F[i]��ǰ��λ��
		{
			for (int l = 0; l < PI[j].size(); l++)
				if (PI[j][l] == F[i])
				{
					currentPos = j;
					break;
				}
			if (currentPos > -1)
				break;
		}
		map<int, string>::iterator it = actionTB.find(F[i]);
		string ch = it->second;
		if (newActTB.count(currentPos))
		{
			map<int, string>::iterator it0 = newActTB.find(currentPos);
			actionTB.erase(it0);						//�����ȼ����滻
		}
		actionTB.insert(pair<int, string>(currentPos, ch));
	}
}