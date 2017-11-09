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

cv::Mat Ear::getPreprocessedEar() {
	return preprocessedEar;
}

cv::Rect Ear::findEar() {
	std::vector<cv::Rect> ears;
	earSelected = originalImg.clone();
	earClassifier.detectMultiScale(originalImg, ears);
	cv::Rect cords(0,0,0,0);
	if(ears.size() == 1) {
		cords = ears[0];
		cords.x -= cords.width * 0.17;
		cords.y -= cords.height * 0.17;
		cords.width += cords.width * 0.21;
		cords.height += cords.height * 0.21;
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

void Ear::ImproveContrast(cv::Mat img, int alpha, int beta) {
	extractedEar.convertTo(img, -1, alpha, beta);
}

void Ear::ApplyGaussianBlur(cv::Mat img, int size) {
	GaussianBlur( img, img, cv::Size( size, size ), 0, 0 );
}

void Ear::ApplyTreshold(cv::Mat img, int p1, int p2) {
	threshold( img, img, p1, p2,CV_THRESH_BINARY );
}

void Ear::NormalizeBrightness(cv::Mat img) {
	int brightness = 0;
	int px_count = 0;
	double average_br = 0;
	double br_factor = 0;
	int value = 0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			brightness += (int) img.at<uchar>(i, j);
		//	std::cout << brightness << std::endl;
			px_count++;
		}
	}
	average_br = (double) brightness / (double)  px_count;
	//std::cout << "Brightess: " << brightness <<" , px_count: " << px_count << std::endl;
	br_factor = 154 / average_br;	
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			//std::cout << br_factor << std::endl;
			value = (int) img.at<uchar>(i,j);
			value *= 1.4;
			if(value > 255)
				value = 255;
			if(value < 0)
				value = 0;
			//std::cout << value << std::endl;
			img.at<uchar>(i,j) = value;
		}
	}
	
	std::cout << "Picture's average brightness is " << average_br << std::endl;
}
void Ear::preprocess() {
	if(isReady) {
		preprocessedEar = extractedEar.clone();
		cv::erode(preprocessedEar,preprocessedEar,cv::Mat(),cv::Point(-1,-1),15);
		NormalizeBrightness(preprocessedEar);
		//segmentation(preprocessedEar);
		//ImproveContrast(preprocessedEar, 2,-100);
		for(int i=0; i<7;i++) {
			ApplyGaussianBlur(preprocessedEar, 25);
		}
		ApplyTreshold(preprocessedEar, 220,255);
		//contours(preprocessedEar);
		imwrite("../Obrazy/Badaniepreprocessingu/progowanie/z.jpg", preprocessedEar);
		//imwrite("../Obrazy/Badaniepreprocessingu/erozja/bez.jpg", extractedEar);
		prep_done = true;
	}
}

void Ear::contours(cv::Mat img) {
	std::vector<std::vector<cv::Point> > contours;
  	std::vector<cv::Vec4i> hierarchy;
	Canny( img, img, 100, 200, 3 );
	findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	cv::Mat drawing = cv::Mat::zeros( img.size(), CV_8UC3 );
	std::vector<std::vector<cv::Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ ) {
		convexHull( cv::Mat(contours[i]), hull[i], false );
	}
  	for( int i = 0; i< contours.size(); i++ )
     	{
       		drawContours( drawing, contours, i, cv::Scalar(50), 2, 8, hierarchy, 0, cv::Point() );
		drawContours( drawing, hull, i, cv::Scalar(255), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
     	}

 	cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  	imshow( "Contours", drawing );
	drawing.copyTo(preprocessedEar);
	//imwrite("../Obrazy/Badaniepreprocessingu/erozja/z.jpg", img);
}

void Ear::segmentation(cv::Mat img)
{
	//cv::erode(img,img,cv::Mat(),cv::Point(-1,-1),30);
	//cv::namedWindow("Eroded Image");
	//cv::imshow("Eroded Image",img);
	//cv::dilate(img,img,cv::MpreprocessedEar),cv::Point(-1,-1),30);
	//cv::namedWindow("Dilated Image");
	//cv::imshow("Dilated Image",img);

	// Eliminate noise and smaller objects
	//cv::Mat fg;
	//cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),6);
}
void Ear::findReference() {
//TODO
	
}




void Ear::execute() {
	isReady = false;
	prep_done = false;
	earCords = findEar();
	extractEar(earCords);
	preprocess();
	//contours(preprocesedEar);
}
	

