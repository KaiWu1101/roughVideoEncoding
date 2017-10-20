//fbitios.h

#ifndef _FBITIOS_H_
#define _FBITIOS_H_

#include	<stdio.h>
#include	<stdlib.h>
#include	<iostream>
#include 	<fstream>
#include	<string.h>
#include 	<set>
#include	<bitset>

using namespace std;

typedef	unsigned int	ui;
typedef unsigned long	ul;
typedef	unsigned char	uc;

class  BitFileIO{
private:
	bitset<32> ins;
	bitset<32> outs;
	fstream ifs, ofs;
	int ins_pos;
	int outs_pos;
	int progress_indicator;
	enum { max_bits = 32 };
public:
	BitFileIO(char *argv_in, char *argv_out);		//constructors
	BitFileIO(char *argv_out, int in_out);
	int  inputBit();		//input one bit from file
	void outputBit(int bit);  	//output one bit to file
	long inputBits(int n);  	//input n bits from file
	void outputBits(ul data, int n);	//output n bits to file
	void closeOutput();		//close_output
	void closeInput();		//close input
};

#endif
