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
			imwrite("ear.jpg", ear.getExtractedEar());
			imwrite("baza/ucho.jpg", ear.getOriginal());
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
		for(int i = 0; i < files.size(); i++) {
			//cout << files[i].data() << endl;
		}

		for(int i = 0; i < ears.size(); i++) {
			if(ears[i].isReady) {
				std::ostringstream stm ;
				stm << i ;
				cv::imwrite("ears/krawedzie/ear" + stm.str() + ".jpg" , ears[i].getPreprocessedEar());
				cv::imwrite("ears/krawedzieNasze/ear" + stm.str() + ".jpg" , ears[i].getEdges2Img());
				//cv::imwrite("ears/wyostrzenie/ear" + stm.str() + ".jpg" , ears[i].getSharpenedEar());
				cv::imwrite("ears/rozmycie/ear" + stm.str() + ".jpg" , ears[i].getBlurredEar());
				cv::imwrite("ears/kontrast/ear" + stm.str() + ".jpg", ears[i].getContrastEar());
				cv::imwrite("ears/rozmiar/ear" + stm.str() + ".jpg" , ears[i].getResizedEar());}
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
//		comparator.addRecord(descEar1.getFeature(), 1);
//		comparator.addRecord(descEar2.getFeature(), 2);

		comparator.loadRecords();
		comparator.saveRecords();
		//int ID = comparator.compare(descEar5.getFeature());

		//std::cout << "ID: " << ID << std::endl;
		/*
		Ear ear("ear0.jpg");
		
		if(ear.preprocessingOk) {
			std::cout << "preprocessing poprawny" << std::endl;
			cv::imshow("conturs", ear.getPreprocessedEar());
			while(cv::waitKey(10) != 27) {};
		} else {
			std::cout << "preprocessing niepoprawny" << std::endl;
			cv::imshow("conturs", ear.getPreprocessedEar());
                        while(cv::waitKey(10) != 27) {};
		*/
#endif
	return 0;
}
