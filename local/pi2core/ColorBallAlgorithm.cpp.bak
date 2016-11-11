#include "ColorBallAlgorithm.h"

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <ctime>

#include "pi2core/opencvcam.h"
#include "pi2core/pi2serialenums.h"

#include <wiringPi.h>
#include <wiringSerial.h>
#include "mongoose.h"

#include "pi2core/FrameQueue.h"
#include "pi2core/CommandQueue.h"

void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours);
bool separateR1(int R, int G, int B);
void separateRegion(cv::Mat const &src, cv::Mat &dest);
void processChannel(cv::Mat &channel);


int ColorBallAlgorithm::pi2Main(){
	std::cout << "Beginning test" << std::endl;
	//RPiCamera camera;
	OpenCVCam camera;

	while(true){
		//Create variables
		cv::Mat frame, rrr, ggg, bbb, processed, canny;
		std::vector<cv::Mat> channels(3);

		//Load image
		camera.getFrame(frame);

		// //obtain indivual rgb channels
		// cv::split(frame, channels);
		// bbb = channels[0];
		// ggg = channels[1];
		// rrr = channels[2];

		// //Process channels with thresholding and morph. close
		// processChannel(bbb);
		// processChannel(ggg);
		// processChannel(rrr);

		// //Combine frames
		// cv::bitwise_and(bbb, ggg, processed);
		// cv::bitwise_and(processed, rrr, processed);

		// cv::Canny(processed, canny, 5, 70, 3);
		// cv::GaussianBlur(canny, canny, cv::Size(7, 7), 0, 0);

		// std::vector<cv::Vec3f> circles;
		// /// Apply the Hough Transform to find the circles
  		// cv::HoughCircles( canny, circles, cv::HOUGH_GRADIENT, 1, canny.rows/8, 200, 100, 50, 0 );

		// for( size_t i = 0; i < circles.size(); i++ )
		// {
		// 	cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		// 	int radius = cvRound(circles[i][2]);
		// 	// circle center 
		// 	circle( frame, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
		// 	// circle outline
		// 	circle( frame, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
		// }

		// std::vector<uchar> buffer;
		// cv::imencode(".jpg", frame, buffer);
		// fq->putFrame(buffer);
		//show image
		cv::imshow("cam", frame);
		// cv::imshow("processed", processed);
		// cv::imshow("canny", canny);
		if (cv::waitKey(30) >= 0) break;

	}
	return 0;
}

void processChannel(cv::Mat &channel){
	cv::adaptiveThreshold(channel, channel, 255, cv::ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 55, 7);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7), cv::Point(3,3));
	cv::morphologyEx(channel, channel, cv::MORPH_CLOSE, element);
}

void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours){
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(src,contours,hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(dest,contours,-1,cv::Scalar(0, 0, 255), 3, 8, hierarchy);
}

