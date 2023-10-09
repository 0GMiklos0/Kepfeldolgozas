#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "histo.h"

using namespace std;
using namespace cv;

void createHisto(const Mat img, Mat& hiszto) {
	//a hiszto az eredmeny, float típusú elemeket tartalmaz majd
	vector<Mat> kepek;
	kepek.push_back(img); // egy képet használunk
	vector<int> csatornak;
	csatornak.push_back(0); //a képnek a 0. csatornáját használjuk
	vector<int> hiszto_meretek;
	hiszto_meretek.push_back(256); //szürkeárnyalatok száma
	vector<float> hiszto_tartomanyok;
	hiszto_tartomanyok.push_back(0.0f); //hol kezdődik a tartomány
	hiszto_tartomanyok.push_back(255.f); //meddig tart
	//accumlate: marad false (nullázza a hisztogrammot)
	calcHist(kepek, csatornak, noArray(), hiszto, hiszto_meretek,
		hiszto_tartomanyok, false);
}

int elso() {
	Mat img = imread("madar.jpg",IMREAD_COLOR);

	vector<Mat> chs;
	split(img, chs);

	imshow("elso G", chs[0]);
	imshow("masodik B", chs[1]);
	imshow("harmadik R", chs[2]);

	Mat dest;
	threshold(chs[0], dest, 0, 255, THRESH_OTSU);
	
	imshow("img", img);
	imshow("img", dest);
	waitKey();
	return 0;
}

int masodik() {
	Mat img = imread("gray_buttons.jpg", IMREAD_GRAYSCALE);

	Mat dest, mask;
	inRange(img, 165, 213, mask);

	medianBlur(mask, mask, 11);

	imshow("mask", mask);
	imshow("dest", img);
	waitKey();
	return 0;
}

int video() {
	VideoCapture cap;
	cap.open("sas.avi");

	if (!cap.isOpened()) {
		cout << "baj van" << endl;
		exit(-1);
	}
	Mat img, skymask, gray;
	while (1) {
		cap >> img;
		if (img.empty()) break;
		cvtColor(img, gray, COLOR_BGR2GRAY);
		inRange(gray, 100, 155, skymask);
		medianBlur(skymask, skymask, 5);
		img.setTo(Scalar(0, 0, 0), skymask);



		imshow("img", img);
		imshow("mask", skymask);
		waitKey(25);
		
	}
	waitKey();
	return 0;
}

int negyedik() {
	Mat img = imread("sejtek.png"), dest;
	int x = 0, y = 0;
	//Histo::calcHisto1();
	for (int th = 20; th < 255; th += 20) {
		string wname = to_string(th);
		namedWindow(wname, WINDOW_NORMAL);

		threshold(img, dest, th, 255, THRESH_BINARY_INV);
		imshow(wname, dest);

		resizeWindow(wname, Size(300, 300));
		moveWindow(wname, x,y);
		x += 300;
		if (x > 1000) {
			y += 300;
			x = 0;
		}
	}
	imshow("img", img);
	waitKey();
	return 0;
}

int calc_th_value(const Mat src, float ratio = 0.1f) {
	Mat histo;
	createHisto(src, histo);
	int th;
	int s = 0;
	int nPixels = src.cols * src.rows * ratio;
	for (th = 0; th < 256; th++) {
		s += histo.at<float>(th);
		if (s >= nPixels) return th;
	}
	return -1;
}

int otodik() {
	Mat img = imread("scanned3.png", IMREAD_GRAYSCALE);
	Mat dest;
	int th = calc_th_value(img, 0.1f);
	if (th == -1) {
		cout << "oops" << endl;
	}
	threshold(img, dest, th, 255, THRESH_BINARY_INV);

	imshow("kuszobolt", dest);
	waitKey();
	return 0;
}

int plusz() {
	int sugar = 20;
	int c = 10;
	namedWindow("tool", WINDOW_NORMAL);
	createTrackbar("kuszob", "tool", &sugar, 225);
	createTrackbar("C", "tool", &c, 50);

	Mat img = imread("zh.jpg", IMREAD_GRAYSCALE);
	Mat dest;
	int q;
	while (waitKey(1) != 'q') {
		adaptiveThreshold(img, dest, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 2*sugar+3, c-25);
		imshow("kuszobolt", dest);
	}
	return 0;
}

int main() {
	//elso();
	//masodik();
	//video();
	//negyedik();
	//otodik();
	plusz();
	return 0;
}
