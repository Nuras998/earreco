
#include "Ear.class.h"
#include <iostream>

extern cv::CascadeClassifier Ear::earClassifier;

Ear::Ear() {
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}
}

Ear::Ear(const char* fileName) {
	originalImg = cv::imread(fileName);
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}
	execute();		
}
Ear::Ear(cv::Mat img) {
	originalImg = img.clone();
	if(earClassifier.empty()) {
		earClassifier.load("classifiers/haarcascade_mcs_leftear.xml");
	}	
	execute();
}

void Ear::load(const char* fileName) {
	originalImg = cv::imread(fileName);
}

cv::Mat Ear::getOriginal() {
	return originalImg;
}

cv::Mat Ear::getSelectedEar() {
	return earSelected;
}
cv::Mat Ear::getExtractedEar() {
	return extractedEar;
}
cv::Mat Ear::getPreprocessedEar() {
	return preprocessedEar;
}
cv::Mat Ear::getBlurredEar() {
	return blurredEar;
}
cv::Mat Ear::getContrastEar() {
	return contrastEar;
}
cv::Mat Ear::getEdges2Img() {
	return edges2Img;
}
cv::Mat Ear::getEdgesGCannyImg() {
	return edgesGCannyImg;
}
cv::Mat Ear::getEdgesBGCannyImg() {
        return edgesBGCannyImg;
}
cv::Mat Ear::getEdgesLaplacianImg() {
	return edgesLaplacian;
}
cv::Rect Ear::findEar() {
	std::vector<cv::Rect> ears;
	earSelected = originalImg.clone();
	earClassifier.detectMultiScale(originalImg, ears);
	cv::Rect cords(0,0,0,0);
	if(ears.size() == 1) {
		cords = ears[0];
		cords.x -= cords.width * 0.08;
		cords.y -= cords.height * 0.12;
		cords.width += cords.width * 0.18;
		cords.height += cords.height * 0.1;
		if(cords.x < 0 || cords.y < 0 || cords.x + cords.width > earSelected.cols || cords.y + cords.height > earSelected.rows) {
			cords.x = 0;
			cords.y = 0;
			cords.width = 0 ;
			cords.height = 0;
		}
		cv::rectangle(earSelected, cords, CV_RGB(255,0,0),2);
	}
	return cords;	
}
bool Ear::shorter(const std::vector<cv::Point_<int> >& a,const std::vector<cv::Point_<int> >& b) {
	return arcLength(a, false) < arcLength(b,false);
}
std::vector<std::vector<cv::Point_<int> > > Ear::contoursFind(cv::Mat img, int minLength) {
	cv::Mat type;
	img.convertTo(type,CV_8UC1);
	type.copyTo(img);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );

	std::sort(contours.begin(),contours.end(), shorter);
	int numCont = contours.size();
	std::vector<std::vector<cv::Point> > contoursBig;
	for(int i=0;i<contours.size();i++) {
		if(arcLength(contours[i],false) > minLength) { //canny 150, choras 250
			contoursBig.push_back(contours[i]);
		}
	}
	return contoursBig;
}

