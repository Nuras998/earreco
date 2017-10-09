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
	void ApplyGaussianBlur(cv::Mat img, int size);
	void ApplyTreshold(cv::Mat img,int p1, int p2);
	void ImproveContrast(cv::Mat img, int aplha, int beta);
	void NormalizeBrightness(cv::Mat img);
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

