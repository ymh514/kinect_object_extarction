
// STL Header
#include <iostream>

// OpenCV Header
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "SRM.h"

// namespace
using namespace std;
using namespace cv;

// global parameters
static int interstX;
static int interstY;

static uchar depthInterestPoint;
static int interestR = 0;
static int interestG = 0;
static int interestB = 0;


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
		 interstX = x;
		 interstY = y;
         cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
}
void withoutMeanShitSRM(Mat inputColor,Mat colorSegment){

	DoSRM(100,inputColor.cols,inputColor.rows,inputColor.channels(),inputColor.data,colorSegment.data,0);

	imshow("After SRM without mean shift",colorSegment);


}
void withMeanShitSRM(Mat inputColor,Mat colorSegment){
	
	//Cpu Meanshift
	cvtColor(inputColor,inputColor,CV_BGR2Lab);
	pyrMeanShiftFiltering(inputColor,inputColor,50,10,3);
	cvtColor(inputColor,inputColor,CV_Lab2BGR);

	//imshow("After mean shift",inputColor);

	DoSRM(100,inputColor.cols,inputColor.rows,inputColor.channels(),inputColor.data,colorSegment.data,0);

	imshow("After SRM with mean shift",colorSegment);
	
}
int main()
{
	/*
		Initial Paremeter
	*/
	Mat inputColor = imread("F:\\KinectDataset\\New\\2red\\color6.jpg");
	Mat inputDepth = imread("F:\\KinectDataset\\New\\2red\\depth6.jpg",CV_LOAD_IMAGE_GRAYSCALE);// 1 chanell input

	Mat colorSegment = inputColor.clone();
	Mat colorMask = Mat(inputColor.size(),CV_8UC1,Scalar(0,0,0));
	Mat depthSegment = inputDepth.clone();
	Mat depthMask = Mat(inputDepth.size(),CV_8UC1,Scalar(0,0,0));

	Mat combineMask = Mat(inputColor.size(),CV_8UC1,Scalar(0,0,0));

	/*
		Choose interest position
	*/
	namedWindow("Choose interest position", 1);
	setMouseCallback("Choose interest position", CallBackFunc, NULL);

	imshow("Choose interest position", inputColor);
	waitKey(0);
	cout<<"x: "<<interstX<<"  y: "<<interstY<<endl;
	destroyWindow("Choose interest position");

	/*
		Create show windows
	*/

	//namedWindow("Color input");
	//namedWindow("Depth input");
	//imshow("Color input",inputColor);
	//imshow("Depth input",inputDepth);
	//namedWindow("After mean shift");
	//namedWindow("After SRM without mean shift");
	namedWindow("After SRM with mean shift");
	namedWindow("Segmentation result");
	
	/*
		Color Segmentation : 
		1. Mean Shift
		2. SRM
		3. Color Segmentation Mask
	*/

	//withoutMeanShitSRM(inputColor,colorSegment);
	withMeanShitSRM(colorSegment,colorSegment);

	double interestPositionColorValue = 0;

	for(int i=0;i<colorSegment.rows;i++){
		for(int j=0;j<colorSegment.cols;j++){
			if(i==interstY && j==interstX){

				interestR = static_cast<int>(colorSegment.at<Vec3b>(i,j)[0]);
				interestG = static_cast<int>(colorSegment.at<Vec3b>(i,j)[1]);
				interestB = static_cast<int>(colorSegment.at<Vec3b>(i,j)[2]);

				cout<<"Interest Position Red :"<<interestR<<endl;
				cout<<"Interest Position Green :"<<interestG<<endl;
				cout<<"Interest Position Blue :"<<interestB<<endl;

			}
		}
	}

	int count = 0;
	for(int i=0;i<colorSegment.rows;i++){
		for(int j=0;j<colorSegment.cols;j++){
			int tempR = static_cast<int>(colorSegment.at<Vec3b>(i,j)[0]);
			int tempG = static_cast<int>(colorSegment.at<Vec3b>(i,j)[1]);
			int tempB = static_cast<int>(colorSegment.at<Vec3b>(i,j)[2]);

			if(tempR == interestR && tempB == interestB && tempG == interestG){
				colorMask.at<uchar>(i,j) = 255;
				count ++;
			}
		}
	}
	imshow("Color Mask",colorMask);


	/*
		Depth Segmentation : 
		1. find intrest point value
		2. blur and erode 5 times
		3. with this value to search all image
		4. put mask on color image
	*/

	for(int i=0;i<depthSegment.rows;i++){
		for(int j=0;j<depthSegment.cols;j++){
			if(i==interstY && j==interstX){
				depthInterestPoint = depthSegment.at<uchar>(i,j);
				int interestPositionDepthValue = static_cast<int>(depthInterestPoint);
				cout<<"Interest Position Depth Gray Value : "<<interestPositionDepthValue<<endl;
			}
		}
	}



	GaussianBlur(depthSegment,depthSegment,Size(5,5),2,0);
	//Mat element = getStructuringElement(MORPH_CROSS,Size(5,5),Point(1,1));
	//for(int i=0;i<5;i++){
	//	erode(depthSegment,depthSegment,element);
	//	dilate(depthSegment,depthSegment,element);
	//}

	//cvtColor(depthSegment,depthSegment,CV_GRAY2BGR);
	//pyrMeanShiftFiltering(depthSegment,depthSegment,15,15,2);
	//cvtColor(depthSegment,depthSegment,CV_BGR2GRAY);
	//imshow("after",depthSegment);

	for(int i=0;i<depthSegment.rows;i++){
		for(int j=0;j<depthSegment.cols;j++){ 
			int tempDepthValue = static_cast<int>(depthSegment.at<uchar>(i,j));
			int interestPositionDepthValue = static_cast<int>(depthInterestPoint);
			if(tempDepthValue >= interestPositionDepthValue-1 && tempDepthValue <= interestPositionDepthValue+1 ){
				depthMask.at<uchar>(i,j) = 255;
			}
		}
	}
	imshow("Depth Mask",depthMask);

	for(int i=0;i<colorMask.rows;i++){
		for(int j=0;j<colorMask.cols;j++){
			if(colorMask.at<uchar>(i,j) == 255){
				if(depthMask.at<uchar>(i,j) == 255){
					combineMask.at<uchar>(i,j) = 255;
				}
			}

		}
	}
	imshow("Combine Mask",combineMask);


	inputColor.copyTo(combineMask,combineMask);
	imshow("Original Image",inputColor);
	imshow("Segmentation result",combineMask);
	
	//Mat canny;
	//imshow("canny depth",inputDepth);
	//Canny(inputDepth,canny,10,30);
	//imshow("canny",canny);

	waitKey(0);
	return 0;
}

