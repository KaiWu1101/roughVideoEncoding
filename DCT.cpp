#include"DCT.h"
#define PI 3.1415926
void DCT(short* srcImg,int imgSize, int N)
{

	for (int i = 0; i < imgSize; i = i + N)
	{
		for (int j = 0; j < imgSize; j = j + N)
		{
			DCT_macroblock(srcImg + i*imgSize * 3 + j * 3 + 0, imgSize);
			//print_DCT(srcImg + i*imgSize * 3 + j * 3 + 0, imgSize);
		}
	}
}

void IDCT(short* srcImg,int imgSize, int N)
{

	for (int i = 0; i < imgSize; i = i + N)
	{
		for (int j = 0; j < imgSize; j = j + N)
		{
			IDCT_macroblock(srcImg + i*imgSize * 3 + j * 3 + 0, imgSize);
		}
	}
}

void DCT_macroblock(short* blockP,int imgSize)
{
	short  temp[8][8][3];
	//short* temp = &tempA[0][0][0];
	double a[32], sum, coef; 
	short i, j, u, v;
	int N = 8;
	double sumY = 0;  
	/*here sumY must be double, because it could be very small number like 0.00003,
	pay attentation to type conversion.*/
	double sumU = 0;
	double sumV = 0;
	a[0] = sqrt(1.0 / N);
	for (i = 1; i < N; ++i) {
		a[i] = sqrt(2.0 / N);
	}
	for (u = 0; u < N; ++u) {
		for (v = 0; v < N; ++v) {
			sumY = 0.0;
			sumU = 0.0;
			sumV = 0.0;
			for (i = 0; i < N; ++i)
			{
				for (j = 0; j < N; ++j)
				{
					coef = cos((2 * i + 1)*u*PI / (2 * N)) * cos((2 * j + 1)*v*PI / (2 * N));
					sumY += *(blockP + i*imgSize * 3 + j * 3 + 0) * coef; 		//f[i][j] * coef
					sumU += *(blockP + i*imgSize * 3 + j * 3 + 1)* coef;
					sumV += *(blockP + i*imgSize * 3 + j * 3 + 2)* coef;
				} //for j
			} //for i
			temp[u][v][0] = a[u] * a[v] * sumY;
			temp[u][v][1] = a[u] * a[v] * sumU;
			temp[u][v][2] = a[u] * a[v] * sumV;
		} //for u
	} //for v


	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			*(blockP + i*imgSize * 3 + j * 3 + 0) = temp[i][j][0];
			*(blockP + i*imgSize * 3 + j * 3 + 1) = temp[i][j][1];
			*(blockP + i*imgSize * 3 + j * 3 + 2) = temp[i][j][2];
			
		}
	}

}


void IDCT_macroblock(short* blockP,int imgSize)
{
	short  temp[8][8][3];
	int N = 8;
	double a[32], sum, coef;
	short i, j, u, v;
	double sumY = 0.0;
	double sumU = 0.0;
	double sumV = 0.0;

	a[0] = sqrt(1.0 / N);
	for (i = 1; i < N; ++i) {
		a[i] = sqrt(2.0 / N);
	}
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			sumY = 0;
			sumU = 0;
			sumV = 0;
			for (u = 0; u < N; ++u)
			{
				for (v = 0; v < N; ++v)
				{
					coef = cos((2 * j + 1)*v*PI / (2 * N)) * cos((2 * i + 1)*u*PI / (2 * N));
					sumY += a[u] * a[v] * (*(blockP + u*imgSize * 3 + v * 3 + 0)) * coef; 	//a[u] * a[v] * F[u][v] * coef
					sumU += a[u] * a[v] * (*(blockP + u*imgSize * 3 + v * 3 + 1)) * coef;
					sumV += a[u] * a[v] * (*(blockP + u*imgSize * 3 + v * 3 + 2)) * coef;

				} //for j
			} //for i
			temp[i][j][0] = sumY;
			temp[i][j][1] = sumU;
			temp[i][j][2] = sumV;
		} //for u
	} //for v

	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			*(blockP + i*imgSize * 3 + j * 3 + 0) = temp[i][j][0];
			*(blockP + i*imgSize * 3 + j * 3 + 1) = temp[i][j][1];
			*(blockP + i*imgSize * 3 + j * 3 + 2) = temp[i][j][2];

		}
	}

}

void print_DCT(short* blockP, int imgSize)
{
	for (int i = 0; i < 8; ++i) {
		cout << endl;
		for (int j = 0; j < 8; ++j) {
			cout << *(blockP + i*imgSize * 3 + j * 3 + 0) << "\t";
		}
	}
}