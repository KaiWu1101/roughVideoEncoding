#include"downSample.h"

void downSample(short *srcImg, int imgSize, string mode,bool flag)
{
	if (flag)
	{
		if (mode == "420")
		{
			for (int i = 0; i < imgSize; i=i+2)
			{
				for (int j = 0; j < imgSize; j = j + 2)
				{
					/**(srcImg + (i + 1)*N * 3 + j * 3 + 0) = *(srcImg + i*N * 3 + j * 3 + 0);
					*(srcImg + i*N * 3 + (j + 1) * 3 + 0) = *(srcImg + i*N * 3 + j * 3 + 0);
					*(srcImg + (i + 1)*N * 3 + (j+1) * 3 + 0) = *(srcImg + i*N * 3 + j * 3 + 0);*/

					*(srcImg + (i + 1)*imgSize * 3 + j * 3 + 1) = *(srcImg + i*imgSize * 3 + j * 3 + 1);
					*(srcImg + i*imgSize * 3 + (j + 1) * 3 + 1) = *(srcImg + i*imgSize * 3 + j * 3 + 1);
					*(srcImg + (i + 1)*imgSize * 3 + (j + 1) * 3 + 1) = *(srcImg + i*imgSize * 3 + j * 3 + 1);

					*(srcImg + (i + 1)*imgSize * 3 + j * 3 + 2) = *(srcImg + i*imgSize * 3 + j * 3 + 2);
					*(srcImg + i*imgSize * 3 + (j + 1) * 3 + 2) = *(srcImg + i*imgSize * 3 + j * 3 + 2);
					*(srcImg + (i + 1)*imgSize * 3 + (j + 1) * 3 + 2) = *(srcImg + i*imgSize * 3 + j * 3 + 2);
				}
			}
		}
		else if (mode == "422")
		{
			for (int i = 0; i < imgSize; i = i + 1)
			{
				for (int j = 0; j < imgSize; j = j + 2)
				{
					/**(srcImg + i*N * 3 + (j + 1) * 3 + 0) = *(srcImg + i*N * 3 + j * 3 + 0);*/
					*(srcImg + i*imgSize * 3 + (j + 1) * 3 + 1) = *(srcImg + i*imgSize * 3 + j * 3 + 1);
					*(srcImg + i*imgSize * 3 + (j + 1) * 3 + 2) = *(srcImg + i*imgSize * 3 + j * 3 + 2);
				}
			}
		}
	}
}