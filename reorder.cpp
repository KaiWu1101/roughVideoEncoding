#include "reorder.h"

reorder:: reorder()
{
	int reorder_position[] =
	{
		0, 1, 8, 16, 9, 2, 3, 10,
		17, 24, 32, 25, 18, 11, 4, 5,
		12, 19, 26, 33, 40, 48, 41, 34,
		27, 20, 13, 6, 7, 14, 21, 28,
		35, 42, 49, 56, 57, 50, 43, 36,
		29, 22, 15, 23, 30, 37, 44, 51,
		58, 59, 52, 45, 38, 31, 39, 46,
		53, 60, 61, 54, 47, 55, 62, 63

	};

	for (int i = 0; i < 64; i++)
	{
		zigzag[i] = reorder_position[i];
	}
}

reorder::reorder(int reorder_positions[])
{
	for (int i = 0; i < 64; i++)
		zigzag[i] = reorder_positions[i];
}

void reorder::reorder_8(short* block, short* blockR,int imgSize)
{
	for (int i = 0; i < imgSize; i += 8)
	{
		for (int j = 0; j < imgSize; j += 8)
		{
			short *position=block + i*imgSize * 3 + j * 3 + 0;
			short *positionR = blockR + i*imgSize * 3 + j * 3 + 0;
			int k = 0;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					int i1 = zigzag[k] / 8;
					int j1 = zigzag[k] % 8;
					*(positionR + i*imgSize * 3 + j * 3 + 0) = *(position + i1*imgSize * 3 + j1 * 3 + 0);
					*(positionR + i*imgSize * 3 + j * 3 + 1) = *(position + i1*imgSize * 3 + j1 * 3 + 1);
					*(positionR + i*imgSize * 3 + j * 3 + 2) = *(position + i1*imgSize * 3 + j1 * 3 + 2);
					k++;
				}
			}

		}

	}
}

void reorder::reverse_reorder_8(short* blockR, short* block, int imgSize)
{
	for (int i = 0; i < imgSize; i += 8)
	{
		for (int j = 0; j < imgSize; j += 8)
		{
			short *positionR = blockR + i*imgSize * 3 + j * 3 + 0;
			short *position = block + i*imgSize * 3 + j * 3 + 0;
			int k = 0;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					int i1 = zigzag[k] / 8;
					int j1 = zigzag[k] % 8;
					*(position + i1*imgSize * 3 + j1 * 3 + 0) = *(positionR + i*imgSize * 3 + j * 3 + 0);
					*(position + i1*imgSize * 3 + j1 * 3 + 1) = *(positionR + i*imgSize * 3 + j * 3 + 1);
					*(position + i1*imgSize * 3 + j1 * 3 + 2) = *(positionR + i*imgSize * 3 + j * 3 + 2);
					k++;
				}
			}

		}

	}
}