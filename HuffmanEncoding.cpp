#include "HuffmanEncoding.h"
int htree1[] = { 97, 98, 99, 100, 101, 3, 2, 6, 1, 4, 8, 10, 0 };

char huffTreeTraverse(int huffCode[], int N)
{
	int loc = 3 * N - 3;
	int count = 0;
	do
	{
		int loc0 = loc;
		if (huffCode[count] == 0)
		{
			loc = htree1[loc0];
			count++;
		}
		else
		{
			loc = htree1[loc0 - 1];
			count++;
		}
	} while (loc >= N);
	return htree1[loc];
}