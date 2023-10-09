#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "histo.h"

using namespace std;
using namespace cv;

int histo() {
	
	Mat img = imread("orange1.jpg",IMREAD_COLOR);
	imshow("unfixed", img);

	Mat hsv;
	cvtColor(img,hsv,COLOR_BGR2HSV);

	Mat lab;
	cvtColor(img, lab, COLOR_BGR2Lab);

	vector<Mat> chs;
	split(lab, chs); //chs 3 elemu 3x1-es csatorna BGR
	vector<Mat> chss;
	split(hsv, chs);
	//1. histogram
	/*double ah, fh; //also hatar es felso hatar a histogram-on
	minMaxLoc(img, &ah, &fh);


	
	Mat res;
	if (ah != fh) {
		fh = 60;
		res = (img - ah) * 255 / (fh - ah);
	}
	else res = img.clone();
	*/
	//2. histogram (kiegyenliteses)

	Mat res;
	equalizeHist(chs[0], chs[0]);
	merge(chs, res);

	cvtColor(res, res, COLOR_Lab2BGR);
	imshow("d", res);
	//Histo::showHisto(res);
	


	waitKey();

	return 0;
}

int noise() {
	string menu = "tool";
	cv::namedWindow(menu, WINDOW_NORMAL);
	cv::resizeWindow(menu, Size(500, 50));
	int radius = 1, sigma = 1;
	createTrackbar("radius", menu, &radius, 25);
	createTrackbar("sigma", menu, &sigma, 25);
	Mat img = imread("szines.png",IMREAD_COLOR);

	Mat blur_img, gauss_img, median_img, bilat_img;
	imshow("eredeti", img);
	while (cv::waitKey(20) != 'q') {
		int ksize = max(3, 2 * radius + 1);
		blur(img, blur_img, Size(ksize, ksize));
		imshow("blur", blur_img);
		GaussianBlur(img, gauss_img, Size(ksize, ksize), sigma/2, sigma/2);
		imshow("gblur", gauss_img);
		medianBlur(img, median_img, ksize);
		imshow("median blur", median_img);
	}
}

int main() {
	//histo();
	noise();
}
