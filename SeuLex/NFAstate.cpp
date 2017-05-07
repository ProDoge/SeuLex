//version 1.0
#include "NFAstate.h"
#include <algorithm>
void NFAstate::insert(char E, int target)
{
	vector<go>::iterator iter = edge.begin();
	while (iter != edge.end())
	{
		if ((E == iter->E) && (target == iter->end))
			return;
		iter++;
	}
	go x = { E,target };
	edge.push_back(x);
}

void NFAstate::remove(char E, int target)
{
	vector<go>::iterator iter = edge.begin();
	while (iter != edge.end())
	{
		if ((E == iter->E) && (target == iter->end))
		{
			iter = edge.erase(iter);
			break;
		}
		iter++;
	}
}

void NFAstate::stateMove(int steps)
{
	stateID += steps;
	for (int i = 0; i < edge.size(); i++)
		edge[i].end += steps;

}

vector<int> NFAstate::nextState(char E)
{
	vector<int> next;
	vector<go>::iterator iter = edge.begin();
	while (iter != edge.end())
	{
		if (E == iter->E)
			next.push_back(iter->end);
		iter++;
	}
	sort(next.begin(), next.end());
	return next;
}