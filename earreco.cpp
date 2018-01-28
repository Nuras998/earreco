#include <opencv2/opencv.hpp>
#include "Structs.h"
#include "Ear.class.h"
#include "Descriptor.class.h"
#include "Comparator.class.h"
#include "defines.h"
#include "tools.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;
int main( int argc, char** argv ) {
#ifdef _VIDEO_CAPTURE_
	cv::VideoCapture cap;
	Comparator comparator;
	comparator.loadRecords();
 	if(!cap.open(1)) {
		return 0;
	}
	for(;;) {
		int key = cv::waitKey(20);
		cv::Mat frame;
		cap >> frame;
		
		if( frame.empty() ) break; // end of video stream
 		Ear ear(frame);
		cv::Mat image = ear.getSelectedEar().clone();

		cv::imshow("earreco", image);
		if(ear.isReady) {
			cv::Mat prep = ear.getPreprocessedEar();
			cv::imshow("conturs", prep);
			if(key == 32) {
				Descriptor desc(prep);
				desc.calcFeature(4, 15);
				int key = 0;
				while(1) {
					int key = cv::waitKey(10);
					if(key == 115) {
						cv::putText(image, "Przypisz id", cv::Point(20, image.rows - 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0), 6);
						cv::imshow("earreco", image);
						int id = cv::waitKey(0);
						id -= 48;
						comparator.addRecord(desc.getFeature(), id);
						break;
					}
					else if(key == 99) {
						int match_id = comparator.compare(desc.getFeature());
						ostringstream ss;
						ss << match_id;
						string str = ss.str();
						cv::putText(image, "Znaleziono id: " + str, cv::Point(20, image.rows - 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0), 6);
						cv::imshow("earreco", image);
					}
						
					if(key == 27) break;
				}
					
			}
			
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
		*/
		
		/*
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

		//comparator.loadRecords();
		//comparator.saveRecords();
		//int ID = comparator.compare(descEar5.getFeature());

		//std::cout << "ID: " << ID << std::endl;
		*/
		Ear ear("ear0.jpg");
		
		if(ear.preprocessingOk) {
			std::cout << "preprocessing poprawny" << std::endl;
			cv::imshow("conturs", ear.getPreprocessedEar());
			while(cv::waitKey(10) != 27) {};
		} else {
			std::cout << "preprocessing niepoprawny" << std::endl;
			cv::imshow("conturs", ear.getPreprocessedEar());
                        while(cv::waitKey(10) != 27) {};
		}
#endif
	return 0;
}
