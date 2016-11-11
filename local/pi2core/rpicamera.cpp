/*
 * RPiCamera.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: Colten
 */

#include "rpicamera.h"
#include <raspicam/raspicam_cv.h>
#include <opencv/cv.h>
#include <iostream>

RPiCamera::RPiCamera() {
	this->vCapture = new raspicam::RaspiCam_Cv();
	this->vCapture->set( CV_CAP_PROP_FRAME_WIDTH, 800 );
	this->vCapture->set( CV_CAP_PROP_FRAME_HEIGHT, 600 );
	this->vCapture->set( CV_CAP_PROP_GAIN, 100 );
	this->vCapture->set( CV_CAP_PROP_BRIGHTNESS, 50 );
	this->vCapture->set( CV_CAP_PROP_EXPOSURE, 50 );
	this->vCapture->open();
}

RPiCamera::~RPiCamera() {
	// TODO Auto-generated destructor stub
}

void RPiCamera::getFrame(cv::Mat & image){
	this->vCapture->grab();
	this->vCapture->retrieve(image);
}
