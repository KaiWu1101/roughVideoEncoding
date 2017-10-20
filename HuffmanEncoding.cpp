#include "HuffmanEncoding.h"


//13 entries for this huffman table;
//int htree_13[] = { 97, 98, 99, 100, 101, 3, 2, 6, 1, 4, 8, 10, 0 };
//char huffTreeTraverse_13(int huffCode[], int N)
//{
//	int loc = 3 * N - 3;
//	int count = 0;
//	do
//	{
//		int loc0 = loc;
//		if (huffCode[count] == 0)
//		{
//			loc = htree_13[loc0];
//			count++;
//		}
//		else
//		{
//			loc = htree_13[loc0 - 1];
//			count++;
//		}
//	} while (loc >= N);
//	return htree_13[loc];
//}
//
//short htree_4[] = { 0x0203, 0x0105, 0x0604, 0x0007 };
//
//char huffTreeTraverse_4(int huffCode[],int N)
//{
//	unsigned short left_mask = 0xFF00;
//	unsigned short right_mask = 0x00FF;
//	int count = 0;
//	unsigned short loc = (N - 1) + N - 1;
//	do
//	{
//		int loc0 = loc - N;
//		if (huffCode[count] == 0)
//		{
//			loc = (htree_4[loc0] & left_mask)>> 8;
//		}
//		else
//			loc = htree_4[loc0] & right_mask;
//		count++;
//	} while (loc >= N);
//	switch (loc)
//	{
//		case 0: loc = 'a'; break;
//		case 1: loc = 'b'; break;
//		case 2: loc = 'b'; break;
//		case 3: loc = 'd'; break;
//		case 4: loc = 'e'; break;
//		default: cout << "wrong encoded huffman code" << endl;
//			exit(1);
//
//	}
//	return loc;
//}

Hcodec::Hcodec()
{
	tableNotBuilt = true;
}

//htable is a set of RunHuff objects
void Hcodec::build_htable()
{
	//N = number of pre-calculated codewords with positive levels
	//  In practice, N should be larger than 100
	short i, j, k, N = 11;
	//lengths of Huffman codewords (not including sign-bit)
	char hlen[] = { 2, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7 };
	//Huffman codewords, 0x60 for ESC, 0x3a for EOS; a codeword should NEVER be a prefix of another
	unsigned short hcode[] = { 0x01, 0x3, 0x7, 0xf, 0xe, 0x16, 0x6, 0x1a, 0x2a, EOS_HUF_CODE, ESC_HUF_CODE };

	//data of 3D run-level tuples ( codewords )
	unsigned char runs[] = { 0, 1, 2, 0, 0, 3, 4, 5, 0, EOS, ESC };  //126 signifies end of stream, 127 signifies Escape
	short levels[] = { 1, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1 };   //Note: don't set Escape level to 0
	char  lasts[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };   //  otherwise => duplicate key          

	RUN3D r;              //a 3D run-level codeword ( tuple )
	RunHuff rf[128];      //table containing RunHuff objects
	//inserting RunHuff objects into rf[] 
	k = 0; j = 0;
	for (i = 0; i < N; i++) {
		r.run = runs[i];
		r.level = levels[i];
		r.last = lasts[i];
		//construct a RunHuff object, positive level, so sign=0
		rf[k++] = RunHuff(r, hcode[i] << 1, hlen[i], j++);
		//do the same thing for negative level, sign = 1 
		r.level = (short)-r.level;
		rf[k++] = RunHuff(r, (short)((hcode[i] << 1) | 1), hlen[i], j++);
	}
	k = 2 * N;            //insert all 2N ( positive and negative levels ) RunHuff objects into htable
	for (i = 0; i < k; ++i)
		htable.insert(rf[i]);
	tableNotBuilt = false;
}

void Hcodec::escape_encode(BitFileIO *outputs, RUN3D &r)
{
	if (r.level < 0) {                  //value of level negative
		outputs->outputBit(1);            //output sign-bit first
		r.level = -r.level;                 //change level value to positive 
	}
	else
		outputs->outputBit(0);            //value of level negative
	outputs->outputBits(ESC_HUF_CODE, 7);  //ESCAPE code
	if (r.run == 64) r.run = 63;        //r.level differentiates between if last element nonzero 
	outputs->outputBits(r.run, 6);      //6 bits for run value
	outputs->outputBits(r.level,16);    //8 bits for level value
	outputs->outputBit(r.last);         //1 bit for last value
}

/*
Inputs: htable, a data member, contains all the pre-calculated Huffman codewords of 3D run-level tuples
runs[] contains the 3D run-level tuples of a macroblock of quantized DCT coefficients
Outputs:bitstreams of codewords ( Huffman + sign or ESCAPE + 3D run-level ) to BitFileIO outputs
*/
void Hcodec::huff_encode(RUN3D runs[], BitFileIO *outputs)
{
	short i, j, k;
	set<RunHuff>::iterator itr;                           //iterator to traverse htable

	if (tableNotBuilt)
		build_htable();

	i = 0;
	k = 0;
	while (i < 64) {                                    //a macroblock has at most 64 samples
		RunHuff rf(runs[k], 0, 0, 0);                    //construct a RunHuff object; only runs[k] is relevant
		//as it is used for searching ( we've defined '<' in RunHuff class )
		if ((itr = htable.find(rf)) != htable.end())   //found
			outputs->outputBits(itr->codeword, itr->hlen + 1);
		else                                                //not found
			escape_encode(outputs, rf.r);                 //need to 'escape encode' the 3D run-level tuple
		if (runs[k].last) break;                          //end of run-level codewords
		i += (runs[k].run + 1);                           //for handling the special case of whole block of run being 0
		k++;
	}
}

