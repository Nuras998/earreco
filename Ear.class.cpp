
#include "Ear.class.h"
#include <iostream>

extern cv::CascadeClassifier Ear::earClassifier;

Ear::Ear() {
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}
}

Ear::Ear(const char* fileName) {
	originalImg = cv::imread(fileName);
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}
	execute();		
}
Ear::Ear(cv::Mat img) {
	originalImg = img.clone();
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}	
	execute();
}

void Ear::load(const char* fileName) {
	originalImg = cv::imread(fileName);
}

cv::Mat Ear::getOriginal() {
	return originalImg;
}

cv::Mat Ear::getSelectedEar() {
	return earSelected;
}
cv::Mat Ear::getExtractedEar() {
	return extractedEar;
}

cv::Rect Ear::findEar() {
	std::vector<cv::Rect> ears;
	earSelected = originalImg.clone();
	earClassifier.detectMultiScale(originalImg, ears);
	cv::Rect cords(0,0,0,0);
	if(ears.size() == 1) {
		cords = ears[0];
		cv::rectangle(earSelected, cords, CV_RGB(255,0,0),2);	
	}
	return cords;	
}

void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	cv::cvtColor(extractedEar, extractedEar, CV_RGB2GRAY);
	isReady = true;
	}
}

void Ear::findReference() {
	cv::Rect refPoint;
	cv::Mat integralImg;
	cv::integral(extractedEar, integralImg);

	float scale = 0.2f;
	int a =  extractedEar.cols * scale;
	int stepY = a/8;
	int stepX = a/5;

	a = a%2 == 0 ? a : a+1;
	stepX = stepX == 0 ? 1 : stepX;
	stepY = stepY == 0 ? 1 : stepY;

	refPoint.x = extractedEar.cols - a;
	refPoint.y = 0;
	refPoint.width = refPoint.height = a;
	
	while(refPoint.x >= 0) {
		while(refPoint.y + refPoint.height < extractedEar.rows) {
			cv::Mat patch = extractedEar(refPoint);
			calcHaarFeature(patch);
			cv::Mat clone = extractedEar.clone();
			cv::rectangle(clone, refPoint, CV_RGB(0,255,0),2);
			cv::imshow("ear", clone);
			cv::imshow("patch", patch);
			refPoint.y += stepY;
			clone.release();
			patch.release();
			cv::waitKey(0);
		}
	refPoint.x -= stepX;
	refPoint.y = 0;
	}
	//cv::Mat patch = cv::imread("images/gradient2.jpg");
	//calcHaarFeature(patch);
}

void Ear::calcHaarFeature(cv::Mat img) {
	//int haarVertical;
	float haarHorizontal;
	long int sum1 = 0;
	long int sum2 = 0;
	long int sum = img.cols * img.rows/2 * 255;
	//std::cout << (int)img.at<uchar>(0,0) << std::endl;
	//std::cout << img.rows << std::endl;

	for(int i = 0; i < img.rows/2; i++) {
		for(int j = 0; j < img.cols; j++) {
			sum1 += (int)img.at<uchar>(i,j);
		}
	}
	std::cout << "Sum1: " << sum1 << std::endl;

	for(int i = img.rows/2; i < img.rows; i++) {
		for(int j = 0; j < img.cols; j++) {
			sum2 += (int)img.at<uchar>(i,j);
		}
	}
	std::cout << "Sum2: " << sum2 << std::endl;
/*
	cv::Point A(point.x, point.y);
	cv::Point B(point.x + point.width, point.y);
	cv::Point C(point.x, point.y + point.height/2);
	cv::Point D(point.x + point.width, point.y + point.height);

	sum1 = integralImg.at<int>(D.x, D.y) -  integralImg.at<int>(B.x, B.y) - integralImg.at<int>(C.x, C.y) + integralImg.at<int>(A.x, A.y);

	A.y += point.height/2;
	B.y += point.height/2;
	C.y += point.height/2;
	D.y += point.height/2;

	sum2 = integralImg.at<int>(D.x, D.y) -  integralImg.at<int>(B.x, B.y) - integralImg.at<int>(C.x, C.y) + integralImg.at<int>(A.x, A.y);
*/
	haarHorizontal = sum1 - sum2;
	haarHorizontal /= sum;
	std::cout << "Horizontal: " << haarHorizontal << std::endl << std::endl;
}




void Ear::execute() {
	isReady = false;
	earCords = findEar();
	extractEar(earCords);
//	findReference();
}
	
