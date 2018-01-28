#include "Descriptor.class.h"

Descriptor::Descriptor() {}

Descriptor::Descriptor(cv::Mat input) {
	conturs = input.clone();
}

void Descriptor::calcFeature(int numbOfCircles, int tresh) {


	int radius = (conturs.rows - 10) /2;
	int step = radius/numbOfCircles;
	cv::Point center(conturs.cols/2, conturs.rows/2); 

	cv::Mat copy = conturs.clone();
	conturs.convertTo(conturs, 0);
	//copy.convertTo(copy, 18);
	//cvtColor(copy, copy, cv::COLOR_GREY2BGR);
	std::vector< std::vector< cv::Point > > points;

	for (int i = 0; i < numbOfCircles; i++)	{

	cv::Mat circle(conturs.rows, conturs.cols, 0, cv::Scalar(0));
	cv::circle(circle, cv::Point(center.x, center.y), step + i*step, cv::Scalar(255));
	cv::circle(copy, cv::Point(center.x, center.y), step + i*step, cv::Scalar(255, 0, 0));
	points.push_back(searchPoints(conturs, circle, tresh));

	}
	
	feature = calcPolar(points, center);
	
	for(int i = 0; i < points.size(); i++) {
		for(int j = 0 ; j < points[i].size();  j++) {
			cv::line(copy, cv::Point(center.x, center.y), cv::Point(points[i][j].x, points[i][j].y), cv::Scalar(255));
		}
	}
	
	for(int i = 0; i < feature.size(); i++) {
//		std::cout << "Vector " << i+1 << std::endl;
		for(int j = 0 ; j < feature[i].size();  j++) {
//			std::cout << "X: " << feature[i][j].x << " Y: " << feature[i][j].y << " Angle: " << feature[i][j].angle << " Radius: " <<  feature[i][j].radius << std::endl;
		}
//		std::cout << "---------------------------------------" << std::endl << std::endl;
	}

//	cv::imshow("copy", copy);
//	cv::imwrite("cricle.jpg", copy);
}

std::vector< cv::Point > Descriptor::searchPoints(cv::Mat img, cv::Mat circle, int tresh) {	
	std::vector< cv::Point > points;
	for(int i = 0 ; i < img.rows; i++) {
		for(int j = 0; j < img.cols; j++) {
			if(circle.at<uchar>(i, j) > 100 && img.at<uchar>(i, j) > 100) {

				cv::Point point(j, i);
				bool pointOut = false;

				for(int h = 0; h < points.size(); h++) {
					if(std::abs(point.x - points[h].x) < tresh  && std::abs(point.y - points[h].y) < tresh) 
					pointOut = true;
				}
				if(!pointOut) {
					points.push_back(point);
				}							
			}
		}
	}
	return points;
}

std::vector<std::vector < Feature > > Descriptor::calcPolar(std::vector< std::vector< cv::Point > > points, cv::Point center) {
	std::vector<Feature> feat;
	std::vector<Feature> sorted;
	std::vector< std::vector< Feature > > features;
	
	for(int i = 0; i < points.size(); i++) {
		for(int j = 0; j < points[i].size(); j++) {
			Feature feature;
			feature.x = points[i][j].x - center.x;
			feature.y = -(points[i][j].y - center.y);	
			feature.angle = atan2(feature.y, feature.x) * 180/3.141592f;
			feature.radius = sqrt(pow(feature.x, 2) + pow(feature.y, 2));
			if(feature.angle < 0) 
				feature.angle = 360 + feature.angle;
			feat.push_back(feature);
		}
	while(feat.size() != 0) {
		int an = 360;
		int index = 0;
		for(int i = 0; i < feat.size(); i++) {
			if(feat[i].angle < an) {
				an = feat[i].angle;
				index = i;
			}
		}
	
	sorted.push_back(feat[index]);
	feat.erase(feat.begin()+index);
	}
	features.push_back(sorted);
	feat.clear();
	sorted.clear();
	}
	return features;	
}

std::vector<std::vector < Feature > > Descriptor::getFeature() { 
	return feature;
}

std::vector<FeatureCH> Descriptor::getFeatureCH() { 
	std::vector<FeatureCH> featurech;
	
	for(int i = 0; i < feature.size(); i++) {
		int distance = 0;
		FeatureCH feat;
		feat.dist = 0;
		feat.k = 0;
		if(feature[i].size() == 0) {
			featurech.push_back(feat);
			continue;
		}
		for(int j = 0 ; j < feature[i].size() - 1;  j++) {
			int a = sqrt(pow(feature[i][j].x - feature[i][j+1].x, 2) + pow(feature[i][j].y - feature[i][j+1].y, 2));
			distance += a;
		}
	feat.dist = distance;
	feat.k = feature[i].size();

	featurech.push_back(feat);
	
	}
//	for(int i = 0; i < featurech.size(); i++) {
//		std::cout << "Distance: " << featurech[i].dist << " Interrsectios: " << featurech[i].k << std::endl;
//	}	
	return featurech;
}
























