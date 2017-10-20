#include"RGB_YUV.h"
#include"downSample.h"
#include"DCT.h"
#include"reorder.h"
#include"quantization.h"
#include"RLE.h"
#include"HuffmanEncoding.h"
#include<time.h>
//#include<iostream>
//#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define IMGSIZE 64
bool huffman_Encoding(int imgSize, RUN3D runs[], int N);//N is blocksize=8
bool huffman_Decoding(int imgSize, RUN3D runs[], int N);//N is blocksize=8
int main()
{
	//unsigned short left_mask = 0xFF00;
	//unsigned short right_mask = 0x00FF;
	//short d = 0x0604;
	//short e = (d & left_mask)>>8;
	//cout << e << endl;
	//int a[] = { 1, 0,1,1 };
	//char b=huffTreeTraverse_13(a, 5);
	//char c = huffTreeTraverse_4(a, 5);
	//cout <<endl<< c << endl;



	Mat rawImg= imread("..\\zhihu_test\\images\\img4.bmp");
	Mat rgbImg2 = rawImg(Rect(0, 0, rawImg.cols / 8 * 8, rawImg.rows / 8 * 8));
	Mat rgbImg1 = rgbImg2(Rect(0, 0, 64, 64));

	namedWindow("original image", WINDOW_NORMAL);
	imshow("original image", rgbImg1);
	waitKey(0);

	int N = 8;
	short rgbImg[IMGSIZE][IMGSIZE][3];
	for (int height = 0; height <rgbImg1.rows; height++)
	{
		for (int width = 0; width < rgbImg1.cols; width++)
		{
			rgbImg[height][width][0] = rgbImg1.at<Vec3b>(height, width)[0];
			rgbImg[height][width][1] = rgbImg1.at<Vec3b>(height, width)[1];
			rgbImg[height][width][2] = rgbImg1.at<Vec3b>(height, width)[2];
		}
	}

	reorder reorder1;
	quantizer quant1(1);


	rgb2yuv(&rgbImg[0][0][0],IMGSIZE);
	downSample(&rgbImg[0][0][0],IMGSIZE,"420",true);
	DCT(&rgbImg[0][0][0],IMGSIZE,8);
	short rgbImgR[IMGSIZE][IMGSIZE][3];
	reorder1.reorder_8(&rgbImg[0][0][0], &rgbImgR[0][0][0], IMGSIZE);
	quant1.quantization(&rgbImgR[0][0][0], IMGSIZE);
	RUN rleCodec; RUN3D runs[IMGSIZE*IMGSIZE *3];

	rleCodec.run_block(&rgbImgR[0][0][0], IMGSIZE, 8, &runs[0]);

	///////////////////////huff huff huff//////////////////
	huffman_Encoding(IMGSIZE, &runs[0], N);
	huffman_Decoding(IMGSIZE, &runs[0], N);
	///////////////////////huff huff huff//////////////////

	rleCodec.run_decode(&runs[0], IMGSIZE, 8, &rgbImgR[0][0][0]);
	quant1.reverse_quantization(&rgbImgR[0][0][0], IMGSIZE);
	reorder1.reverse_reorder_8(&rgbImgR[0][0][0], &rgbImg[0][0][0], IMGSIZE);
	IDCT(&rgbImg[0][0][0],IMGSIZE,8);
	yuv2rgb(&rgbImg[0][0][0],IMGSIZE);






	for (int height = 0; height <rgbImg1.rows; height++)
	{
		for (int width = 0; width < rgbImg1.cols; width++)
		{
			rgbImg1.at<Vec3b>(height, width)[0] = rgbImg[height][width][0];
			rgbImg1.at<Vec3b>(height, width)[1] = rgbImg[height][width][1];
			rgbImg1.at<Vec3b>(height, width)[2] = rgbImg[height][width][2];
		}
	}

	namedWindow("recovered image3", WINDOW_NORMAL);
	imshow("recovered image3", rgbImg1);
	waitKey(0);
}


bool huffman_Encoding(int imgSize, RUN3D runs[], int N)//N is blocksize=8
{
	//RUN3D runs_block[64];
	for (int channel = 0; channel < 3; channel++)
	{
		int blockNum = 0; int k = 0;
		for (int blockNum = 0; blockNum < imgSize / N*imgSize / N; blockNum++)
		{
			RUN3D runs_block[64];
			k = 0;
			do
			{
				runs_block[k] = runs[k * 3 + blockNum*N*N * 3 + channel];
				k++;
			} while (runs[(k - 1) * 3 + blockNum*N*N * 3 + channel].last != 1);
			Hcodec hcodec1;
			char buf[10];
			sprintf_s(buf, "%d", blockNum);
			string hufFileNum = buf;
			char buf1[10];
			sprintf_s(buf1, "%d", channel); string channelNum = buf1;
			string hufFileName = "huff" + hufFileNum +"_"+buf1+".huf";
			char *buf2 = &hufFileName[0];
			BitFileIO *outputs = new BitFileIO(buf2, 0);
			//set<RunHuff> huffSet; //BitFileIO outputs("1.txt","2.txt");
			hcodec1.build_htable();
			hcodec1.huff_encode(runs_block, outputs);
			//hcodec1.huff_encode_EOS(outputs);
			outputs->closeOutput();

		}
	}
	return true;
}

bool huffman_Decoding(int imgSize, RUN3D runs[], int N)//N is blocksize=8
{
	for (int channel = 0; channel < 3; channel++)
	{
		int blockNum = 0; int k = 0;
		for (int blockNum = 0; blockNum < imgSize / N*imgSize / N; blockNum++)
		{
			RUN3D runs_block[64];
			Hcodec hcodec;
			hcodec.build_htable();
			Dtables decode_tables;     //tables used in decoding process
			hcodec.build_huff_tree(decode_tables);
			char buf[10];
			sprintf_s(buf, "%d", blockNum);
			string hufFileNum = buf;
			char buf1[10];
			sprintf_s(buf1, "%d", channel); string channelNum = buf1;
			string hufFileName = "huff" + hufFileNum +"_"+buf1+ ".huf";
			char *buf2 = &hufFileName[0];

			BitFileIO *bitin = new BitFileIO(buf2, 1);    //construct bitstream for input
			hcodec.huff_decode(bitin, decode_tables, runs_block);
			bitin->closeInput();
			k = 0;
			do
			{
				runs[blockNum*N*N * 3 + k * 3 + channel] = runs_block[k];
				k++;
			} while (runs_block[k-1].last != 1);

		}
	}
	return true;
}
