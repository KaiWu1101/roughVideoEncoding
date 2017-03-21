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
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					short* pixelPointerY = blockPointer + i*imgSize + j + 0;
					if (*(pixelPointerY) == 0) 
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].level = *(pixelPointerY);
					runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].last = 0;
					run_length = 0;
					k++;
				}
			}
			if (k > 0)
				runs[(k - 1) * 3 + blockNum*N*N * 3 + 0 * N*N].last = 1;       //last nonzero element
			else {                      //whole block 0
				runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].run = 64;
				runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].level = 0;
				runs[k * 3 + blockNum*N*N * 3 + 0 * N*N].last = 1;         //this needs to be 1 to terminate
			}
			/////////////////////////////////////////////////////////
			//short* blockPointer = srcImg + i*imgSize * 3 + j * 3 + 0;
			run_length = 0, k = 0;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					short* pixelPointerY = blockPointer + i*imgSize + j + 0;
					if (*(pixelPointerY+1) == 0)
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 1].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 1].level = *(pixelPointerY);
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
					short* pixelPointerY = blockPointer + i*imgSize + j + 0;
					if (*(pixelPointerY+2) == 0)
					{
						run_length++;
						continue;
					}
					runs[k * 3 + blockNum*N*N * 3 + 2].run = run_length;
					runs[k * 3 + blockNum*N*N * 3 + 2].level = *(pixelPointerY);
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
	for (int i = 0; i < imgSize; i = i + N)
	{
		for (int j = 0; j < imgSize; j = j + N)
		{
			short* blockPointer = outputImg + i*imgSize * 3 + j * 3 + 0;
			int i1, j1, r, k = 0, n = 0;
			while (n < 64)
			{
				for (r = 0; r < runs[k*3 + blockNum*N*N * 3 + 0].run; r++)
				{
					i1 = n / 8;
					j1 = n % 8;
					*(blockPointer+i1*imgSize*3+j1*3+0) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n / 8;
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
				i1 = n / 8;
				j1 = n % 8;
				*(blockPointer + i1*imgSize * 3 + j1 * 3 + 0) = 0;
				n++;
			}

		//////////////////////////////////////////////
			k = 0, n = 0;
			while (n < 64)
			{
				for (r = 0; r < runs[k * 3 + blockNum*N*N * 3 + 1].run; r++)
				{
					i1 = n / 8;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 1) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n / 8;
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
				i1 = n / 8;
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
					i1 = n / 8;
					j1 = n % 8;
					*(blockPointer + i1*imgSize * 3 + j1 * 3 + 2) = 0;
					n++;
				}
				if (n < 64)
				{
					i1 = n / 8;
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
				i1 = n / 8;
				j1 = n % 8;
				*(blockPointer + i1*imgSize * 3 + j1 * 3 + 2) = 0;
				n++;
			}
			blockNum++;
		}
	}
}