#ifndef _RLE_H
#define _RLE_H
#include<iostream>
using namespace std;
class RUN3D
{
public:
	unsigned char run;
	short level;
	char last;
};

class RUN
{
public:
	void run_block(short* srcImg, int imgSize, int N, RUN3D runs[]);
	void run_decode(RUN3D runs[], int imgSize, int N, short* outputImg);
};

#endif