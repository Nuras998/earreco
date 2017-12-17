#include <opencv2/opencv.hpp>
#include "Ear.class.h"
#include "defines.h"
#include "tools.h"
#include <vector>

using namespace std;
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
			imwrite("tutaj/ucho.jpg", ear.getOriginal());
		}
		else {
			//cv::destroyWindow("Extracted ear");
		}
		if( cv::waitKey(10) == 27 ) break; // stop capturing by pressing ESC 
	}
#else // _VIDEO_CAPTURE_
		vector<string> files;
		vector<Ear> ears;

		getFilesInDirectory(argv[1], files);
		for(int i = 0; i < files.size(); i++) {
			ears.push_back(Ear(files[i].data()));
		}
		for(int i = 0; i < files.size(); i++) {
			cout << files[i].data() << endl;
		}

		for(int i = 0; i < ears.size(); i++) {
			if(ears[i].isReady) {
				std::ostringstream stm ;
				stm << i ;
				cv::imwrite("ears/krawedzie/ear" + stm.str() + ".jpg" , ears[i].getExtractedEar());
				//cv::imwrite("ears/wyostrzenie/ear" + stm.str() + ".jpg" , ears[i].getSharpenedEar());
				cv::imwrite("ears/rozmycie/ear" + stm.str() + ".jpg" , ears[i].getBlurredEar());
				cv::imwrite("ears/kontrast/ear" + stm.str() + ".jpg", ears[i].getContrastEar());
				cv::imwrite("ears/rozmiar/ear" + stm.str() + ".jpg" , ears[i].getResizedEar());}
		}
#endif
	return 0;
}
