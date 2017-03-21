#include"quantization.h"

quantizer::quantizer(short Qstep)
{
	this->Qstep = Qstep;
}

void quantizer::quantization(short* inputImg, int imgSize)
{
	short* Y;
	short* Cb;
	short* Cr;
	for (int i = 0; i < imgSize; i++)
	{
		for (int j = 0; j < imgSize; j++)
		{
			Y  = (inputImg + i*imgSize * 3 + j * 3 + 0);
			Cb = (inputImg + i*imgSize * 3 + j * 3 + 1);
			Cr = (inputImg + i*imgSize * 3 + j * 3 + 2);
			*Y = (short)round((double)(*Y) / Qstep);
			*Cb = (short)round((double)(*Cb) / Qstep);
			*Cr = (short)round((double)(*Cr) / Qstep);


		}
	}
}

void quantizer::reverse_quantization(short* inputImg, int imgSize)
{
	short* Y;
	short* Cb;
	short* Cr;
	for (int i = 0; i < imgSize; i++)
	{
		for (int j = 0; j < imgSize; j++)
		{
			Y  = (inputImg + i*imgSize * 3 + j * 3 + 0);
			Cb = (inputImg + i*imgSize * 3 + j * 3 + 1);
			Cr = (inputImg + i*imgSize * 3 + j * 3 + 2);
			*Y  = (short)((*Y)  * Qstep);
			*Cb = (short)((*Cb) * Qstep);
			*Cr = (short)((*Cr) * Qstep);
		}
	}
}