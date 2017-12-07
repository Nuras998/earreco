#include <opencv2/opencv.hpp>
#include <vector>
class Ear {

public:
        Ear();
        Ear(cv::Mat img);
        Ear(const char* fileName);
        cv::Mat getOriginal();
        cv::Mat getSelectedEar();
        cv::Mat getExtractedEar();
	cv::Mat cont;
        void load(const char* fileName);
        bool isReady;
private:
        cv::Rect findEar();
        void findReference();
        void execute();
        void calcHaarFeature(cv::Mat img);
        void extractEar(cv::Rect cords);
	void improveContrast(cv::Mat img);
	void houghLines(cv::Mat img);
	static bool less_vectors(const std::vector<cv::Point_<int> >& a, const std::vector<cv::Point_<int> >& b);
	void contours(cv::Mat img, cv::Mat src);

        static cv::CascadeClassifier earClassifier;
        cv::Mat originalImg;
        cv::Mat earSelected;
        cv::Mat extractedEar;
        cv::Rect earCords;
};

