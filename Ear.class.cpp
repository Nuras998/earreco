
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
cv::Mat Ear::getResizedEar() {
	return resizedEar;
}
cv::Mat Ear::getContrastEar() {
	return contrastEar;
}
cv::Mat Ear::getSharpenedEar() {
	return sharpenedEar;
}
cv::Mat Ear::getEdges2Img() {
	return edges2Img;
}
cv::Mat Ear::getEdgesCannyImg() {
	return edgesCannyImg;
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
	double first = cv::contourArea(a, false );
	double second =  cv::contourArea(b, false);
	//return (first < second) && a.size() < b.size();
	return arcLength(a, false) < arcLength(b,false);
}
std::vector<std::vector<cv::Point_<int> > > Ear::contoursFind(cv::Mat img, int minLength) {
	cv::Mat type;
	img.convertTo(type,CV_8UC1);
	type.copyTo(img);
	//std::cout << preprocessedEar.depth() << std::endl;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0, 0) );

	std::sort(contours.begin(),contours.end(), shorter);
	int numCont = contours.size();
//	std::cout << contours.size() << std::endl;
	std::vector<std::vector<cv::Point> > contoursBig;
	for(int i=0;i<contours.size();i++) {
		if(arcLength(contours[i],false) > minLength) { //canny 150 nasze 250
			contoursBig.push_back(contours[i]);
		}
	}
	//std::sort(contoursBig.begin(),contoursBig.end(), shorter);
/*	//CHOOSING ONLY FIVE LONGEST CONTOURS - bez sensu
	std::vector<std::vector<cv::Point> > contoursBigTopFive;
	if(contoursBig.size() == 5) {
		for(int j=0;j<5;j++) {
			contoursBigTopFive.push_back(contoursBig[j]);
		}
	} else if(contoursBig.size() > 5) {
		for(int j=4;j>=0;j--) {
			contoursBigTopFive.push_back(contoursBig[j]);
		}
	}
*/

/*
	for(int i=0;i<contoursBig.size();i++) {
		if(!isContourConvex(contoursBig[i])) {
			std::cout << "WKLESLY" << std::endl;
		}
	}
*/
/*
	std::vector<std::vector<cv::Point> >hull( contoursBig.size() );
	for( int i = 0; i < contoursBig.size(); i++ )
	{  convexHull( cv::Mat(contoursBig[i]), hull[i], false ); }
*/
/*	
	//zapis parametrow konturow do pliku
	td::ofstream myfile;
	myfile.open ("example.txt");
	for(int i=0;i<contoursBig.size();i++) {
		myfile << contourArea(contoursBig[i],false);
		myfile << " \n";
	}
	myfile.close();
*/
	//contours.erase(contours.begin() , contours.end()-25);
//	std::cout << contoursBig.size() << std::endl;
//	std::cout << "Nastepny: " << std::endl;
	return contoursBig;

//SORTING CONTOURS AND CHOOSING ONLY THE LONGEST ONE
/*
	std::sort(contours.begin(),contours.end(), shorter);
	std::vector<cv::Point> singleContour = contours[contours.size()-1];
	std::vector<std::vector<cv::Point> > vecContours;
	vecContours.push_back(singleContour);
	cv::Mat picSingleContour = cv::Mat::zeros(preprocessedEar.size(), CV_8UC3);
	cv::Vec3b myColor(255,0,0);
	std::vector<std::vector<cv::Point> >hullSingle(vecContours.size());
        convexHull( cv::Mat(vecContours[0]), hullSingle[0], false );
	cv::cvtColor(preprocessedEar,preprocessedEar,CV_GRAY2BGR);
	drawContours( preprocessedEar, hullSingle, 0, myColor, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
	drawContours( extractedEar, hullSingle, 0, myColor, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
*/
}

