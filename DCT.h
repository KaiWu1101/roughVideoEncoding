#include<iostream>
#include"macroBlock.h"
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void DCT(short* srcImg,int imgSize, int N);
void IDCT(short* srcImg, int imgSize, int N);
void DCT_macroblock(short* blockP, int imgSize);
void IDCT_macroblock(short* blockP, int imgSize);
void print_DCT(short* blockP, int imgSize);