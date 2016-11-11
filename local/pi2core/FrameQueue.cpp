#include "FrameQueue.h"
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

FrameQueue::FrameQueue(){
	cv::Mat startImg(240, 320, CV_8UC3, cv::Scalar(0,0,0));
	cv::imencode(".jpg", startImg, fq_frame1);
	cv::imencode(".jpg", startImg, fq_frame2);
}

void FrameQueue::putFrame(std::vector<uchar> frame){
	this->fq_mutex1.lock();
	this->fq_frame1 = frame;
	this->fq_mutex1.unlock();
}

void FrameQueue::putFrame(std::vector<uchar> frame, int num = 1){
	if (num == 1){
		this->fq_mutex1.lock();
		this->fq_frame1 = frame;
		this->fq_mutex1.unlock();
	} else {
		this->fq_mutex2.lock();
		this->fq_frame2 = frame;
		this->fq_mutex2.unlock();
	}
}

std::vector<uchar> FrameQueue::getFrame(){
	std::vector<uchar> f;
	this->fq_mutex1.lock();
	f = this->fq_frame1;
	this->fq_mutex1.unlock();
	return f;
}

std::vector<uchar> FrameQueue::getFrame(int num = 1){
	std::vector<uchar> f;
	if (num == 1){
		this->fq_mutex1.lock();
		f = this->fq_frame1;
		this->fq_mutex1.unlock();
	} else {
		this->fq_mutex2.lock();
		f = this->fq_frame2;
		this->fq_mutex2.unlock();
	}
	return f;
}