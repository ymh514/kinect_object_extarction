
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

//void withoutMeanShitSRM(Mat inputColor,Mat output);
//void withMeanShitSRM(Mat inputColor,Mat output);

static int interstX;
static int interstY;
static uchar colorIntrestPoint;
static uchar depthIntrestPoint;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
		 interstX = x;
		 interstY = y;
         cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
}
void withoutMeanShitSRM(Mat inputColor,Mat output){

	DoSRM(100,inputColor.cols,inputColor.rows,inputColor.channels(),inputColor.data,output.data,0);

	imshow("After SRM without mean shift",output);


}
void withMeanShitSRM(Mat inputColor,Mat output){
	
	//Cpu Meanshift
	cvtColor(inputColor,inputColor,CV_BGR2Lab);
	pyrMeanShiftFiltering(inputColor,inputColor,50,10,3);
	cvtColor(inputColor,inputColor,CV_Lab2BGR);

	//imshow("After mean shift",inputColor);

	DoSRM(100,inputColor.cols,inputColor.rows,inputColor.channels(),inputColor.data,output.data,0);

	imshow("After SRM with mean shift",output);
	
}
int main()
{
	/*
		Initial Paremeter
	*/
	Mat inputColor = imread("F:\\KinectDataset\\New\\2red\\color3.jpg");
	Mat inputDepth = imread("F:\\KinectDataset\\New\\2red\\depth3.jpg",CV_LOAD_IMAGE_GRAYSCALE);// 1 chanell input
	//DoSRM(20000,inputDepth.cols,inputDepth.rows,inputDepth.channels(),inputDepth.data,inputDepth.data,0);

	Mat output = inputColor.clone();
	Mat colorMask = Mat(inputColor.size(),CV_8UC1,Scalar(0,0,0));
	Mat depthSegment = inputDepth.clone();
	Mat depthMask = Mat(inputDepth.size(),CV_8UC1,Scalar(0,0,0));

	/*
		Choose interest position
	*/
	namedWindow("Choose interest position", 1);
	setMouseCallback("Choose interest position", CallBackFunc, NULL);

	imshow("Choose interest position", inputDepth);
	waitKey(0);
	cout<<"x: "<<interstX<<"  y: "<<interstY<<endl;
	destroyWindow("Choose interest position");

	/*
		Create show windows
	*/
	namedWindow("Color input");
	namedWindow("Depth input");
	imshow("Color input",inputColor);
	imshow("Depth input",inputDepth);
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

	//withoutMeanShitSRM(inputColor,output);
	withMeanShitSRM(inputColor,output);
	cvtColor(output,output,CV_BGR2GRAY);
	double interestPositionColorValue = 0;

	for(int i=0;i<output.rows;i++){
		for(int j=0;j<output.cols;j++){
			if(i==interstY && j==interstX){
				colorIntrestPoint = output.at<uchar>(i,j);
				interestPositionColorValue = static_cast<int>(colorIntrestPoint);
				cout<<"Interest Position Color Gray Value : "<<interestPositionColorValue<<endl;

			}
		}
	}
	imshow("wait wait ",output);
	int count = 0;
	for(int i=0;i<output.rows;i++){
		for(int j=0;j<output.cols;j++){
			int tempColorValue = static_cast<int>(output.at<uchar>(i,j));
			if(interestPositionColorValue == tempColorValue){
				colorMask.at<uchar>(i,j) = 255;
				count ++;
			}
		}
	}
	imshow("color mask",colorMask);

	//adaptiveThreshold(output,colorMask,interestPositionColorValue,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,3,2);
	//imshow("colormask",colorMask);
	

	/*
		Depth Segmentation : 
		1. find intrest point value
		2. blur and erode 5 times
		3. with this value to search all image
		4. put mask on color image
	*/

	for(int i=0;i<inputDepth.rows;i++){
		for(int j=0;j<inputDepth.cols;j++){
			if(i==interstY && j==interstX){
				depthIntrestPoint = depthSegment.at<uchar>(i,j);
				int interestPositionDepthValue = static_cast<int>(depthIntrestPoint);
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
			int interestPositionDepthValue = static_cast<int>(depthIntrestPoint);
			if(tempDepthValue >= interestPositionDepthValue-1 && tempDepthValue <= interestPositionDepthValue+1 ){
				depthMask.at<uchar>(i,j) = 255;
			}
		}
	}
	imshow("Depth Mask",depthMask);


	inputColor.copyTo(depthMask,depthMask);
	imshow("Segmentation result",depthMask);
	
	//Mat canny;
	//imshow("canny depth",inputDepth);
	//Canny(inputDepth,canny,10,30);
	//imshow("canny",canny);

	waitKey(0);
	return 0;
}

