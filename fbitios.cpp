// fbitios.cpp
// http://forejune.co/
#include "fbitios.h"

using namespace std;

BitFileIO::BitFileIO(char *argv_in, char *argv_out)
{
	ifs.open(argv_in, ios::in);
	if (ifs.fail()) {
		cout << "error open file " << argv_in << endl;
		exit(1);
	}
	ofs.open(argv_out, ios::out);
	if (ifs.fail()) {
		cout << "error open file " << argv_out << endl;
		exit(1);
	}
	ins_pos = outs_pos = 0;
	progress_indicator = 0;
}

BitFileIO::BitFileIO(char *argv_out, int in_out)
{
	if (in_out == 1){
		ifs.open(argv_out, ios::in);
		if (ifs.fail()) {
			cout << "error open file " << argv_out << endl;
			exit(1);
		}
	}
	else {
		ofs.open(argv_out, ios::out);
		if (ofs.fail()) {
			cout << "error open file " << argv_out << endl;
			exit(1);
		}
	}
	ins_pos = outs_pos = 0;
	progress_indicator = 0;
}

//read one bit from file
int BitFileIO::inputBit()
{

	int value;
	if (ins_pos == 0) {
		char c = ifs.get();
		if (ifs.eof()) {
			//cout << "Out of data" << endl;
			return -1;
		}
		if (!(progress_indicator++ & 0x0fff)){
			cout << "." << flush;
		}
		ins = (ul)c;
	}
	int i = ins_pos++;
	if (ins_pos > 7)
		ins_pos = 0;
	return(ins[i] ? 1 : 0);
}

//output one bit
void BitFileIO::outputBit(int bit)
{
	if (bit)
		outs.set(outs_pos);	//set bit to 1
	else
		outs.reset(outs_pos);	//set bit to 0
	if (outs_pos == 7) {
		char c = (char)outs.to_ulong();
		ofs.put(c);
		if (ofs.fail()) {
			cout << "\nfatal error in output bit " << endl;
		}
		outs_pos = 0;	//reset output position
		if (!(progress_indicator++ & 0x0fff)){
			cout << "." << flush;
		}
	}
	else
		++outs_pos;
}	// OutputBit() 

//read n bits from file
long BitFileIO::inputBits(int n)
{
	if (n > max_bits - 1) {
		cout << "Cannot handle " << n << " bits at one time";
		return -2;
	}
	bitset<32> value;
	int pos = 0;
	while (pos < n) {
		if (ins_pos == 0) {
			char c = ifs.get();
			if (ifs.eof()) {
				//cout << "Out of data" << endl;
				return -1;
			}
			if (!(progress_indicator++ & 0x0fff)){
				cout << "." << flush;
			}
			ins = (ul)c;
		} //if ( read one char )
		int i = ins_pos++;
		if (ins_pos > 7)
			ins_pos = 0;
		if (ins[i])
			value.set(pos);	//a 1
		else
			value.reset(pos);    	// a 0
		++pos;			//points to next bit position
	} //while
	return ((long)value.to_ulong());	//return the n-bit value
}	//inputBits()

//output n bits to file
void  BitFileIO::outputBits(ul data, int n)
{
	bitset<32> value = data;
	int pos = 0;
	while (pos < n) {
		if (value[pos++]) 	//test the bit of input value
			outs.set(outs_pos);	//set bit to 1
		else
			outs.reset(outs_pos);	//set bit to 0
		if (outs_pos == 7) {
			char c = (char)outs.to_ulong();
			ofs.put(c);
			if (ofs.fail()) {
				cout << "\nfatal error in output bit " << endl;
			}
			outs_pos = 0;	//reset output position
			if (!(progress_indicator++ & 0x0fff)){
				cout << "." << flush;
			}
		}
		else
			++outs_pos;
	}
}	/* outputBits() */

//push out remaining bits to file
void	BitFileIO::closeOutput()
{
	if (outs_pos != 0){
		for (int i = outs_pos; i < 7; ++i)
			outs[i] = 0;
		ofs.put((char)outs.to_ulong());
	}
	ofs.close();
}

void	BitFileIO::closeInput()
{
	ifs.close();
}

