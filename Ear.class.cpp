
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
	/* TODO
	cv::Rect refPoint;
	float scale = 0.2f;
	refPoint.x = extractedEar.cols - extractedEar.cols * scale;
	refPoint.y = 0;
	refPoint.width = refPoint.height = extractedEar.cols * scale;
	cv::Mat integralImg;
	cv::Mat patch = extractedEar(refPoint);
	cv::imshow("patch", patch);
	cv::integral(patch, integralImg, CV_32S);
	calcHaarFeature(integralImg);
	cv::rectangle(extractedEar, refPoint, CV_RGB(0,255,0),2);
	*/
	
}

void Ear::calcHaarFeature(cv::Mat integralImg) {
	/* TODO
	float haarVertical;
	float haarHorizontal;

	int a, b, tmp1, tmp2;
	
	if(integralImg.cols%2 == 0) {
		a = (integralImg.cols/2) - 1;
		b = (2*a)+1;
		tmp1 = integralImg.at<int>(a,a) - integralImg.at<int>(0,0) - integralImg.at<int>(a,0)- integralImg.at<int>(0,a);
		tmp1 -= integralImg.at<int>(b,b) - integralImg.at<int>(a + 1, a + 1) - integralImg.at<int>(b, a + 1)- integralImg.at<int>(a + 1, b);
	}
	else {
		a = integralImg.cols/2;
		b = 2*a;
		tmp1 = integralImg.at<int>(a,a) - integralImg.at<int>(0,0) - integralImg.at<int>(a,0)- integralImg.at<int>(0,a);
		tmp1 -= integralImg.at<int>(b,b) - integralImg.at<int>(a + 1, a + 1) - integralImg.at<int>(b, a + 1)- integralImg.at<int>(a + 1, b);
		std::cout << "nieparzyste" << std::endl;
	}
	haarVertical = tmp1;
	std::cout << tmp << std::endl;
	*/
}



void Ear::execute() {
	isReady = false;
	earCords = findEar();
	extractEar(earCords);
	//findReference();	
}
	

