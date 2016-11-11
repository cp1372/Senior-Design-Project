#include "BarcodeAlgorithm.h"

#include <opencv/highgui.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "pi2core/rpicamera.h"
#include "pi2core/opencvcam.h"
#include "pi2core/CommandQueue.h"
#include "pi2core/FrameQueue.h"
#include "pi2core/pi2serialenums.h"

#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

void setLabel(cv::Mat& im, const std::string label, int x, int y) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);

	cv::Point pt(x, y);
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

int BarcodeAlgorithm::pi2Main(){
	RPiCamera camera;
	//OpenCVCam camera;

	int hmax=255, hmin=0, smax=255, smin=0, vmax=100, vmin=0;

    int fd = 0;

    if ((fd = serialOpen("/dev/ttyAMA0", 9600)) < 0)
    {
            std::cout << "Unable to open serial device" << std::endl;
            return 1 ;
    }

    if (wiringPiSetup() == -1 )
    {
            std::cout << "Unable to start wiringPi" << std::endl;
    }
    
    	std::vector<std::vector<cv::Point> > contours;
    	std::vector<cv::Vec4i> hierarchy;
    	std::vector<cv::Point> approx;

	cv::Mat inputImg, hsvImg, greyImg, threshImg;
	std::vector<uchar> buffer1, buffer2;
	bool sawBarcode = false;
	int lastCommand = pi2::FORWARD;
	int lastSeenDir = pi2::FORWARD;
	bool ready = false;

	while (1) {
		sawBarcode = false;
		camera.getFrame(inputImg);
		
		cv::Point2f ptCp(inputImg.cols * 0.5, inputImg.rows * 0.5);
		cv::Mat rotMatrix = cv::getRotationMatrix2D(ptCp, 90, 1.0);
		cv::warpAffine(inputImg, inputImg, rotMatrix, inputImg.size(), cv::INTER_CUBIC);
		
		Command c = this->commandQueue->getCommand();
		//std::cout << c.type << std::endl;		
		if (c.type != pi2::NOCOMMAND) {
			if (c.type == pi2::INIT) {
				ready = true;
			}

			if (ready) {
				std::cout << "Setting Command: " << c.type << " on port " << fd << std::endl; 
				serialPutchar(fd,(char)c.type);

				if (c.type == pi2::TIMEUP) {
					while(1) {};
				}
			}
		}

		if (!ready) continue;

		cv::cvtColor(inputImg, hsvImg, CV_BGR2HSV);
		cv::cvtColor(inputImg, greyImg, CV_BGR2GRAY);
		cv::threshold(greyImg, threshImg, 30, 255, cv::THRESH_BINARY);
		morphologicalImgOpen(threshImg);

		std::vector<cv::Mat> hsv_channels;
		cv::split(hsvImg, hsv_channels);
		cv::Mat hImg = hsv_channels[0];
		cv::Mat sImg = hsv_channels[1];
		cv::Mat vImg = hsv_channels[2];
		cv::Mat binaryImg = hImg.clone();

		for (int row = 0; row < hImg.rows; row++) {
			uchar* h = hImg.ptr(row);
			uchar* s = sImg.ptr(row);
			uchar* v = vImg.ptr(row);
			uchar* b = binaryImg.ptr(row);
			
			// H: 0-180, S: 0-255, V: 0-255
			for (int col = 0; col < hImg.cols; col++) {
				if ((*(h+col) > 169 || *(h+col) < 11) && *(s+col) > 110 && *(v+col) > 60) {
					*(b+col) = 255;
				} else {
					*(b+col) = 0;
				}
			}
		}
		
		morphologicalImgOpen(binaryImg);
		morphologicalImgClose(binaryImg);
		
		cv::findContours(binaryImg.clone(), contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);	
		
		for (int i = 0; i < contours.size(); i++) {
			std::vector<cv::Point> contour = contours[i];
			double area = cv::contourArea(contour);

			if (area < 300) {
				contours.erase(contours.begin() + i);
				i--;
			}
		}

		std::vector<cv::Moments> mu(contours.size());
		std::vector<cv::Point> mc(contours.size());
		
		for (int i = 0; i < contours.size(); i++) {
			mu[i] = moments(contours[i], false);
			mc[i] = cv::Point(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}
		
		for (int i = 0; i < mc.size(); i++) {
			for (int j = i+1; j < mc.size(); j++) {				
				double distance = sqrt(pow((mc[i].x-mc[j].x), 2) + pow((mc[i].y-mc[j].y), 2));

				if (distance > 55 && distance < 5000) {					
					std::vector<cv::Point> top, bottom;
					cv::Rect r_top, r_bottom;
					
					if (mc[i].y > mc[j].y) {
						r_top = cv::boundingRect(contours[i]);
						r_bottom = cv::boundingRect(contours[j]);
					}
					else {
						r_top = cv::boundingRect(contours[j]);
						r_bottom = cv::boundingRect(contours[i]);
					}

					int top_boundary = r_top.y;
					int bottom_boundary = r_bottom.y + r_bottom.height;
					int left_boundary = r_bottom.x;
					int right_boundary = r_top.x + r_top.width;
					int bitHeight = r_top.height * 0.75;
					int lastTransition = bottom_boundary;

					int hor_center = (left_boundary + right_boundary) / 2;
					std::string code = "";

					for (int k = bottom_boundary; k < top_boundary; k++) {
						uchar* t = threshImg.ptr(k);
						uchar* t_prev = threshImg.ptr(k-1);
						int current = *(t + hor_center);
						int prev = *(t_prev + hor_center);
						int difference = prev - current;

						if (difference != 0 || k == (top_boundary - 1)) {
							int len = k - lastTransition;
							lastTransition = k;
							int numBits = std::max((int)round(len / (double)bitHeight), 1);
							std::string bit;

							if (k == (top_boundary - 1)) {
								bit = (current > 0) ? "1" : "0";
							}
							else {
								bit = (difference > 0) ? "1" : "0";
							}

							for (int l = 0; l < numBits; l++) {
								code += bit;
							}
						}
					}

					if (code.size() == 4) {
						sawBarcode = true;
						lastSeenDir = (hor_center > 400) ? pi2::FORWARD : pi2::BACKWARD;

						if (hor_center < 360) {
							serialPutchar(fd,(char)pi2::BACKWARD);
							lastCommand = pi2::BACKWARD;
						}
						else if (hor_center > 440) {
							serialPutchar(fd,(char)pi2::FORWARD);
							lastCommand = pi2::FORWARD;
						}
						else if (code == "1010") {
							(*(this->localScore))++;
							serialPutchar(fd,(char)pi2::SHOOT);
							serialPutchar(fd,(char)pi2::STOP);
							lastCommand = pi2::STOP;
							usleep(2000000);
						}

//						setLabel(inputImg, code, top_boundary, hor_center);
						std::cout << code << std::endl;

					}

				}
			}
		}

		if (!sawBarcode && lastCommand == pi2::STOP) {
			serialPutchar(fd,(char)lastSeenDir);
			lastCommand = lastSeenDir;
		}
		
		cv::imencode(".jpg", inputImg, buffer1);
		this->frameQueue->putFrame(buffer1, 1);
		cv::imencode(".jpg", binaryImg, buffer2);
		this->frameQueue->putFrame(buffer2, 2);
	}
	
	return 0;
}

double BarcodeAlgorithm::angleBetween(cv::Point &p1, cv::Point &p2) {
	double slope = (p2.y - p1.y) / double(p2.x - p1.x);
	return tan(slope);
}

void BarcodeAlgorithm::morphologicalImgOpen(cv::Mat &frame) {
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(-1,-1));
	cv::morphologyEx(frame, frame, cv::MORPH_OPEN, element);
}

