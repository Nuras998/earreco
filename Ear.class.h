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
	cv::Mat getPreprocessedEar();
	void segmentation(cv::Mat);
	void ApplyGaussianBlur(cv::Mat img, int size);
	void ApplyThreshold(cv::Mat img,int p1, int p2);
	void ImproveContrast(cv::Mat img, int aplha, int beta);
	void NormalizeBrightnessMean(cv::Mat img);
	void NormalizeBrightnessMedian(cv::Mat img);
	void NormalizeBrightnessAddConst(cv::Mat img);
	double calcThresholdMean(cv::Mat img);
	double calcThresholdMedian(cv::Mat img);
	int calcMedian(cv::Mat img);
	void contours(cv::Mat img);
	void load(const char* fileName);
	cv::Rect findEar();
	void execute();
	void extractEar(cv::Rect ears);
	void preprocess(); 
	bool isReady;
	bool prep_done; //flag used to indicate whether preprocessing is done
private:
	cv::Mat originalImg;
	cv::Mat earSelected;
	cv::Mat extractedEar;
	cv::Mat preprocessedEar;
	cv::Rect earCords;
	void findReference();
}; 

