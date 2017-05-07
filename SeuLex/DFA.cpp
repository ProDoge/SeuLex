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
	//NFA的确定化
	vector<int> I = n.closure(0),newI;
	vector<vector<int>> setI;
	queue<vector<int>> waitI;
	setI.push_back(I);
	waitI.push(I);
	stateNum = 0;
	//等待队列不为空时循环，否则表示结束
	while (waitI.size())
	{
		map<char, int> jump;
		I = waitI.front();
		waitI.pop();
		for (int i = 0; i < alphabet.size(); i++)
		{
			newI = n.move(I, alphabet[i]);
			if (newI.size())//判断是否空
			{
				int id = setI.size();
				//判断重复
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
				//如果不重复
				if (flag)
				{
					setI.push_back(newI);
					jump.insert(pair<char, int>(alphabet[i], setI.size() - 1));
					waitI.push(newI);
					//判断新集合是否终态
					int state = n.judgeF(newI);
					if (state)
					{
						F.push_back(setI.size() - 1);
						actionTB.insert(pair<int,string>(setI.size() - 1, n.actionTB[newI[state-1]]));
					}
				}
				//重复时
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
	//初始化分非终态和终态集合
	PI.push_back(G);
	PI.push_back(F);
	PInew = PI;
	do {
		PI = PInew;
		for (int i = 0; i < PI.size(); i++)						//i是PI内当前集合的指针
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
					if (sigma[PInew[i][k]].count(alphabet[j]) > 0)										//判断是否有该跳转
					{
						currentEnd = sigma[PInew[i][k]].find(alphabet[j])->second;						//PInew[i][k]状态alphabet[j]跳转的End
						for (int l = 0; l < PInew.size(); l++)											//对PInew中每个集合找End
						{
							if (find(PInew[l].begin(), PInew[l].end(), currentEnd) != PInew[l].end())		//如果End在集合PInew[l]中，说明对新的PInew集合，PInew[i][k]状态alphabet[j]跳转的End为l
							{
								currentEnd = l;
								break;
							}
						}
					}
					else																			//没有该跳转时,令currentEnd为-1
						currentEnd = -1;
					for (int l = 0; l < end.size(); l++)
						if (currentEnd == end[l])													//如果End已经在end容器中，且位置是L
						{
							flag = false;
							if (l)																	//不是end的第一个即当前集合存在多个跳转终点,把目标从当前集合取出放到已有的集合中
							{
								vector<int> temp;
								temp.push_back(PInew[i][k]);
								PInew.push_back(temp);
								PInew[i].erase(PInew[i].begin() + k);
							}
							else																	//是第一个，不变
								k++;
						}
					if (flag)																		//如果End jump到新的集合
						if (end.size())																//如果不是第一个跳转
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
	//下面利用PI集做新的跳转表
	//选择任意每行首位作为该集合的代表
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

	//高序号终态为先读取到的规则，优先级高
	sort(F.begin(), F.end());
	map<int, string> newActTB;
	for (int i = 0; i < F.size(); i++)
	{
		int currentPos=-1;
		for (int j = 0; j < PI.size(); j++)				//找到终态F[i]当前的位置
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
			actionTB.erase(it0);						//高优先级的替换
		}
		actionTB.insert(pair<int, string>(currentPos, ch));
	}
}