std::vector<std::vector<cv::Point_<int> > > Ear::getNotOverlappingContours(std::vector<std::vector<cv::Point_<int> > > v1, std::vector<std::vector<cv::Point_<int> > > v2) {
	std::vector<std::vector<cv::Point> > contoursNotOverlap;
	std::vector<bool> blocked1(v1.size());
	blocked1.assign(blocked1.size(),false);
	std::vector<bool> blocked2(v2.size());
	blocked2.assign(blocked2.size(),false);
	std::vector<bool> alreadyThere1(v1.size());
	alreadyThere1.assign(alreadyThere1.size(),false);
	std::vector<bool> alreadyThere2(v2.size());
        alreadyThere2.assign(alreadyThere2.size(),false);
	for(int c=0; c<v1.size(); c++) {
		int count=0;
		for(int d=0; d<v2.size(); d++) {
			for(int i=0; i<v1[c].size(); i++) {
				for(int j=0; j<v2[d].size(); j++) {
					for(int k=-2; k<=2;k++) {
						for(int l=-2; l<=2; l++) {
							if( (v1[c][i].x+l)==v2[d][j].x && (v1[c][i].y+k)==v2[d][j].y) {
								count++;
							}
						}
					}
				}
			}
			if(count >= 50) {
				if(arcLength(v1[c],false) > arcLength(v2[d],false)) {
					if(!alreadyThere1[c])
						contoursNotOverlap.push_back(v1[c]);
					alreadyThere1[c]=true;
					blocked2[d]=true;
					for(int m=0;m<contoursNotOverlap.size();m++) {
						if(contoursNotOverlap[m]==v2[d]) {
							contoursNotOverlap.erase(contoursNotOverlap.begin()+m);
							m--;
						}
					}
				} else {
					if(!alreadyThere2[d])
						contoursNotOverlap.push_back(v2[d]);
					alreadyThere2[d]=true;
					blocked1[c]=true;
					for(int n=0;n<contoursNotOverlap.size();n++) {
						if(contoursNotOverlap[n]==v1[c]) {
							contoursNotOverlap.erase(contoursNotOverlap.begin()+n);
							n--;
						}
					}
				}
			} else {
				if(!blocked1[c] && !alreadyThere1[c]) {
					contoursNotOverlap.push_back(v1[c]);
					alreadyThere1[c]=true;
				}
				if(!blocked2[d] && !alreadyThere2[d]) {
					contoursNotOverlap.push_back(v2[d]);
					alreadyThere2[d]=true;
				}
			}
		}
	}
	return contoursNotOverlap;
}
bool Ear::checkContours(std::vector<std::vector<cv::Point_<int> > > contours) {
	std::vector<std::vector<cv::Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
		{  convexHull( cv::Mat(contours[i]), hull[i], false ); }
	std::sort(hull.begin(),hull.end(), shorter);
	std::vector<cv::Point> biggestC=hull[hull.size()-1];
	int cInsideCount=0;

	cv::Mat cImg(hull[hull.size()-1]);
	cv::Mat cImgType;
	cImg.convertTo(cImgType,CV_32F);

	for(int i=0; i<hull.size(); i++) {
		float pointsInsideCount=0;
		for(int j=0; j<hull[i].size(); j++) {
			if(pointPolygonTest(cImgType,hull[i][j],false)==1)
				pointsInsideCount++;
		}
		if(pointsInsideCount>= (float) (0.5*hull[i].size()))
			cInsideCount++;
	}
/*
	cv::Mat drawing = cv::Mat::zeros( preprocessedEar.size(), CV_32F );;
	cv::Scalar gray = cv::Scalar(125,125,125);
	for(int i=0;i<(hull.size()-1);i++) {
		drawContours( drawing, hull, i, gray, 1, 8, cv::noArray(), 0, cv::Point() );
	}
	cv::Scalar white = cv::Scalar( 255,255,255 );
	drawContours( drawing, hull, hull.size()-1, white, 1, 8, cv::noArray(), 0, cv::Point() );
	drawing.copyTo(preprocessedEar);

	std::cout << cInsideCount << std::endl;
*/
	if(cInsideCount>=3)
		return true;
	else
		return false;
}
void Ear::drawingContours(std::vector<std::vector<cv::Point_<int> > > contours) {
	cv::Mat drawing = cv::Mat::zeros( preprocessedEar.size(), CV_32F );
	cv::Mat contoursClosed = cv::Mat::zeros( preprocessedEar.size(), CV_32F );

	for( int i = 0; i< contours.size(); i++ ) {
		cv::Scalar color = cv::Scalar( 255,255,255 );
		drawContours( drawing, contours, i, color, 1, 8, cv::noArray(), 0, cv::Point() );
	}

	drawing.copyTo(preprocessedEar);
}
bool R1(int R, int G, int B) {
	bool e1 = (R>95) && (G>40) && (B>20) && ((cv::max(R,cv::max(G,B)) - cv::min(R, cv::min(G,B)))>15) && (abs(R-G)>15) && (R>G) && (R>B);
	bool e2 = (R>220) && (G>210) && (B>170) && (abs(R-G)<=15) && (R>B) && (G>B);
	return (e1||e2);
}

bool R2(float Y, float Cr, float Cb) {
	bool e3 = Cr <= 1.5862*Cb+20;
	bool e4 = Cr >= 0.3448*Cb+76.2069;
	bool e5 = Cr >= -4.5652*Cb+234.5652;
	bool e6 = Cr <= -1.15*Cb+301.75;
	bool e7 = Cr <= -2.2857*Cb+432.85;
	return e3 && e4 && e5 && e6 && e7;
}

bool R3(float H, float S, float V) {
	return (H<25) || (H > 230);
}

