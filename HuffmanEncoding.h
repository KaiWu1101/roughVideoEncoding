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

class Hcodec
{
private:
	const static int NSymbols = 256;   //maximum symbols allowed
	const static short ESC = 127;      //Escape run
	const static short ESC_HUF_CODE = 0x60;  //Escape code
	const static short EOS = 126;      //End of Stream 'symbol' ( run )
	const static short EOS_HUF_CODE = 0x0a;  //End of Stream 'symbol'
	bool tableNotBuilt;

public:
	set<RunHuff> htable;  //table to hold pre-calculated run-level and Huffman codewords
	//default constructor
	Hcodec();
	//use a set ( htable ) to collect all pre-calculated run-level and Huffman codewords
	void build_htable();
	void escape_encode(BitFileIO *outputs, RUN3D &r);
	void huff_encode(RUN3D runs[], BitFileIO *outputs);
	//Encode End of Stream symbol
	void huff_encode_EOS(BitFileIO *outputs);
	void build_huff_tree(Dtables &d);
	short huff_decode(BitFileIO *inputs, Dtables &d, RUN3D runs[]);
};

#endif