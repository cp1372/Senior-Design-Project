#include "ColorBallAlgorithm.h"

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <ctime>

#include "pi2core/rpicamera.h"
#include "pi2core/opencvcam.h"
#include "pi2core/CommandQueue.h"
#include "pi2core/FrameQueue.h"
#include "pi2core/pi2serialenums.h"

#include <wiringPi.h>
#include <wiringSerial.h>

void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours);
void morphologicalImgProc(cv::Mat &frame);
int getMotorSpeeds(int &lmotor, int &rmotor, int com_x, int bbox_len, int frameWidth, int frameHeight);
int getLargestIndex(std::vector<std::vector<cv::Point> > const &contours);

int ColorBallAlgorithm::pi2Main(){
	
	RPiCamera camera;
	//OpenCVCam camera;

	int hmax=47, hmin=25, smax=255, smin=155, vmax=255, vmin=39;

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

	while (1) {
		cv::Mat inputImg, threshImg;
		std::vector<uchar> buffer1, buffer2;
		camera.getFrame(inputImg);

		//change the color to HSV space
		cv::cvtColor(inputImg, threshImg, cv::COLOR_BGR2HSV);
		
		Command c = this->commandQueue->getCommand();

		switch(c.type){
			case pi2::HMAX :
				hmax = c.m1speed;
				break;
			case pi2::HMIN :
				hmin = c.m1speed;
				break;
			case pi2::SMAX :
				smax = c.m1speed;
				break;
			case pi2::SMIN :
				smin = c.m1speed;
				break;
			case pi2::VMAX :
				vmax = c.m1speed;
				break;
			case pi2::VMIN :
				vmin = c.m1speed;
				break;
			default :
				break;
		}
		//get the threshold frame using the minimum and maximum HSV values
		cv::inRange(threshImg, cv::Scalar(hmin, smin, vmin), cv::Scalar(hmax, smax, vmax), threshImg);


		//using the Morphological Image processing to process the image, remove noise
		morphologicalImgProc(threshImg);

		cv::imencode(".jpg", threshImg, buffer2);
		this->frameQueue->putFrame(buffer2, 2);

		std::vector<std::vector<cv::Point> > contours;
		getContours(threshImg, inputImg, contours);

		//get bounding box and COM
		int com_x = 0, com_y =0, bbox_y=0, bbox_len=0;
		int largest_index = 0;
		if(contours.size() > 0){
			largest_index = getLargestIndex(contours);
			cv::Rect bBox;
			cv::Moments moment;
			bBox = cv::boundingRect(contours[largest_index]);
			moment = cv::moments(contours[largest_index]);
			com_x = moment.m10 / moment.m00;
			com_y = moment.m01 / moment.m00;
			bbox_y = bBox.y;
			bbox_len = bBox.height;
			cv::rectangle(inputImg, bBox,cv::Scalar(0, 255, 0));
			cv::circle(inputImg, cv::Point(com_x, com_y), 30, cv::Scalar(255, 0, 0), 2);
		}

		cv::Size s = inputImg.size();
		int desired_x = s.width/2;
		int desired_len = 200;
		int deadspot = 10;
		int l_motor = 0, r_motor = 0;
		getMotorSpeeds(l_motor, r_motor, com_x, bbox_len, s.width, s.height);
		
		if(contours.size() == 0){
			l_motor = 0;
			r_motor = 0;
		}
		
		serialPutchar(fd, pi2::SETM1SPEED);
		serialPutchar(fd, (char)l_motor);
		serialPutchar(fd, pi2::SETM2SPEED);
		serialPutchar(fd, (char)r_motor);	
		cv::imencode(".jpg", inputImg, buffer1);
		this->frameQueue->putFrame(buffer1, 1);
		//show the different kinds of frames
	}
	return 0;
}

void morphologicalImgProc(cv::Mat &frame) {
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7), cv::Point(3, 3));
	cv::morphologyEx(frame, frame, cv::MORPH_OPEN, element);
}

void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours){
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(src, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
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

int getMotorSpeeds(int &l_motor, int &r_motor, int com_x, int bbox_len, int frameWidth, int frameHeight){
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

