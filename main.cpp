#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "vision.hpp"  
#include "opencv2/opencv.hpp"

#define MINDISTANCE 75
#define ERR 4
#define RPM 100
using namespace std;
using namespace cv;

bool ctrl_c_pressed;
void ctrlc(int)
{
	ctrl_c_pressed = true;
}

int main(void)
{	
/*
    string src = "nvarguscamerasrc sensor-id=0 ! \
video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
format=(string)NV12, framerate=(fraction)30/1 ! \
nvvidconv flip-method=0 ! video/x-raw, \
width=(int)640, height=(int)360, format=(string)BGRx ! \
videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture source(src, CAP_GSTREAMER);
*/
    VideoCapture source("5_lt_cw_100rpm_out.mp4");
	//VideoCapture source("7_lt_ccw_100rpm_in.mp4");
	if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }
	VideoWriter vw("output_video(out).mp4", VideoWriter::fourcc('H', '2', '6', '4'), 30, Size(640, 360/4), true);
	if (!vw.isOpened())
	{
		std::cout << "Can't write video !!! check setting" << std::endl;
		return -1;
	}
	
	string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=ultrafast ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=192.168.0.35 port=8001 sync=false";
	VideoWriter writer1(dst1, 0, (double)30, Size(640, 360/4), true);
	if (!writer1.isOpened()) {
		cerr << "Writer open failed!" << endl; return -1;
	}

	string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=ultrafast ! \
h264parse ! rtph264pay pt=96 ! \
udpsink host=192.168.0.35 port=8002 sync=false";
	VideoWriter writer2(dst2, 0, (double)30, Size(640, 360), true);
	if (!writer2.isOpened()) {
		cerr << "Writer2 open failed!" << endl; return -1;
	}
	signal(SIGINT, ctrlc); //시그널 핸들러 지정
	Mat stats, centroids, labels,frame, gray;
	TickMeter tm;

	while (true)
	{
		tm.reset();
		tm.start(); 

		source >> frame;
		if (frame.empty()) { cerr << "frame empty!" << endl; break; }
		writer2<<frame;

		setFrame(frame);

		int lable =  connectedComponentsWithStats(frame, labels, stats, centroids);

		static Point po = Point(frame.cols/2,frame.rows/2);

		int index = findMinIndex(stats,centroids, lable,po, MINDISTANCE);
		
		drawBoundingBox(frame,stats,centroids, lable, index);

		double lvel = RPM + getErr(frame, po, ERR), rvel = RPM*-1 + getErr(frame, po, ERR);
		
		writer1 <<frame; 
		vw <<frame;
		if (ctrl_c_pressed) break;
		usleep(30 * 1000);
		tm.stop();
		cout<<"error : "<<getErr(frame, po, ERR) <<" lvel : " << lvel <<" rvel : " << rvel <<" time : " <<tm.getTimeSec() << "Sec"<<endl;
	}
	return 0;
}
