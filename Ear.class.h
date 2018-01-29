#include <opencv2/opencv.hpp>
#include <vector>
#include <ctime>
#include <chrono>
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
	cv::Mat getContrastEar();
	cv::Mat getEdges2Img();
	cv::Mat getEdgesCannyImg();
	cv::Mat getEdgesLaplacianImg();
	cv::Mat cont;
        void load(const char* fileName);
        bool isReady;
	bool preprocessingOk;
private:
        cv::Rect findEar();
        void findReference();
        void execute();
        void calcHaarFeature(cv::Mat img);
        void extractEar(cv::Rect cords);
	void preprocess();
	double calcMean(cv::Mat img);
	double calcStandardDeviation(cv::Mat img);
	void findEdges(cv::Mat img);
	static bool shorter(const std::vector<cv::Point_<int> >& a, const std::vector<cv::Point_<int> >& b);
	std::vector<std::vector<cv::Point_<int> > > contoursFind(cv::Mat img, int minLength);
	std::vector<std::vector<cv::Point_<int> > > getNotOverlappingContours(std::vector<std::vector<cv::Point_<int> > > v1, std::vector<std::vector<cv::Point_<int> > > v2);
	bool checkContours(std::vector<std::vector<cv::Point_<int> > > contours);
	void drawingContours(std::vector<std::vector<cv::Point_<int> > > contours);
	//bool R1(int R, int G, int B);
	//bool R2(float Y, float Cr, float Cb);
	//bool R3(float H, float S, float V);
	static cv::Mat GetSkin(cv::Mat const &src);

        static cv::CascadeClassifier earClassifier;
        cv::Mat originalImg;
        cv::Mat earSelected;
        cv::Mat extractedEar;
	cv::Mat extractedEarPadding;
	cv::Mat preprocessedEar;
	cv::Mat blurredEar;
	cv::Mat contrastEar;
	cv::Mat edges2Img;
	cv::Mat edgesCannyImg;
	cv::Mat edgesLaplacian;
	cv::Mat conturs2Img;
	cv::Mat contoursCannyImg;
	cv::Mat contoursLaplacian;
	cv::Rect earCords;
};