cv::Mat Ear::GetSkin(cv::Mat const &src) {
	// allocate the result matrix
	cv::Mat dst = src.clone();

	cv::Vec3b cwhite = cv::Vec3b::all(255);
	cv::Vec3b cblack = cv::Vec3b::all(0);

	cv::Mat src_ycrcb, src_hsv;
	// OpenCV scales the YCrCb components, so that they
	// cover the whole value range of [0,255], so there's
	// no need to scale the values:
	cvtColor(src, src_ycrcb, CV_BGR2YCrCb);
	// OpenCV scales the Hue Channel to [0,180] for
	// 8bit images, so make sure we are operating on
	// the full spectrum from [0,360] by using floating
	// point precision:
	src.convertTo(src_hsv, CV_32FC3);
	cvtColor(src_hsv, src_hsv, CV_BGR2HSV);
	// Now scale the values between [0,255]:
	normalize(src_hsv, src_hsv, 0.0, 255.0, cv::NORM_MINMAX, CV_32FC3);

	for(int i = 0; i < src.rows; i++) {
		for(int j = 0; j < src.cols; j++) {

		cv::Vec3b pix_bgr = src.ptr<cv::Vec3b>(i)[j];
		int B = pix_bgr.val[0];
		int G = pix_bgr.val[1];
		int R = pix_bgr.val[2];
		// apply rgb rule
		bool a = R1(R,G,B);

		cv::Vec3b pix_ycrcb = src_ycrcb.ptr<cv::Vec3b>(i)[j];
		int Y = pix_ycrcb.val[0];
		int Cr = pix_ycrcb.val[1];
		int Cb = pix_ycrcb.val[2];
		// apply ycrcb rule
		bool b = R2(Y,Cr,Cb);

		cv::Vec3f pix_hsv = src_hsv.ptr<cv::Vec3f>(i)[j];
		float H = pix_hsv.val[0];
		float S = pix_hsv.val[1];
		float V = pix_hsv.val[2];
		// apply hsv rule
		bool c = R3(H,S,V);

		if(!(a&&b&&c))
			dst.ptr<cv::Vec3b>(i)[j] = cblack;
		}
	}
	return dst;
}
double Ear::calcMean(cv::Mat img) {
	int mean = 0;
	for(int i=0;i<img.rows;i++) {
		for(int j=0;j<img.cols;j++) {
			mean+=img.at<uchar>(i,j);
		}
	}
	return mean/pow((img.rows*img.cols),2);
}
double Ear::calcStandardDeviation(cv::Mat img) {
	int deviation = 0;
	int mean = calcMean(img);
	for(int i=0;i<img.rows;i++) {                                  
                for(int j=0;j<img.cols;j++) {
                        deviation += pow((img.at<uchar>(i,j)-mean),2);
                }
        }
	return sqrt(deviation / pow( (img.rows*img.cols), 2) );
}

