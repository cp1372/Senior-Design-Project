#include "TestAlg.h"
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "pi2core/opencvcam.h"
#include <iostream>

int Hue_Min = 0;
int Hue_Max = 256;
int Sat_Min = 0;
int Sat_Max = 256;
int Val_Min = 0;
int Val_Max = 256;
const std::string trackingHSVApplication = "Tracking HSV Value";

void createHSVApp() {
	cv::namedWindow(trackingHSVApplication, 0);
	cv::createTrackbar("Hue_Min", trackingHSVApplication, &Hue_Min, Hue_Max);
	cv::createTrackbar("Hue_Max", trackingHSVApplication, &Hue_Max, Hue_Max);
	cv::createTrackbar("Sat_Min", trackingHSVApplication, &Sat_Min, Sat_Max);
	cv::createTrackbar("Sat_MAX", trackingHSVApplication, &Sat_Max, Sat_Max);
	cv::createTrackbar("Val_MIN", trackingHSVApplication, &Val_Min, Val_Max);
	cv::createTrackbar("Val_MAX", trackingHSVApplication, &Val_Max, Val_Max);
}

void morphologicalImgProc(cv::Mat &frame) {
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9), cv::Point(5, 5));
	morphologyEx(frame, frame, cv::MORPH_CLOSE, element);
}

void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours){
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(src,contours,hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(dest,contours,-1,cv::Scalar(0, 0, 255), 3, 8, hierarchy);
}

int getLargestIndex(std::vector<std::vector<cv::Point> > const &contours){
	int index = 0;
	int max_area = 0;
	for(int i = 0; i < contours.size(); i++){
		cv::Moments moment = moment = cv::moments(contours[i]);
		int area = moment.m00;
		if (area > max_area){
			max_area = area;
			index = i;
		}
	}
	return index;
}

int TestAlg::pi2Main(){
	//initialization of local frames
	OpenCVCam camera;

	createHSVApp();
	//default the capture frame size to the certain size & open the camera
	
	while (true) {
		cv::Mat cameraFrame, hsvFrame, thresholdFrame, contourFrame;
		//get image from stream
		camera.getFrame(cameraFrame);

		//switch the RGB to HSV space, combined with background substraction
		cv::cvtColor(cameraFrame, hsvFrame, CV_BGR2HSV);
		cv::inRange(hsvFrame, cv::Scalar(Hue_Min, Sat_Min, Val_Min), cv::Scalar(Hue_Max, Sat_Max, Val_Max), thresholdFrame);
		cv::medianBlur(thresholdFrame, thresholdFrame, 3);
		morphologicalImgProc(thresholdFrame);

		contourFrame = thresholdFrame.clone();
    	// std::vector<std::vector<cv::Point> > contours;
    	// getContours(contourFrame, cameraFrame, contours);

		cv::Canny(thresholdFrame, contourFrame, 5, 70, 3);
		cv::GaussianBlur(contourFrame, contourFrame, cv::Size(7, 7), 0, 0);

		std::vector<cv::Vec3f> circles;
  		cv::HoughCircles( contourFrame, circles, cv::HOUGH_GRADIENT, 1, contourFrame.rows/16, 200, 100, 500, 0 );

  		std::cout << circles.size() << std::endl;

  		for( size_t i = 0; i < circles.size(); i++ )
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( cameraFrame, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circle( cameraFrame, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
		}

		cv::imshow("thresholdFrame", thresholdFrame);
		cv::imshow("cameraFrame", cameraFrame);
		cv::imshow("contourFrame", contourFrame);
		if (cv::waitKey(10) >= 0)
			break;
		//release the memory
	}
	return 0;

}

