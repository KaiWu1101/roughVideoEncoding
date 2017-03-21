#include<iostream>
using namespace std;

class quantizer 
{
private:
	short Qstep;
public:
	quantizer(short Qstep=1);
	void quantization(short* inputImg, int imgSize);
	void reverse_quantization(short* inputImg, int imgSize);
};