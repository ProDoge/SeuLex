#pragma once
#include <vector>
using namespace std;
struct go {
	char E;								//���ϵ��ַ�
	int end;							//��ת��״̬
};
class NFAstate
{
private:
	int stateID;
	vector<go> edge;					//���п��ܵĳ���
public:
	NFAstate(int ID) :stateID(ID) {};
	void insert(char E, int target);	//����һ����
	void remove(char E, int target);	//ɾ��һ����
	void stateMove(int steps);			//NFA�ϲ�ʱ�ڲ���ת״̬��λ��
	vector<int> nextState(char E);		//�������п��ܵ��¸�״̬
};
