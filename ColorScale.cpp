#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <vector>
#include "histo.h"

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("background.jpg", IMREAD_COLOR);
	Mat dest = img.clone().setTo(Scalar(0,0,0)), masker;
	vector<Mat> vec, mask = {Mat(img.size(),IMREAD_GRAYSCALE),Mat(img.size(),IMREAD_GRAYSCALE),Mat(img.size(),IMREAD_GRAYSCALE)};

	
	dest.setTo(Scalar(0,0,0));
	namedWindow("szuro", WINDOW_NORMAL);
	resizeWindow("szuro", 500, 100);
	
	int colorR = 0,colorB = 0,colorG=0, q;
	createTrackbar("Red","szuro",&colorR,255);
	createTrackbar("Green", "szuro", &colorG, 255);
	createTrackbar("Blue", "szuro", &colorB, 255);
	split(img, vec);
	Vec3b tmp;
	while ((q = waitKey(40)) != 'q') {
		dest = 0;
		inRange(vec[0], 0, colorB, mask[0]);
		inRange(vec[1], 0, colorG, mask[1]);
		inRange(vec[2], 0, colorR, mask[2]);
		merge(mask, masker);
		img.copyTo(dest, masker);
		imshow("img", dest);
	}

	return 0;
}
