
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
		cords.x -= cords.width * 0.25;
		cords.y -= cords.height * 0.17;
		cords.width += cords.width * 0.21;
		cords.height += cords.height * 0.21;
		cv::rectangle(earSelected, cords, CV_RGB(255,0,0),2);	
	}
	return cords;	
}
void Ear::houghLines(cv::Mat img) {
	std::vector<cv::Vec2f> lines;
	HoughLines(extractedEar, lines, 1, CV_PI/180, 250, 0, 0 );

	for( size_t i = 0; i < lines.size(); i++ ) {
		float rho = lines[i][0];
		float theta = lines[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a*rho;
		double y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( extractedEar, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
	}


	/*std::vector<cv::Vec4i> lines;
	HoughLinesP(extractedEar, lines, 1, CV_PI/180, 255, 50, 150 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		cv::Vec4i l = lines[i];
		line( extractedEar, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
	}*/
}
void Ear::contours(cv::Mat img) {
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	std::vector<std::vector<cv::Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{  convexHull( cv::Mat(contours[i]), hull[i], false ); }

	cv::Mat drawing = cv::Mat::zeros( img.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ ) {
		cv::Scalar color = cv::Scalar( 50,50,50 );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
		drawContours( drawing, hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	}
	cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
}
void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	GaussianBlur( extractedEar, extractedEar, cv::Size( 7,7 ), 0, 0 );
	cv::cvtColor(extractedEar, extractedEar, CV_RGB2GRAY);
	Canny(extractedEar, extractedEar, 50, 200, 3);
	houghLines(extractedEar);
	contours(extractedEar);
	isReady = true;
	}
}

void Ear::findReference() {
//TODO
	
}




void Ear::execute() {
	isReady = false;
	earCords = findEar();
	extractEar(earCords);
}
	

