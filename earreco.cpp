#include <opencv2/opencv.hpp>
#include "Ear.class.h"
#include "defines.h"

int main( int argc, char** argv ) {
#ifdef _VIDEO_CAPTURE_
	cv::VideoCapture cap;   
 	if(!cap.open(0)) {
		return 0;
	}
	for(;;) {
		cv::Mat frame;
		cap >> frame;
		if( frame.empty() ) break; // end of video stream
 		Ear ear(frame);
		cv::imshow("Ear rocognition", ear.getSelectedEar());
		if(ear.isReady) {
			cv::namedWindow("Extracted ear",  cv::WINDOW_NORMAL);
			cv::imshow("Extracted ear", ear.getExtractedEar());
			imwrite("ear.jpg", ear.getExtractedEar());
		}
		else {
			//cv::destroyWindow("Extracted ear");
		}
		if( cv::waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
	}
#else // _VIDEO_CAPTURE_
		Ear ear("images/ucho10.jpg");
		Ear prep_ear = ear;
		cv::imshow("Ear rocognition", ear.getSelectedEar());
		if(ear.isReady) {
			cv::imshow("Extracted ear", ear.getExtractedEar());
			imwrite("ear.jpg", ear.getExtractedEar());
		}
		else {
			//cv::destroyWindow("Extracted ear");
		}
		if(ear.prep_done) {
			//cv::namedWindow("Ear after preprocessing", cv::WINDOW_NORMAL);
			//cv::imshow("Ear after preprocessing", prep_ear.getPreprocessedEar());
			imwrite("prep_ear.jpg", prep_ear.getPreprocessedEar());
		}
		while( cv::waitKey(10) != 27 ) {

		}
#endif
	return 0;
}
