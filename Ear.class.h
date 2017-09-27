#include <opencv2/opencv.hpp>
#include <vector>
class Ear {



public:
	static cv::CascadeClassifier earClassifier;
	Ear();
	Ear(cv::Mat img);
	Ear(const char* fileName);
	cv::Mat getOriginal();
	cv::Mat getSelectedEar();
	cv::Mat getExtractedEar();
	void load(const char* fileName);
	cv::Rect findEar();
	void execute();
	void extractEar(cv::Rect ears); 
	bool isReady;
private:
	cv::Mat originalImg;
	cv::Mat earSelected;
	cv::Mat extractedEar;
	cv::Rect earCords;
	void findReference();
	void calcHaarFeature(cv::Mat integralImg);
}; 
