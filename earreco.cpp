#include <opencv2/opencv.hpp>
#include "Ear.class.h"
#include "defines.h"
#include "tools.h"
#include <vector>

using namespace std;
int main( int argc, char** argv ) {
#ifdef _VIDEO_CAPTURE_
	cv::VideoCapture cap;
	int counter = 0;
	cv::Mat savedEar;   
 	if(!cap.open(1)) {
		return 0;
	}
	for(;;) {
		cv::Mat frame;
		cap >> frame;
		if( frame.empty() ) break; // end of video stream
 		Ear ear(frame);
		cv::imshow("Ear rocognition", ear.getSelectedEar());
		int key = cv::waitKey(10);
		
		if(ear.isReady) {
			cv::namedWindow("Extracted ear",  cv::WINDOW_NORMAL);
			cv::imshow("Extracted ear", ear.getExtractedEar());
			savedEar = ear.getExtractedEar().clone();
		}
		if(key == 32 && !savedEar.empty()) {
			ostringstream stm;
			stm << counter;
			imwrite("ears2/ear" + stm.str() + ".jpg", savedEar);
			counter++;
		}
		if( key  == 27 ) break; // stop capturing by pressing ESC 
	}
#else // _VIDEO_CAPTURE_

		vector<string> files;
		vector<Ear> ears;

		getFilesInDirectory(argv[1], files);
		for(int i = 0; i < files.size(); i++) {
			ears.push_back(Ear(files[i].data()));
		}

		for(int i = 0; i < ears.size(); i++) {
			if(ears[i].isReady) {
				std::ostringstream stm ;
				stm << i ;
				cv::imwrite("ears/ear" + stm.str() + ".jpg" , ears[i].getExtractedEar());
			}
		}
#endif
	return 0;
}
