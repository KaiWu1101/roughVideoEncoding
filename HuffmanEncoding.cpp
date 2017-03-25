#include "HuffmanEncoding.h"


//13 entries for this huffman table;
int htree_13[] = { 97, 98, 99, 100, 101, 3, 2, 6, 1, 4, 8, 10, 0 };
char huffTreeTraverse_13(int huffCode[], int N)
{
	int loc = 3 * N - 3;
	int count = 0;
	do
	{
		int loc0 = loc;
		if (huffCode[count] == 0)
		{
			loc = htree_13[loc0];
			count++;
		}
		else
		{
			loc = htree_13[loc0 - 1];
			count++;
		}
	} while (loc >= N);
	return htree_13[loc];
}

short htree_4[] = { 0x0203, 0x0105, 0x0604, 0x0007 };

char huffTreeTraverse_4(int huffCode[],int N)
{
	unsigned short left_mask = 0xFF00;
	unsigned short right_mask = 0x00FF;
	int count = 0;
	unsigned short loc = (N - 1) + N - 1;
	do
	{
		int loc0 = loc - N;
		if (huffCode[count] == 0)
		{
			loc = (htree_4[loc0] & left_mask)>> 8;
		}
		else
			loc = htree_4[loc0] & right_mask;
		count++;
	} while (loc >= N);
	switch (loc)
	{
		case 0: loc = 'a'; break;
		case 1: loc = 'b'; break;
		case 2: loc = 'b'; break;
		case 3: loc = 'd'; break;
		case 4: loc = 'e'; break;
		default: cout << "wrong encoded huffman code" << endl;
			exit(1);

	}
	return loc;
}

void build_htable(set<RunHuff> &htable)
{
	//N is the number of precalculated run-level Huffman codeword;
	int i, j, k, N = 10;

	//length of Huffman codewords (not include sigh bit)
	char len[] = { 2, 3, 4, 4, 4, 5, 5, 5, 6, 7 };

	//huffman codeword, 0x60 is the escape code
	unsigned short hcode[] = { 0x01, 0x3, 0x7, 0xf, 0xe, 0x16, 0x6, 0x1a, 0x2a, 0x60 };

	//value of runs in run-level, 255 signifies escape code (ESC)
	unsigned char runs[] = { 0, 1, 2, 0, 0, 3, 4, 5, 0, 255 };

	//value of levels in run-level,
	short levels[] = { 1, 1, 1, 2, 1, 1, 1, 1, 3, 0 };

	//vaules of lasts
	unsigned char lasts[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };

	RUN3D r;

	RunHuff rf[128];
	k = 0; j = 0;
	for (i = 0; i < N; ++i)
	{
		r.run = runs[i];
		r.level = levels[i];
		r.last = lasts[i];

		//RunHuff objects, sign-bit=0
		rf[k++] = RunHuff(r, hcode[i]<<1, len[i], j++);
		r.level = -r.level;
		rf[k++] = RunHuff(r, hcode[i]<<1 | 1, len[i], j++);
	}
	k= 2 * N;
	for (i = 0; i < k; i++)
	{
		htable.insert(rf[i]);
	}
}

void huff_encode(set<RunHuff> &htable, RUN3D runs[], BitFileIO &outputs)
{
	short i, j, k;
	k = 0;
	set<RunHuff>::iterator itr;

	i = 0;
	while (i < 64) // a macroblock has at most 64 samples;
	{
		RunHuff rf(runs[k], 0, 0, 0);

		if (itr == htable.find(rf) && itr != htable.end())
			outputs.outputBits(itr->codeword, itr->hlen + 1);
		else
			escape_encode(outputs, rf.r);

		if (runs[k].last) break;
		i += (runs[k].run + 1);  //+1 because the number of elements=zeros(run)+level;
		k++;
	}
}

void escape_encode(BitFileIO &outputs, RUN3D &r)
{
	if (r.level < 0)
	{
		outputs.outputBit(1);
		r.level = -r.level;
	}
	else
	{
		outputs.outputBit(0);
	}
	outputs.outputBits(0x60, 7);
	if (r.run == 64)
	{
		r.run = 63;  //check RLE.cpp,  63 zeros plus 1 level(0)=64 elements
	}
	outputs.outputBits(r.run, 6);
	outputs.outputBits(r.level, 8);
	outputs.outputBits(r.last,1);
}

void build_huff_tree(set<RunHuff> &htable, Dtables &d)
{
	set<RunHuff>::iterator itr;
	short i, j, n0, free_slot, loc, loc0, root, ntotal;
	unsigned int mask, hcode;
	n0 = NSymbols;
	ntotal = 2 * n0 - 1;
	root = 3 * n0 - 3; //=2*(n0-1)+n0 , because it starts from 0, so =3*n0-3;
	free_slot = root - 2; 

	//initialize the table, all entries empty
	for (i = 0; i < ntotal;i++)
		d.huf_tree[i] = -1;
	
	for (itr = htable.begin(); itr != htable.end; ++itr)
	{
		if (itr->r.level < 0) continue;
		d.run_table[itr->index / 2] = itr->r;
		loc = root;
		mask = 0x01;
		hcode = itr->codeword >> 1;
		for (i = 0; i < itr->hlen; ++i)
		{
			loc0 = loc - n0;
			if (i == (itr->hlen - 1))
			{
				if ((mask&hcode) == 0)
					d.huf_tree[loc0] = itr->index / 2;
				else
					d.huf_tree[loc0 - 1] = itr->index / 2;
				continue;
			}
			if (mask&hcode == 0)
			{
				if (d.huf_tree[loc0] == -1)
				{
					d.huf_tree[loc0] = free_slot;
					free_slot -= 2;
				}
				loc = d.huf_tree[loc0];
			}
			else
			{
				if (d.huf_tree[loc0 - 1] == -1)
				{
					d.huf_tree[loc0 - 1] = free_slot;
					free_slot -= 2;
				}
				loc = d.huf_tree[loc0 - 1];
			}
			mask << 1;
		}

	}


}