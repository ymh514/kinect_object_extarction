
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
static uchar intrestPoint;

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
	
	//cpu meanshift
	cvtColor(inputColor,inputColor,CV_BGR2Lab);
	pyrMeanShiftFiltering(inputColor,inputColor,50,10,3);
	cvtColor(inputColor,inputColor,CV_Lab2BGR);

	imshow("After mean shift",inputColor);

	DoSRM(100,inputColor.cols,inputColor.rows,inputColor.channels(),inputColor.data,output.data,0);

	imshow("After SRM with mean shift",output);
	
}
int main()
{
	Mat inputColor = imread("F:\\KinectDataset\\New\\2red\\color3.jpg");
	Mat inputDepth = imread("F:\\KinectDataset\\New\\2red\\depth3.jpg",CV_LOAD_IMAGE_GRAYSCALE);// 1 chanell input
	Mat output = inputColor.clone();
	Mat depthSegment = inputDepth.clone();
	Mat depthMask = Mat(inputDepth.size(),CV_8UC1,Scalar(0,0,0));

	namedWindow("Choose interest position", 1);
	setMouseCallback("Choose interest position", CallBackFunc, NULL);
	imshow("Choose interest position", inputDepth);
	waitKey(0);
	cout<<"x: "<<interstX<<"  y: "<<interstY<<endl;
	destroyAllWindows();

	namedWindow("Color input");
	namedWindow("After mean shift");
	namedWindow("After SRM with mean shift");
	namedWindow("After SRM without mean shift");
	namedWindow("After SRM with mean shift");
	namedWindow("After SRM with mean shift + depth");

	//uchar *output = new uchar[inputColor.cols*inputColor.rows*inputColor.channels()];
	
	imshow("Color input",inputColor);

	withoutMeanShitSRM(inputColor,output);
	withMeanShitSRM(inputColor,output);

	
	/*
	  depth segmentation 
	  1. find intrest point value
	  2. blur and erode 5 times
	  3. with this value to search all image
	  4. put mask on color image
	*/
	for(int i=0;i<inputDepth.rows;i++){
		for(int j=0;j<inputDepth.cols;j++){
			if(i==interstY && j==interstX){
				intrestPoint = depthSegment.at<uchar>(i,j);
				//cout<<"i: "<<i<<" ix:"<<interstX<<endl;
				//cout<<"j: "<<j<<" iy:"<<interstY<<endl;
				//for(int k=-10;k<10;k++){
				//	for(int w=-10;w<10;w++){
				//		cout<<"test"<<endl;
				//		depthTest.at<uchar>(i+k,j+w) = 255;
				//	}
				//}
			}
		}
	}

	GaussianBlur(depthSegment,depthSegment,Size(5,5),2,0);
	Mat element = getStructuringElement(MORPH_CROSS,Size(5,5),Point(1,1));
	for(int i=0;i<5;i++){
		erode(depthSegment,depthSegment,element);
		dilate(depthSegment,depthSegment,element);
	}

	for(int i=0;i<depthSegment.rows;i++){
		for(int j=0;j<depthSegment.cols;j++){
			if(depthSegment.at<uchar>(i,j) == intrestPoint){
				depthMask.at<uchar>(i,j) = 255;
			}
		}
	}


	output.copyTo(depthMask,depthMask);
	imshow("After SRM with mean shift + depth",depthMask);

	//Mat canny;
	//imshow("canny depth",inputDepth);
	//Canny(inputDepth,canny,10,30);
	//imshow("canny",canny);

	waitKey(0);
	return 0;
}

