#include"RGB_YUV.h"
#include"downSample.h"
#include"DCT.h"
#include"reorder.h"
#include"quantization.h"
#include"RLE.h"
//#include<iostream>
//#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define IMGSIZE 64

int main()
{
	Mat rawImg= imread("..\\zhihu_test\\images\\img3.bmp");
	Mat rgbImg1 = rawImg(Rect(0, 0, rawImg.cols / 8 * 8, rawImg.rows / 8 * 8));

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
	quantizer quant1(2);


	rgb2yuv(&rgbImg[0][0][0],IMGSIZE);
	downSample(&rgbImg[0][0][0],IMGSIZE,"422",true);
	DCT(&rgbImg[0][0][0],IMGSIZE,8);
	short rgbImgR[IMGSIZE][IMGSIZE][3];
	reorder1.reorder_8(&rgbImg[0][0][0], &rgbImgR[0][0][0], IMGSIZE);
	quant1.quantization(&rgbImgR[0][0][0], IMGSIZE);
	RUN rleCodec; RUN3D runs[IMGSIZE*IMGSIZE *3/8/8*8*8];
	rleCodec.run_block(&rgbImgR[0][0][0], IMGSIZE, 8, &runs[0]);


	rleCodec.run_decode(&runs[0], IMGSIZE, 8, &rgbImg[0][0][0]);
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