void BarcodeAlgorithm::morphologicalImgClose(cv::Mat &frame) {
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5), cv::Point(-1,-1));
	cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, element);
}

int BarcodeAlgorithm::getLargestIndex(std::vector<std::vector<cv::Point> > const &contours){
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

int BarcodeAlgorithm::getMotorSpeeds(int &l_motor, int &r_motor, int com_x, int bbox_len, int frameWidth, int frameHeight){
	enum ForwardDirection {FORWARD, REVERSE, NONE};
	int x_target = frameWidth/2;
	int height_target = 90; //tune this number later
	int x_deadspot = 20;
	int height_deadspot = 7;
	int max_speed = 40;
	ForwardDirection forward_direction = NONE;

	int forward_speed = 0;
	if (bbox_len > (height_target + height_deadspot)){
		int max_height = frameHeight;
		double speed_factor = ((double)bbox_len - (double)height_target)/((double)max_height - (double)height_target);
		forward_speed = (max_speed * speed_factor);
		forward_direction = REVERSE;
	} else if (bbox_len < (height_target - height_deadspot)){
		double speed_factor = 1.0 - ((double)bbox_len/(double)height_target);
		forward_speed = -(max_speed * speed_factor) * 1.5;
		forward_direction = FORWARD;
	} else {
		forward_speed = 0;
		forward_direction = NONE;
	}

	int turn_speed = 0;
	if (com_x > (x_target + x_deadspot)){
		int max_width = frameWidth;
		double speed_factor = ((double)com_x - (double)x_target)/((double)max_width - (double)x_target);
		turn_speed = max_speed * speed_factor;
		switch(forward_direction){
			case FORWARD:
			case NONE:
				l_motor = turn_speed + forward_speed;
				r_motor = forward_speed;
				break;
			case REVERSE:
				l_motor = forward_speed;
				r_motor = -(turn_speed) + forward_speed;
				break;
		}

	} else if (com_x < (x_target - x_deadspot)){
		double speed_factor = 1.0 - ((double)com_x/(double)x_target);
		turn_speed = max_speed * speed_factor;
		switch(forward_direction){
			case FORWARD:
			case NONE:
				l_motor = forward_speed;
				r_motor = turn_speed + forward_speed;
				break;
			case REVERSE:
				l_motor = -(turn_speed) + forward_speed;
				r_motor = forward_speed;
				break;
		}
	} else {
		turn_speed = 0;
		l_motor = forward_speed;
		r_motor = forward_speed;
	}

	if(bbox_len < 20){
		l_motor = 0;
		r_motor = 0;
	}	
}

