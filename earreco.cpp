#include <opencv2/opencv.hpp>
#include "Structs.h"
#include "Ear.class.h"
#include "Descriptor.class.h"
#include "Comparator.class.h"
#include "defines.h"
#include "tools.h"
#include <vector>
#include <string>

using namespace std;
int main( int argc, char** argv ) {
#ifdef _VIDEO_CAPTURE_
	cv::VideoCapture cap;
	int counter = 0;
	cv::Mat savedEar;
	cv::Mat original;
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
			original = ear.getOriginal().clone();
		}
		if(key == 32 && !savedEar.empty()) {
			ostringstream stm;
			stm << counter;
			imwrite("ears2/ear" + stm.str() + ".jpg", savedEar);
			imwrite("ears3/ear" + stm.str() + ".jpg", original);
			counter++;
		}
		if( key  == 27 ) break; // stop capturing by pressing ESC 
	}
#else // _VIDEO_CAPTURE_
		/*
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
		*/
		cv::Mat conturs1 = cv::imread(argv[1]);
		cv::Mat conturs2 = cv::imread(argv[2]);
		cv::Mat conturs3 = cv::imread(argv[3]);
		cv::Mat conturs4 = cv::imread(argv[4]);
		cv::Mat query_conturs = cv::imread(argv[5]);

		Descriptor descEar1(conturs1);
		Descriptor descEar2(conturs2);
		Descriptor descEar3(conturs3);
		Descriptor descEar4(conturs4);
		Descriptor descEar5(query_conturs);

		descEar1.calcFeature(4, 15);
		descEar2.calcFeature(4, 15);
		descEar3.calcFeature(4, 15);
		descEar4.calcFeature(4, 15);
		descEar5.calcFeature(4, 15);

		Comparator comparator;
		comparator.addRecord(descEar1.getFeature(), 1);
		comparator.addRecord(descEar2.getFeature(), 2);
		comparator.addRecord(descEar3.getFeature(), 3);
		comparator.addRecord(descEar4.getFeature(), 4);

		int ID = comparator.compare(descEar5.getFeature());

		std::cout << "ID: " << ID << std::endl;
//		while(cv::waitKey(10) != 27) { };
#endif
	return 0;
}
