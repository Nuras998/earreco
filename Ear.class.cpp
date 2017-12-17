
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
cv::Mat Ear::getBlurredEar() {
	return blurredEar;
}
cv::Mat Ear::getResizedEar() {
	return resizedEar;
}
cv::Mat Ear::getContrastEar() {
	return contrastEar;
}
cv::Mat Ear::getSharpenedEar() {
	return sharpenedEar;
}

cv::Rect Ear::findEar() {
	std::vector<cv::Rect> ears;
	earSelected = originalImg.clone();
	//earClassifier.detectMultiScale(originalImg, ears);
	cv::Rect cords(0,0,0,0);
	if(ears.size() == 1) {
		cords = ears[0];
		cords.x -= cords.width * 0.25;
		cords.y -= cords.height * 0.17;
		cords.width += cords.width * 0.21;
		cords.height += cords.height * 0.21;
		if(cords.x < 0 || cords.y < 0 || cords.x + cords.width > earSelected.cols || cords.y + cords.height > earSelected.rows) {
		cords.x = 0;
		cords.y = 0;
		cords.width = 0 ;
		cords.height = 0;
		}
		cv::rectangle(earSelected, cords, CV_RGB(255,0,0),2);	
	}
	return cords;	
}
bool Ear::less_vectors(const std::vector<cv::Point_<int> >& a,const std::vector<cv::Point_<int> >& b) {
	double first = cv::contourArea(a, false );
	double second =  cv::contourArea(b, false);
	return (first < second) && a.size() < b.size();
	//return arcLength(a, false) < arcLength(b,false);
}
void Ear::contours() {
	//std::cout << preprocessedEar.depth() << std::endl;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( preprocessedEar, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );

	//for
	/*std::vector<std::vector<cv::Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{  convexHull( cv::Mat(contours[i]), hull[i], false ); }

	cv::Mat drawing = cv::Mat::zeros( preprocessedEar.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ ) {
		cv::Scalar color = cv::Scalar( 50,50,50 );
		//drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
		drawContours( drawing, hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	}*/

	std::sort(contours.begin(),contours.end(), less_vectors);
	std::vector<cv::Point> singleContour = contours[contours.size()-1];
	std::vector<std::vector<cv::Point> > vecContours;
	vecContours.push_back(singleContour);
	cv::Mat picSingleContour = cv::Mat::zeros(preprocessedEar.size(), CV_8UC3);
	cv::Vec3b myColor(255,0,0);
	std::vector<std::vector<cv::Point> >hullSingle(vecContours.size());
        convexHull( cv::Mat(vecContours[0]), hullSingle[0], false );
	cv::cvtColor(preprocessedEar,preprocessedEar,CV_GRAY2BGR);
	drawContours( preprocessedEar, hullSingle, 0, myColor, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	drawContours( extractedEar, hullSingle, 0, myColor, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
}

bool R1(int R, int G, int B) {
	bool e1 = (R>95) && (G>40) && (B>20) && ((cv::max(R,cv::max(G,B)) - cv::min(R, cv::min(G,B)))>15) && (abs(R-G)>15) && (R>G) && (R>B);
	bool e2 = (R>220) && (G>210) && (B>170) && (abs(R-G)<=15) && (R>B) && (G>B);
	return (e1||e2);
}

bool R2(float Y, float Cr, float Cb) {
	bool e3 = Cr <= 1.5862*Cb+20;
	bool e4 = Cr >= 0.3448*Cb+76.2069;
	bool e5 = Cr >= -4.5652*Cb+234.5652;
	bool e6 = Cr <= -1.15*Cb+301.75;
	bool e7 = Cr <= -2.2857*Cb+432.85;
	return e3 && e4 && e5 && e6 && e7;
}

bool R3(float H, float S, float V) {
	return (H<25) || (H > 230);
}

cv::Mat Ear::GetSkin(cv::Mat const &src) {
	// allocate the result matrix
	cv::Mat dst = src.clone();

	cv::Vec3b cwhite = cv::Vec3b::all(255);
	cv::Vec3b cblack = cv::Vec3b::all(0);

	cv::Mat src_ycrcb, src_hsv;
	// OpenCV scales the YCrCb components, so that they
	// cover the whole value range of [0,255], so there's
	// no need to scale the values:
	cvtColor(src, src_ycrcb, CV_BGR2YCrCb);
	// OpenCV scales the Hue Channel to [0,180] for
	// 8bit images, so make sure we are operating on
	// the full spectrum from [0,360] by using floating
	// point precision:
	src.convertTo(src_hsv, CV_32FC3);
	cvtColor(src_hsv, src_hsv, CV_BGR2HSV);
	// Now scale the values between [0,255]:
	normalize(src_hsv, src_hsv, 0.0, 255.0, cv::NORM_MINMAX, CV_32FC3);

	for(int i = 0; i < src.rows; i++) {
		for(int j = 0; j < src.cols; j++) {

		cv::Vec3b pix_bgr = src.ptr<cv::Vec3b>(i)[j];
		int B = pix_bgr.val[0];
		int G = pix_bgr.val[1];
		int R = pix_bgr.val[2];
		// apply rgb rule
		bool a = R1(R,G,B);

		cv::Vec3b pix_ycrcb = src_ycrcb.ptr<cv::Vec3b>(i)[j];
		int Y = pix_ycrcb.val[0];
		int Cr = pix_ycrcb.val[1];
		int Cb = pix_ycrcb.val[2];
		// apply ycrcb rule
		bool b = R2(Y,Cr,Cb);

		cv::Vec3f pix_hsv = src_hsv.ptr<cv::Vec3f>(i)[j];
		float H = pix_hsv.val[0];
		float S = pix_hsv.val[1];
		float V = pix_hsv.val[2];
		// apply hsv rule
		bool c = R3(H,S,V);

		if(!(a&&b&&c))
			dst.ptr<cv::Vec3b>(i)[j] = cblack;
		}
	}
	return dst;
}

void Ear::preprocess() {
	//cv::cvtColor(extractedEar,extractedEar,CV_BGR2GRAY);
	preprocessedEar = extractedEar.clone();
	preprocessedEar.convertTo(preprocessedEar, -1, 0.75,0);
	preprocessedEar = GetSkin(preprocessedEar).clone();
	cv::cvtColor(preprocessedEar,preprocessedEar, CV_BGR2GRAY);
	int erosion_size = 2;
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
              cv::Point(erosion_size, erosion_size) );
	dilate(preprocessedEar,preprocessedEar,element);
	equalizeHist(preprocessedEar,preprocessedEar);
	contrastEar = preprocessedEar.clone();

	GaussianBlur(preprocessedEar,preprocessedEar, cv::Size(25,25), 3,3, cv::BORDER_DEFAULT );
	medianBlur(preprocessedEar,preprocessedEar,5);
	blurredEar = preprocessedEar.clone();
	resize(preprocessedEar,preprocessedEar, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC );
	//resize(preprocessedEar,preprocessedEar, cv::Size(), 2,2, cv::INTER_CUBIC );
	resizedEar = preprocessedEar.clone();
	//preprocessedEar.convertTo(preprocessedEar, -1, 1.5, -80);
	//cv::GaussianBlur(preprocessedEar, sharpenedEar, cv::Size(0, 0), 3);
	//cv::addWeighted(preprocessedEar, 2, sharpenedEar, -1, 0, sharpenedEar);
	//preprocessedEar = sharpenedEar.clone();

	//cv::cvtColor(preprocessedEar,preprocessedEar, CV_BGR2GRAY);
	Canny(preprocessedEar,preprocessedEar,30,90,3);
	resize(preprocessedEar,preprocessedEar, cv::Size(), 2,2, cv::INTER_CUBIC );
	//dilate(preprocessedEar,preprocessedEar,element);
	contours();
}
void Ear::extractEar(cv::Rect cords) {
	//if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	extractedEar = originalImg.clone();
	preprocess();
	isReady = true;
	//}
}

void Ear::findReference() {
//TODO
	
}




void Ear::execute() {
	isReady = false;
	earCords = findEar();
	extractEar(earCords);
}
	