//Encode End of Stream symbol
void Hcodec::huff_encode_EOS(BitFileIO *outputs)
{
	RUN3D r;                       //a 3D run-level codeword ( tuple )
	set<RunHuff>::iterator itr;     //iterator to traverse htable
	r.last = 0;
	r.level = 1;
	r.run = EOS;
	//construct a RunHuff object; only run3D r is relevant as it is
	//  used for searching (we've defined '<' in RunHuff class)
	RunHuff rf(r, 0, (unsigned char)0, (short)0);
	if ((itr = htable.find(rf)) != htable.end())   //found
		outputs->outputBits(itr->codeword, itr->hlen + 1);
	else
		printf("\nError: EOS not in Huffman table!\n");

	return;
}

/* -------------- The following are for decoding ----------------------- */

/*
Input:  htable, a data member, is a set containing all pre-calculated Huffman codewords of 3D run-level tuples
Outputs: d.huf_tree[], the Huffman tree containing pointers and indices pointing to run-level tuples
d.run_table[], the table that contains the run-level tuples ( codewords )
*/
void Hcodec::build_huff_tree(Dtables &d)
{
	set<RunHuff>::iterator itr;           //iterator to traverse set htable

	short i, j, n0, free_slot, loc, loc0, root, ntotal;
	unsigned int mask, hcode;

	n0 = NSymbols;                        //number of symbols ( = number of leaves in tree )
	ntotal = 2 * n0 - 1;                  //Huffman tree has  n0 - 1  internal nodes 
	root = 3 * n0 - 3;                    //location of root ( see section 21 ), offset n0 has been added
	free_slot = root - 2;                 //next free table entry for filing in with a pointer or an index
	//  ( note: root has root_left, root_right )
	for (i = 0; i < ntotal; ++i)        //initialize the table
		d.huf_tree[i] = -1;                 //all entries empty
	for (itr = htable.begin(); itr != htable.end(); ++itr) {    //consider all codewords in the set htable
		if (itr->r.level < 0) continue;  //only save positive levels of run-level codeword
		d.run_table[itr->index / 2] = itr->r;  //save run-level codeword; index divided by 2 as only postive levels saved
		loc = root;                        //always start from root
		mask = 0x01;                       //for examining bits of Huffman codeword
		hcode = itr->codeword >> 1;        //the rightmost bit is sign-bit, not part of Huffman
		for (i = 0; i < itr->hlen; ++i){ //traverse the Huffman codeword
			loc0 = loc - n0;               //everything shifted by offset n0

			if (i == (itr->hlen - 1)){  //last bit, should point to leaf
				if ((mask & hcode) == 0)    //a 0, save it at 'left' leaf
					d.huf_tree[loc0] = itr->index / 2;
				else                          //a 1, save it at 'right' leaf
					d.huf_tree[loc0 - 1] = itr->index / 2;
				continue;                     //get out of for i for loop, consider next codeword 
			}
			if ((mask & hcode) == 0){     //a 0 ( go left )
				if (d.huf_tree[loc0] == -1){  //slot empty
					d.huf_tree[loc0] = free_slot;       //point to left new child
					free_slot -= 2;             //next free table entry
				}                             //else : already has left child
				loc = d.huf_tree[loc0];       //follow the left child
			}
			else {                        //a 1 ( go right )
				if (d.huf_tree[loc0 - 1] == -1){ //slot empty
					d.huf_tree[loc0 - 1] = free_slot;//point to right new child
					free_slot -= 2;
				}                             //else: already has right child
				loc = d.huf_tree[loc0 - 1];     //follow the right child
			}
			mask <<= 1;                     //consider next bit
		} //for i
	} //for itr 
} //build_huff_tree()

/*
Inputs: inputs, the encoded bitstream to be decoded
d.huf_tree[], table containing the Huffman tree
d.run_table[], table containing the actual run-level codewords
Output: runs[], table containing the run-level tuples of a macroblock

*/
short Hcodec::huff_decode(BitFileIO *inputs, Dtables &d, RUN3D runs[])
{
	short n0, loc, loc0, root, k;
	char c, sign;
	bool done = false;
	RUN3D r;

	n0 = NSymbols;                        //number of symbols
	root = 3 * n0 - 3;                    //points to root of tree
	k = 0;
	while (!done) {
		loc = root;                         //starts from root
		sign = inputs->inputBit();               //sign-bit
		do {
			loc0 = loc - n0;
			c = inputs->inputBit();                //read one bit
			if (c < 0) { done = true; break; }  //no more data, done    
			if (c == 0)                     //a 0, go left
				loc = d.huf_tree[loc0];
			else                              //a 1, go right
				loc = d.huf_tree[loc0 - 1];
		} while (loc >= n0);              //traverse until reaches leaf
		r = d.run_table[loc];
		if (r.run == ESC) {               //ESCAPE code, read actual run-level tuple
			r.run = inputs->inputBits(6);    //read 6 bits for run
			r.level = inputs->inputBits(16);  //read 8 bits for level
			r.last = inputs->inputBit();         //read 1 bit for last
			if (sign)                     //if sign is 1, level should be negative
				r.level = -r.level;
		}
		else {                            //not ESCAPE code 
			if (sign)                       //1 => negative
				r.level = -r.level;
		}
		if ((r.run == 63) && (r.level == 0)) r.run = 64;  //whole block 0
		runs[k++] = r;                      //save tuple in table runs[]
		if (r.last)                       //end of macroblock 
			break;
	}  //while
	if (done) return -1;                //if ( done ) => no more data
	else return 1;
}

