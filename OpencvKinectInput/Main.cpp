
// STL Header
#include <iostream>

// OpenCV Header
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


// namespace
using namespace std;
using namespace cv;

int main()
{
	Mat inputColor = imread("F:\\KinectDataset\\New\\2red\\color3.jpg");
	Mat inputDepth = imread("F:\\KinectDataset\\New\\2red\\depth3.jpg",CV_8UC1);// 1 chanell input
	Mat colorGray;

	imshow("color input ",inputColor);

	//Mat mask = Mat(inputColor.size(),CV_8UC3,Scalar(0,0,0));
	Mat showContours = Mat(inputColor.size(),CV_8UC1,Scalar(0,0,0));

	cvtColor(inputColor,colorGray,CV_BGR2GRAY);

	threshold(colorGray,colorGray,127,255,THRESH_BINARY_INV+THRESH_OTSU);
	imshow("gray image",colorGray);


	//draw contours
	vector<vector<Point>> contours;
	findContours(colorGray,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	for(int i=0;i<contours.size();i++){
		Rect boundRect=boundingRect(contours[i]);
		double n = contourArea(contours[i]);
					drawContours(showContours,contours,i,Scalar(255,255,255),CV_FILLED);

		if(n>4000){

		}
		else{
		}
	}
	//imshow("draw contours",showContours);

	inputColor.copyTo(showContours,showContours);
	imshow("segment result",showContours);
	
	waitKey(0);
	return 0;
}
