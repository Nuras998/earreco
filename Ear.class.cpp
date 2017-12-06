
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
bool Ear::less_vectors(const std::vector<cv::Point_<int> >& a,const std::vector<cv::Point_<int> >& b) {
	//return a.size() < b.size();
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
	//for(int i=0;i<singleContour.size();i++)
	//	picSingleContour.at<cv::Vec3b>(singleContour[i].x,singleContour[i].y)=myColour;
	std::vector<std::vector<cv::Point> >hullSingle(vecContours.size() );
        convexHull( cv::Mat(vecContours[0]), hullSingle[0], false );

                //drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
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
void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	cv::cvtColor(extractedEar, extractedEar, CV_RGB2GRAY);
	cv::Mat src = extractedEar.clone();
	//!GaussianBlur( extractedEar, extractedEar, cv::Size( 11,11 ), 0, 0 );
	//GaussianBlur( extractedEar, extractedEar, cv::Size( 25,25 ), 0, 0 );
	//GaussianBlur( extractedEar, extractedEar, cv::Size( 25,25 ), 0, 0 );
	//medianBlur(extractedEar, extractedEar, 25);
	int erosion_size =7;
	cv::Mat erodeElement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
              cv::Point(erosion_size, erosion_size) );
	int dilatation_size =2;
        cv::Mat dilateElement = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * dilatation_size + 1, 2 * dilatation_size + 1),
              cv::Point(dilatation_size, dilatation_size) );

	erode(extractedEar,extractedEar,erodeElement);
	//erode(extractedEar,extractedEar,erodeElement);
	//erode(extractedEar,extractedEar,erodeElement);
	//erode(extractedEar,extractedEar,erodeElement);
	//erode(extractedEar,extractedEar,erodeElement);
	medianBlur(extractedEar, extractedEar, 15);
	dilate(extractedEar,extractedEar,dilateElement);
	//cv::addWeighted(extractedEar,0.5, extractedEar, 0.5, 10, extractedEar);
	cv::namedWindow( "Erode dilate", CV_WINDOW_AUTOSIZE );
        imshow( "Erode dilate", extractedEar );
	//dilate(extractedEar,extractedEar,erodeElement);
	//dilate(extractedEar,extractedEar,erodeElement);
	improveContrast(extractedEar);
	imwrite("tylkodylatacjaerozja.jpg",extractedEar);
	//Canny(extractedEar, extractedEar, 10, 40, 7);
	//houghLines(extractedEar);
	int kernel_size = 7;
	int scale = 1;
	int delta = 0;
	//int ddepth = CV_16S;
	int ddepth = CV_8U;
	Laplacian( extractedEar, extractedEar, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT );
	//convertScaleAbs( extractedEar, extractedEar );
	//erode(extractedEar, extractedEar,erodeElement);  // dilate(image,dst,element);
	//dilate(extractedEar, extractedEar, erodeElement);
	fastNlMeansDenoising(extractedEar, extractedEar, 9, 9,21 );

	//dilate(extractedEar, extractedEar, erodeElement);
	//medianBlur(extractedEar, extractedEar, 3);
	//erode(extractedEar, extractedEar, erodeElement);
	//dilate(extractedEar, extractedEar, element);
	//Canny(extractedEar, extractedEar, 150, 250, 3);
	contours(extractedEar,src);
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
	

