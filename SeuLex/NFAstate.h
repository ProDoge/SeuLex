#pragma once
#include <vector>
using namespace std;
struct go {
	char E;								//边上的字符
	int end;							//跳转的状态
};
class NFAstate
{
private:
	int stateID;
	vector<go> edge;					//所有可能的出边
public:
	NFAstate(int ID) :stateID(ID) {};
	void insert(char E, int target);	//插入一条边
	void remove(char E, int target);	//删除一条边
	void stateMove(int steps);			//NFA合并时内部跳转状态的位移
	vector<int> nextState(char E);		//返回所有可能的下个状态
};
