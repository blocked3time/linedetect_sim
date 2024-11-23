#include<math.h>
#include"vision.hpp"
using namespace cv;

void setFrame(Mat& frame){
		if(frame.type() != CV_8UC3 ) return ;
		frame +=  Scalar(100,100,100) - mean(frame);
		frame = frame(Rect(Point(0,frame.rows/4*3),Point(frame.cols,frame.rows)));
        cvtColor(frame,frame,COLOR_RGB2GRAY);
		threshold(frame, frame, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void drawBoundingBox(Mat& frame,Mat labels,Mat stats,Mat centroids, int lable, int index){
	if(frame.type() != CV_8U ) return ;
	cvtColor(frame, frame, COLOR_GRAY2BGR);
	Scalar sc;
	 for(int i = 1;i<lable;i++){
		if(i == index) sc = Scalar(0,0,255);
        else sc =stats.at<int>(i,4)<50 ? Scalar(0,255,255):Scalar(255,0,0);
        rectangle(frame,Rect(stats.at<int>(i,0),stats.at<int>(i,1),stats.at<int>(i,2),stats.at<int>(i,3)),sc);
        rectangle(frame,Rect(centroids.at<double>(i,0),centroids.at<double>(i,1),3,3),sc);
        } 
}

int findMinIndex(Mat stats,Mat centroids, int lable, Point& po,int MINDISTANCE){
	int index = 0;
	double mindistance = MINDISTANCE;
	for(int i = 1;i<lable;i++){
        if(stats.at<int>(i,4)<50) continue;
		double distance = sqrt(pow((po.x-centroids.at<double>(i,0)),2)+pow((po.y- centroids.at<double>(i,1)),2));
		if(distance<mindistance){
            mindistance = distance;
            index = i;
        }
     }
	if(mindistance <  MINDISTANCE && po != Point(centroids.at<double>(index,0),centroids.at<double>(index,1))) 
		po = Point(centroids.at<double>(index,0),centroids.at<double>(index,1));
	return index;
}

double getErr(Mat frame, Point po,int err){
	return (frame.cols/2 -po.x)/err;
}