std::vector<std::vector<cv::Point_<int> > > Ear::getNotOverlappingContours(std::vector<std::vector<cv::Point_<int> > > v1, std::vector<std::vector<cv::Point_<int> > > v2) {
	//USUWANIE NAKŁADAJĄCYCH SIĘ NA SIEBIE KONTURÓW
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
			//std::vector<cv::Point> sharedPoints;
			for(int i=0; i<v1[c].size(); i++) {
				for(int j=0; j<v2[d].size(); j++) {
					for(int k=-2; k<=2;k++) {
						for(int l=-2; l<=2; l++) {
							if( (v1[c][i].x+l)==v2[d][j].x && (v1[c][i].y+k)==v2[d][j].y) { //!
								count++;
							}
						}
					}
				}
			}
			if(count >= 50) { //(double) (0.05*v1[c].size()) && count >= (double) (0.05*v2[d].size())) {
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
	//std::unique(contoursNotOverlap.begin(),contoursNotOverlap.end());
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
	//std::vector<std::vector<cv::Point> > testD;
	std::vector<cv::Point> cVect = hull[hull.size()-1];
	//testD.push_back(test);
	//cv::Mat testObraz=cv::Mat::zeros( preprocessedEar.size(), CV_32F );;
	//drawContours( testObraz, testD, 0, cv::Scalar(255,255,255), 1, 8, cv::noArray(), 0, cv::Point() );
	//imshow("obrys", testObraz);
	cv::Mat cImgType;
	//cv::waitKey();
	cImg.convertTo(cImgType,CV_32F);

	for(int i=0; i<hull.size(); i++) {
		float pointsInsideCount=0;
		for(int j=0; j<hull[i].size(); j++) {
			//int check=pointPolygonTest(cImgType,hull[i][j],false);
			if(pointPolygonTest(cImgType,hull[i][j],false)==1)
				pointsInsideCount++;
		}
		if(pointsInsideCount>= (float) (0.7*hull[i].size()))
			cInsideCount++;
	}
	cv::Mat drawing = cv::Mat::zeros( preprocessedEar.size(), CV_32F );;
	cv::Scalar gray = cv::Scalar(125,125,125);
	for(int i=0;i<(hull.size()-1);i++) {
		drawContours( drawing, hull, i, gray, 1, 8, cv::noArray(), 0, cv::Point() );
	}
	cv::Scalar white = cv::Scalar( 255,255,255 );
	drawContours( drawing, hull, hull.size()-1, white, 1, 8, cv::noArray(), 0, cv::Point() );
	drawing.copyTo(preprocessedEar);

	std::cout << cInsideCount << std::endl;
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
		//drawContours( contoursClosed, hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
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
int Ear::calcMedian(cv::Mat img) {
        std::vector<int> pixels;
        int median = 0;
        for(int i=0; i<img.rows; i++) {
                for(int j=0; j<img.cols; j++) {
                        pixels.push_back( (int) img.at<uchar>(i, j));
                }
        }
        std::sort(pixels.begin(), pixels.end(), std::greater<int>() ); //sortowanie od najmniejszego do największego
        //for(int i=0; i<pixels.size(); i++)
                //std::cout << pixels.at(i) << std::endl;
        if( pixels.size() % 2 != 0 ) //jeżeli liczba pikseli nieparzysta
                median = pixels.at( (pixels.size()+1)/2);
        else if(pixels.size() % 2 == 0) //l pikseli parzysta
                median =( ( (int) floor( pixels.at( (pixels.size()+1)/2) ) + (int) ceil( pixels.at( (pixels.size()+1)/2) ) ) / 2);
        return median;
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
	//cv::Mat wynik(img.rows,img.cols, CV_8UC3, cv::Scalar::all(0));
	//result=img.clone();
	//cv::Mat window;//(cv::Size(3,3),CV_8UC1);

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
				//std::cout << "weszlo wiersz " << y << " , kolumna "<< x << " z wiersza " << img.rows << ", kolumny " << img.cols << std::endl;
				result.at<uchar>(y+8,x+8) = 255;
			} else {
				//std::cout << "nie wiersz " << y << ", kolumna " <<  x << " z wiersza " << img.rows << ", kolumny " << img.cols << std::endl;
				result.at<uchar>(y+8,x+8) = 0;
			}
			patch.x++;
			patchNeighbourhood.x++;
			//patch+=cv::Point(1,0);
		}
		patch.x=0;
		patch.y++;
		patchNeighbourhood.x=0;
		patchNeighbourhood.y++;
		//patch+=cv::Point(0,1);
	}

