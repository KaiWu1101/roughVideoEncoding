#include<iostream>
using namespace std;


class reorder
{
private:
	int zigzag[64];

public:
	reorder();
	reorder(int reorder_position[]);
	void reorder_8(short* block, short* blockR,int imgSize);
	void reverse_reorder_8(short* blockR, short* block,int imgSize);
};
