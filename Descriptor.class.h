#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include "Structs.h"

struct FeatureCH {
int dist;
int k;
};
class Descriptor {

public:

Descriptor();
Descriptor(cv::Mat input);
void calcFeature(int nummOfCircles, int tresh);
std::vector< std::vector <Feature> > getFeature();
std::vector<FeatureCH> getFeatureCH();

private:
cv::Mat conturs;
std::vector< std::vector <Feature> > feature; 

std::vector<cv::Point> searchPoints(cv::Mat img, cv::Mat circle, int tresh = 10);
std::vector< std::vector < Feature > > calcPolar(std::vector< std::vector< cv::Point > > points, cv::Point center);

};