/*
	cv::Mat result=cv::Mat::zeros(img.size(),CV_8UC1);
	cv::Rect patch(0,0,3,3);
	for(int y=0;y<(img.rows-2);y++) {
		for(int x=0;x<(img.cols-2);x++) {	
			cv::Mat window=img(patch);

			double mean = calcMean(window);
			double standardDeviation = calcStandardDeviation(window);
			double k = 0.2;
			double T = mean - k*standardDeviation;
			double min, max;
			cv::minMaxLoc(window, &min, &max);
			double S = max - min;
			if(S>=T) {
				//std::cout << "weszlo wiersz " << y << " , kolumna "<< x << " z wiersza " << img.rows << ", kolumny " << img.cols << std::endl;
				result.at<uchar>(y+1,x+1) = 255;
			} else {
				//std::cout << "nie wiersz " << y << ", kolumna " <<  x << " z wiersza " << img.rows << ", kolumny " << img.cols << std::endl;
				result.at<uchar>(y+1,x+1) = 0;
			}
			patch.x++;
			//patch+=cv::Point(1,0);
		}
		patch.x=0;
		patch.y++;
		//patch+=cv::Point(0,1);
	}
*/	//std::cout << "Krawedzie rozmiar" << result.size() << std::endl;
	result.copyTo(edges2Img);
}
void Ear::preprocess() {
	//cv::cvtColor(extractedEar,extractedEar,CV_BGR2GRAY);
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
	std::vector<std::vector<cv::Point> > contoursCanny;
	std::vector<std::vector<cv::Point> > contours2method;
	std::vector<std::vector<cv::Point> > contoursLaplacian;
	std::vector<std::vector<cv::Point> > contours;
	for(int i = 0; i<3;i++) {
		if(i==0) { //Canny
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//cv::Mat denoised;
			//cv::Mat equalHist;
			//cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
			//clahe->setClipLimit(4);
			//clahe->apply(bw,contrastEar);
			equalizeHist(bw,contrastEar);
			GaussianBlur(contrastEar,blurredEar, cv::Size(0, 0), 5);
			//int sizeBilateral =9;
			//bilateralFilter ( contrastEar,blurredEar, sizeBilateral, sizeBilateral*2, sizeBilateral/2 );
			//blurredEar.convertTo(contrastEar, -1, 1.5, 0);
			//GaussianBlur(contrastEar,blurredEar, cv::Size(sizeBilateral,sizeBilateral), 3,3, cv::BORDER_DEFAULT );
			//cv::Mat scalledDown=cv::Mat::zeros(fixedSize.size(),CV_16S);
			//resize(blurredEar,scalledDown, cv::Size(), 1.0/(1.41* (double) i),1.0/(1.41* (double) i), cv::INTER_CUBIC );
			//resize(smallerContrast,scalledDown, cv::Size(), 0.5,0.5, cv::INTER_CUBIC );

			cv::Mat edges=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//cv::Mat edgesScale=cv::Mat::zeros(fixedSize.size(),CV_16S);
			Canny(blurredEar,edges,10,30,7);
			//resize(edges,edgesScale, fixedSize.size(), 0,0, cv::INTER_CUBIC );

			edges.copyTo(edgesCannyImg);
			contoursCanny = contoursFind(edgesCannyImg,150);

		} else if(i==1) { //Choraś
			preprocessedEar=cv::Mat::zeros(fixedSize.size(),CV_32F);
			GaussianBlur(bw,blurredEar, cv::Size(0, 0), 3);
			findEdges(bw);
			medianBlur(edges2Img,blurredEar,7);
			blurredEar.copyTo(edges2Img);
			cv::Mat edgesFinal=cv::Mat::zeros(fixedSize.size(),CV_32F);
			//Canny(edges2Img,edgesFinal,10,30,7);
			//edgesFinal.copyTo(edges2Img);
			//contours2method = contoursFind(edges2Img,350);
		} else if(i==2) { //Laplacian
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
	if(contoursCanny.size()>0)
		preprocessingOk = checkContours(contoursCanny);
	//drawingContours(contoursCanny);
/*
//DODAWANIE OBRAZÓW
	cv::Mat s=cv::Mat::zeros(fixedSize.size(),CV_32F);
	for(int i=0;i<images.size();i++) {
		cv::Mat dst=cv::Mat::zeros(fixedSize.size(),CV_32F);
		add(s,images[i],dst,cv::noArray(),-1);
		dst.copyTo(s);
	}
	cv::Mat dilated;
	//dilate(s,dilated,element);
	s.copyTo(preprocessedEar);
*/
	//contours(250);
}
void Ear::extractEar(cv::Rect cords) {
	if(cords.x && cords.y) {
	extractedEar = originalImg(cords);
	//extractedEar = originalImg.clone();
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
	

