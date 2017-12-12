
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
bool Ear::less_vectors(const std::vector<cv::Point_<int> >& a,const std::vector<cv::Point_<int> >& b) {
	double first = cv::contourArea(a, false );
	double second =  cv::contourArea(b, false);
	return (first < second) && a.size() < b.size();
}
void Ear::contours(cv::Mat img, cv::Mat src) {
	cv::cvtColor(src,src,CV_GRAY2RGB);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	std::vector<std::vector<cv::Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{  convexHull( cv::Mat(contours[i]), hull[i], false ); }

	cv::Mat drawing = cv::Mat::zeros( img.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ ) {
		cv::Scalar color = cv::Scalar( 50,50,50 );
		//drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
		drawContours( drawing, hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	}
	cv::namedWindow("allContours", CV_WINDOW_AUTOSIZE);
	imshow("allContours", drawing);


	std::sort(contours.begin(),contours.end(), less_vectors);
	std::vector<cv::Point> singleContour = contours[contours.size()-1];
	std::vector<std::vector<cv::Point> > vecContours;
	vecContours.push_back(singleContour);
	cv::Mat picSingleContour = cv::Mat::zeros(img.size(), CV_8UC3);
	cv::Vec3b myColor(255,0,0);
	std::vector<std::vector<cv::Point> >hullSingle(vecContours.size() );
        convexHull( cv::Mat(vecContours[0]), hullSingle[0], false );

	drawContours( src, hullSingle, 0, myColor, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", src );
	imwrite("Contours.jpg", src);
}

void Ear::improveContrast(cv::Mat img) {
	for( int y = 0; y < img.rows; y++ ) {
                for( int x = 0; x < img.cols; x++ ) {
                        img.at<uchar>(y,x) = cv::saturate_cast<uchar>(  img.at<uchar>(y,x) + 10 );
                }
        }

	double alpha = 1.0;
	for( int y = 0; y < img.rows; y++ ) {
		for( int x = 0; x < img.cols; x++ ) {
			img.at<uchar>(y,x) = cv::saturate_cast<uchar>( alpha*( img.at<uchar>(y,x)-90 ) );
		}
	}
	cv::namedWindow( "Contrast", CV_WINDOW_AUTOSIZE );
        imshow( "Contrast", img );
}
void Ear::NormalizeBrightnessAddConst(cv::Mat img) {
	int value = 0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			value = (int) img.at<uchar>(i,j);
			value += 10;
			if(value > 255)
				value = 255;
			if(value < 0)
				value = 0;
			img.at<uchar>(i,j) = value;
		}
	}
}
double Ear::calcThresholdMedian(cv::Mat img) {
	int median = calcMedian(img);
	return 0.9 * median;
}

int Ear::calcMedian(cv::Mat img) {
	std::vector<int> pixels;
	int median = 0;
	for(int i=0; i<img.rows; i++) {
                for(int j=0; j<img.cols; j++) {
                        pixels.push_back( (int) img.at<uchar>(i, j));
                }
        }
	std::sort(pixels.begin(), pixels.end(), std::greater<int>() ); //sortowanie od najmniejszego do największego
	//for(int i=0; i<pixels.size(); i++)
		//std::cout << pixels.at(i) << std::endl;
	if( pixels.size() % 2 != 0 ) //jeżeli liczba pikseli nieparzysta
		median = pixels.at( (pixels.size()+1)/2);
	else if(pixels.size() % 2 == 0) //l pikseli parzysta
		median =( ( (int) floor( pixels.at( (pixels.size()+1)/2) ) + (int) ceil( pixels.at( (pixels.size()+1)/2) ) ) / 2);
	return median;
}
void Ear::preprocess(cv::Mat img) {
	cv::cvtColor(img,img, CV_RGB2GRAY);
	cv::Mat src = img.clone();
	NormalizeBrightnessAddConst(img);
	GaussianBlur( img, img, cv::Size(25,25), 0, 0 );
	double thresh = calcThresholdMedian(img);
	cv::namedWindow( "Before binarization", CV_WINDOW_AUTOSIZE );
	imshow( "Before binarization",img);
	threshold( img, img, thresh, 255,CV_THRESH_BINARY );
	cv::namedWindow("After thresholding", CV_WINDOW_AUTOSIZE);
	imshow("After thresholding", img);
	Canny( img,img, 100, 200, 3 );
	contours(img,src);
}
void Ear::preprocess2(cv::Mat img) {
	cv::cvtColor(img,img, CV_RGB2GRAY);
	cv::Mat src = img.clone();
	int erosion_size =7;
	cv::Mat erodeElement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
              cv::Point(erosion_size, erosion_size) );
	int dilatation_size =2;
        cv::Mat dilateElement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * dilatation_size + 1, 2 * dilatation_size + 1),
              cv::Point(dilatation_size, dilatation_size) );

	erode(img,img,erodeElement);
	medianBlur(img,img, 15);
	dilate(img,img,dilateElement);
	cv::namedWindow( "Erode dilate", CV_WINDOW_AUTOSIZE );
        imshow( "Erode dilate", img );
	improveContrast(img);
	imwrite("tylkodylatacjaerozja.jpg",img);
	int kernel_size = 7;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_8U;
	Laplacian( img,img, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT );
	fastNlMeansDenoising(img,img, 9, 9,21 );

	medianBlur(img,img, 3);
	contours(img,src);
}
void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	preprocess(extractedEar);
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
	

