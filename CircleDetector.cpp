#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
#include "histo.h"

using namespace std;
using namespace cv;

void kordetektalo1(const Mat& gray, vector<Vec3f>& circles) {

	int cannyTh = 80;  // biztos el hatara a Canny detektornak
	int minPoints = 50; // hany eloterponton kell atmennia a korvonalnak
	int minR = 22;  //minimalis sugar
	int maxR = 26;  //maximalis sugar
	int d = 2; //felbontast osztja (1-> eredeti, 2 -> 2x2-es resz egy 'doboz', stb)  
	int minDist = 10; //a minR-nel kozelebb korokbol az erosebbet kerjuk

	//vector<Vec3f> circles; // Vec3f: [0]: kozeppont x,  [1] kozeppont y, [2] subar
	cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, d, minDist, cannyTh, minPoints, minR, maxR);
}

int main() {
	//bekerunk egy kepet a sorozatban szereplo kepek meretenek megismeresehez
	Mat img = imread("gray_buttons.jpg", IMREAD_GRAYSCALE), dest;
	Mat img2 = Mat::zeros(img.size(), IMREAD_GRAYSCALE);
	Mat colorImg = img.clone();
	cvtColor(colorImg, colorImg, COLOR_GRAY2BGR);
	Mat g, gx, gy;
	Sobel(img, gx, CV_32F, 0, 1);
	Sobel(img, gy, CV_32F, 1, 0);
	cv::sqrt(gx.mul(gx) + gy.mul(gy), g);
	Mat gviz;
	convertScaleAbs(g, gviz);
	imshow("dest", g);
	int q, mask= 0, radius = 1, sigma = 1, ksize;
	vector<Vec3f> circles;
	namedWindow("threshold bar",WINDOW_NORMAL);
	resizeWindow("threshold bar", 500, 100);
	createTrackbar("Threshold", "threshold bar",&mask,255);
	createTrackbar("Radius", "threshold bar", &radius, 10);
	createTrackbar("Sigma", "threshold bar", &sigma, 50);
	int x, y, r;
	while ((q = waitKey(40))!='q') {
		colorImg = 0;
		ksize = max(2 * radius + 1, 3);
		sigma = max(sigma, 10);
		threshold(gviz, dest, mask, 255, THRESH_BINARY);
		GaussianBlur(dest, dest, Size(ksize, ksize), sigma/10);
		kordetektalo1(dest, circles);
		cvtColor(dest, colorImg, COLOR_GRAY2BGR);
		for (auto c : circles) {
			x = c[0];
			y = c[1];
			r = c[2];
			circle(colorImg, Point(x, y), r, Scalar(0,0,255), 2);
		}
		

		imshow("gviz", dest);
		imshow("circles", colorImg);
	}

	return 0;
}