void Ear::findEdges(cv::Mat img) {
	cv::Mat result=cv::Mat::zeros(img.size(),CV_8UC1);
	cv::Rect patch(7,7,3,3);
	cv::Rect patchNeighbourhood(0,0,17,17);
	for(int y=0;y<(img.rows-16);y++) {
		for(int x=0;x<(img.cols-16);x++) {
			cv::Mat window=img(patch);
			cv::Mat neighbourhood=img(patchNeighbourhood);

			double mean = calcMean(window);
			double standardDeviation = calcStandardDeviation(neighbourhood);
			double k = 0.2;
			double T = mean - k*standardDeviation;
			double min, max;
			cv::minMaxLoc(window, &min, &max);
			double S = max - min;
			if(S>=T) {
				result.at<uchar>(y+8,x+8) = 255;
			} else {
				result.at<uchar>(y+8,x+8) = 0;
			}
			patch.x++;
			patchNeighbourhood.x++;
		}
		patch.x=0;
		patch.y++;
		patchNeighbourhood.x=0;
		patchNeighbourhood.y++;
	}

	result.copyTo(edges2Img);
}
void Ear::preprocess() {
	preprocessingOk = false;
	preprocessedEar = extractedEar.clone();
	cv::Mat fixedSize;
	resize(preprocessedEar,fixedSize, cv::Size(180,250), 0,0, cv::INTER_CUBIC );
	contrastEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
	blurredEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
	cv::Mat bw=cv::Mat::zeros(fixedSize.size(),CV_32F);
	cv::cvtColor(fixedSize,bw, CV_BGR2GRAY);
	int erosion_size = 1;
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
              cv::Point(erosion_size, erosion_size) );

	std::vector<cv::Mat> images;
	std::vector<std::vector<cv::Point> > contoursGCanny;
	std::vector<std::vector<cv::Point> > contoursBGCanny;
	std::vector<std::vector<cv::Point> > contours2method;
	std::vector<std::vector<cv::Point> > contoursLaplacian;
	std::vector<std::vector<cv::Point> > contours;
	for(int i = 0; i<1;i++) {
		if(i==0) { //Gauss Canny
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
			//clahe->setClipLimit(4);
			//clahe->apply(bw,contrastEar);
			equalizeHist(bw,contrastEar);
			GaussianBlur(contrastEar,blurredEar, cv::Size(0, 0), 5);
			cv::Mat edges=cv::Mat::zeros(fixedSize.size(),CV_32F);
			Canny(blurredEar,edges,10,30,7);

			edges.copyTo(edgesGCannyImg);
			contoursGCanny = contoursFind(edgesGCannyImg,150);

		} else if(i==1) { //Bilateral Gauss Canny
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
			//clahe->setClipLimit(4);
			//clahe->apply(bw,contrastEar);
			equalizeHist(bw,contrastEar);
			int size = 15;
			cv::Mat partBlurred;
			bilateralFilter ( contrastEar,partBlurred, size, size*2, size/2 );
			GaussianBlur(partBlurred,blurredEar, cv::Size(0, 0), 5);
			cv::Mat edges=cv::Mat::zeros(fixedSize.size(),CV_32F);
			Canny(blurredEar,edges,10,30,7);

			edges.copyTo(edgesBGCannyImg);
			contoursBGCanny = contoursFind(edgesBGCannyImg,150);

		} else if(i==2) { //Choras
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			GaussianBlur(bw,blurredEar, cv::Size(0, 0), 3);
			findEdges(bw);
			medianBlur(edges2Img,blurredEar,7);
			blurredEar.copyTo(edges2Img);
			cv::Mat edgesFinal=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//Canny(edges2Img,edgesFinal,10,30,7);
			//edgesFinal.copyTo(edges2Img);
			//contours2method = contoursFind(edges2Img,350);
		} else if(i==3) { //Laplacian
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			equalizeHist(bw,contrastEar);
			GaussianBlur(contrastEar,blurredEar, cv::Size(0, 0), 3);
			cv::Mat scalledDown;
			resize(blurredEar,scalledDown, cv::Size(), 0.5,0.5, cv::INTER_CUBIC );
			cv::Mat edges=cv::Mat::zeros(fixedSize.size(),CV_32F);
			Laplacian(scalledDown,edges,CV_32F,3,1,0,cv::BORDER_DEFAULT );
			cv::Mat scale;
			resize(edges,scale, fixedSize.size(), 0,0, cv::INTER_CUBIC );
			threshold(scale,edgesLaplacian,15,255,CV_THRESH_BINARY);
			cv::Mat mb;
			medianBlur(edgesLaplacian,mb,3);
			//edges.copyTo(edgesLaplacian);
			//contoursLaplacian = contoursFind(edgesLaplacian,250);
			//mb.copyTo(preprocessedEar);
		}
	//images.push_back(preprocessedEar);
	}
	//contours = getNotOverlappingContours(contoursCanny,contours2method);
	for(int i=0;i<contoursGCanny.size();i++)
                contours.push_back(contoursGCanny[i]);
        for(int i=0;i<contoursBGCanny.size();i++)
                contours.push_back(contoursBGCanny[i]);
	if(contours.size()>0)
		preprocessingOk = checkContours(contours);
	else
		preprocessingOk = false;
	drawingContours(contours);

/*
//DODAWANIE OBRAZÓW
	cv::Mat s=cv::Mat::zeros(fixedSize.size(),CV_32F);
	for(int i=0;i<images.size();i++) {
		cv::Mat dst=cv::Mat::zeros(fixedSize.size(),CV_32F);
		add(s,images[i],dst,cv::noArray(),-1);
		dst.copyTo(s);
	}
	s.copyTo(preprocessedEar);
*/
}
void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	preprocess();
	isReady = true;
	}
}

void Ear::findReference() {
//TODO
	
}




void Ear::execute() {
	isReady = false;
	earCords = findEar();
	extractEar(earCords);
}
	

