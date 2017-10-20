#include "RLE.h"

void RUN::run_block(short* srcImg, int imgSize, int N, RUN3D runs[])

{
	int blockNum = 0;
	for (int i = 0; i < imgSize; i = i + N)
	{
		for (int j = 0; j < imgSize; j = j + N)
		{
			short* blockPointer = srcImg + i*imgSize * 3 + j * 3 + 0;
			
			unsigned char run_length = 0, k = 0;
			for (int i = 0; i < 8; i++) 
			{
				cout << endl;
				for (int j = 0; j < 8; j++) 
				{
					short* pixelPointerY = blockPointer + i*imgSize*3 + j*3 + 0;
					cout << *(pixelPointerY) << "\t";
					if (*(pixelPointerY) == 0) 
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 0 ].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 0 ].level = *(pixelPointerY);
					runs[k * 3 + blockNum*N*N * 3 + 0 ].last = 0;
					run_length = 0;
					k++;
				}
			}
			if (k > 0)
				runs[(k - 1) * 3 + blockNum*N*N * 3 + 0 * N*N].last = 1;       //last nonzero element
			else {                      //whole block 0
				runs[k * 3 + blockNum*N*N * 3 + 0 ].run = 64;
				runs[k * 3 + blockNum*N*N * 3 + 0 ].level = 0;
				runs[k * 3 + blockNum*N*N * 3 + 0 ].last = 1;         //this needs to be 1 to terminate
			}
			/////////////////////////////////////////////////////////
			//short* blockPointer = srcImg + i*imgSize * 3 + j * 3 + 0;
			run_length = 0, k = 0;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					short* pixelPointerU = blockPointer + i*imgSize * 3 + j * 3 + 1;
					if (*(pixelPointerU) == 0)
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 1].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 1].level = *(pixelPointerU);
					runs[k * 3 + blockNum*N*N * 3 + 1].last = 0;
					run_length = 0;
					k++;
				}
			}
			if (k > 0)
				runs[(k - 1) * 3 + blockNum*N*N * 3 + 1].last = 1;       //last nonzero element
			else {                      //whole block 0
				runs[k * 3 + blockNum*N*N * 3 + 1].run = 64;
				runs[k * 3 + blockNum*N*N * 3 + 1].level = 0;
				runs[k * 3 + blockNum*N*N * 3 + 1].last = 1;         //this needs to be 1 to terminate
			}
			///////////////////////////////////////////////////////////
			//short* blockPointer = srcImg + i*imgSize * 3 + j * 3 + 0;
			run_length = 0, k = 0;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					short* pixelPointerV = blockPointer + i*imgSize * 3 + j * 3 + 2;
					if (*(pixelPointerV) == 0)
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 2].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 2].level = *(pixelPointerV);
					runs[k * 3 + blockNum*N*N * 3 + 2].last = 0;
					run_length = 0;
					k++;
				}
			}
			if (k > 0)
				runs[(k - 1) * 3 + blockNum*N*N * 3 + 2].last = 1;       //last nonzero element
			else {                      //whole block 0
				runs[k * 3 + blockNum*N*N * 3 + 2].run = 64;
				runs[k * 3 + blockNum*N*N * 3 + 2].level = 0;
				runs[k * 3 + blockNum*N*N * 3 + 2].last = 1;         //this needs to be 1 to terminate
			}
			blockNum++;
		}
	}
}

void RUN::run_decode(RUN3D runs[], int imgSize, int N, short* outputImg)
{
	int blockNum = 0;
	system("clc");
	for (int i = 0; i < imgSize; i = i + N)
	{
		for (int j = 0; j < imgSize; j = j + N)
		{
			short* blockPointer = outputImg + i*imgSize * 3 + j * 3 + 0;
			int i1, j1, r, k = 0, n = 0;
			while (n < 64)
			{
				cout << "(" << runs[k * 3 + blockNum*N*N * 3 + 0].run - NULL << "\t";
				cout << runs[k * 3 + blockNum*N*N * 3 +0].level << "\t";
				cout << runs[k * 3 + blockNum*N*N * 3 +0].last - NULL << "£©" << endl;
				for (r = 0; r < runs[k*3 + blockNum*N*N * 3 + 0].run; r++)
				{
					i1 = n >> 3; //n>>3==n/8
					j1 = n % 8;
					*(blockPointer+i1*imgSize*3+j1*3+0) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n >>3;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 0) =
						runs[k * 3 + blockNum*N*N * 3 + 0].level;
					n++;
				}
				if (runs[k * 3 + blockNum*N*N * 3 + 0].last != 0) break;
				k++;
				
			}

			//run of 0s to end
			while (n < 64)
			{
				i1 = n >> 3;
				j1 = n % 8;
				*(blockPointer + i1*imgSize * 3 + j1 * 3 + 0) = 0;
				n++;
			}

			for (int i = 0; i < 8; i = i + 1)
			{
				cout << endl;
				for (int j = 0; j < 8; j = j + 1)
				{
					cout << *(blockPointer + i*imgSize * 3 + j * 3 + 0) << "\t";
				}
			}
		//////////////////////////////////////////////
			k = 0, n = 0;
			while (n < 64)
			{
				
				for (r = 0; r < runs[k * 3 + blockNum*N*N * 3 + 1].run; r++)
				{
					i1 = n >> 3;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 1) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n >> 3;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 1) =
						runs[k * 3 + blockNum*N*N * 3 + 1].level;
					n++;
				}
				if (runs[k * 3 + blockNum*N*N * 3 + 1].last != 0) break;
				k++;
			}

			//run of 0s to end
			while (n < 64)
			{
				i1 = n >> 3;
				j1 = n % 8;
				*(blockPointer + i1*imgSize * 3 + j1 * 3 + 1) = 0;
				n++;
			}
		////////////////////////////////////
			k = 0, n = 0;
			while (n < 64)
			{
				for (r = 0; r < runs[k * 3 + blockNum*N*N * 3 + 2].run; r++)
				{
					i1 = n >> 3;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 2) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n >> 3;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 2) =
						runs[k * 3 + blockNum*N*N * 3 + 2].level;
					n++;
				}
				if (runs[k * 3 + blockNum*N*N * 3 + 2].last != 0) break;
				k++;
			}

			//run of 0s to end
			while (n < 64)
			{
				i1 = n >> 3;
				j1 = n % 8;
				*(blockPointer + i1*imgSize * 3 + j1 * 3 + 2) = 0;
				n++;
			}
			blockNum++;
		}
	}
}