#ifndef _HUFFMANENCODING_H_
#define _HUFFMANENCODING_H_
#include <iostream>
#include "RLE.h"
#include <set>
#include "fbitios.h"
using std::set;
using namespace std;
char huffTreeTraverse_13(int huffCode[], int N);
char huffTreeTraverse_4(int huffCode[], int N);

class RunHuff
{
public:
	RUN3D r;
	unsigned int codeword;
	char hlen;
	short index; //table index where codeword saved;
	RunHuff(){}; //constructor
	RunHuff(RUN3D a, unsigned int c, char len, short idx)
	{
		r = a; codeword = c; hlen = len; index = idx;
	}
	friend bool operator <(RunHuff left, RunHuff right)
	{
		if (left.r.run < right.r.run)
			return true;
		if (left.r.run > right.r.run)
			return false;

		if (left.r.level < right.r.level)
			return true;
		if (left.r.level > right.r.level)
			return false;

		if (left.r.last > right.r.last)  //why l.last>r.last, return true;
			return true;
		return false;
	}
};


class Dtables
{
public:
	short huf_tree[1024];
	RUN3D run_table[512];
};

void build_huff_tree(set<RunHuff> &htable, Dtables &d);
void build_htable(set<RunHuff> &htable);
void huff_encode(set<RunHuff> &htable, RUN3D runs[], BitFileIO &outputs);
void escape_encode(BitFileIO &outputs, RUN3D &r);
#endif