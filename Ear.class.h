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
	cv::Mat getPreprocessedEar();
	cv::Mat getBlurredEar();
	cv::Mat getResizedEar();
	cv::Mat getContrastEar();
	cv::Mat getSharpenedEar();
	cv::Mat cont;
        void load(const char* fileName);
        bool isReady;
private:
        cv::Rect findEar();
        void findReference();
        void execute();
        void calcHaarFeature(cv::Mat img);
        void extractEar(cv::Rect cords);
	void preprocess();
	static bool less_vectors(const std::vector<cv::Point_<int> >& a, const std::vector<cv::Point_<int> >& b);
	void contours();
	//bool R1(int R, int G, int B);
	//bool R2(float Y, float Cr, float Cb);
	//bool R3(float H, float S, float V);
	static cv::Mat GetSkin(cv::Mat const &src);

        static cv::CascadeClassifier earClassifier;
        cv::Mat originalImg;
        cv::Mat earSelected;
        cv::Mat extractedEar;
	cv::Mat preprocessedEar;
	cv::Mat blurredEar;
	cv::Mat resizedEar;
	cv::Mat contrastEar;
	cv::Mat sharpenedEar;
	cv::Rect earCords;
};

