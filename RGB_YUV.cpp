#include"RGB_YUV.h"

void rgb2yuv(short* srcImg, int imgSize)
{
	short R, G, B;
	short Y, Cb, Cr;
	int height, width;
	for (height = 0; height < imgSize; height++)
	{
		for (width = 0; width < imgSize; width++)
		{
			R = *(srcImg + height*imgSize * 3 + width * 3 + 2);
			G = *(srcImg + height*imgSize * 3 + width * 3 + 1);
			B = *(srcImg + height*imgSize * 3 + width * 3 + 0);
			Y = ((19595*R+38470*G+7471*B)>>16);
			Cb = ((36962 * (B - Y) >> 16) + 128);
			Cr = ((46727 * (R - Y) >> 16) + 128);
			if (Y>255 || Cb>255 || Cr > 255)
			{
				cout << "error!!!:" << endl << "\t height=" << height << "\t width=" << width << endl;
			}
			*(srcImg + height*imgSize * 3 + width * 3 + 2) = Cr;
			*(srcImg + height*imgSize * 3 + width * 3 + 1) = Cb;
			*(srcImg + height*imgSize * 3 + width * 3 + 0) = Y;
		}
	}
	int k = 0;
	//yuvImg.copyTo(srcImg);
}

void yuv2rgb(short* srcImg, int imgSize)
{
	short R, G, B;
	short Y, Cb, Cr;
	for (int height = 0; height < imgSize; height++)
	{
		for (int width = 0; width < imgSize; width++)
		{
			Y = *(srcImg + height*imgSize * 3 + width * 3 + 0);// srcImg.at<Vec3b>(height, width)[0];
			Cb = *(srcImg + height*imgSize * 3 + width * 3 + 1);
			Cr = *(srcImg + height*imgSize * 3 + width * 3 + 2);
			R = Y + (91881 * Cr >> 16) - 179;
			G = Y - ((22544 * Cb + 46793 * Cr) >> 16) + 135;
			B = Y + (116129 * Cb >> 16) - 226;
			if (R>255)
				R = 255;
			else if (G > 255)
				G = 255;
			else if (B > 255)
				B = 255;

			if (R<0)
				R = 0;
			else if (G <0)
				G = 0;
			else if (B <0)
				B = 0;
			//if (R>255 || G>255 || B > 255)
			//{
			//	cout << "error!!!:" << endl << "\t height=" << height << "\t width=" << width << endl;
			//	cout << "R=: " << R << "G= " << G << "B= " << B<<endl;
			//}
			*(srcImg + height*imgSize * 3 + width * 3 + 2) = R;
			*(srcImg + height*imgSize * 3 + width * 3 + 1) = G;
			*(srcImg + height*imgSize * 3 + width * 3 + 0) = B;
		}
	}
	//rgbImg.copyTo(srcImg);
}