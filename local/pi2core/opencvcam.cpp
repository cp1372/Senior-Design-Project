/*
 * OpenCVCam.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: Colten
 */

#include "opencvcam.h"

OpenCVCam::OpenCVCam() {
	this->vCapture = new cv::VideoCapture(0);
}

OpenCVCam::~OpenCVCam() {
	// TODO Auto-generated destructor stub
}

void OpenCVCam::getFrame(cv::Mat & image){
	vCapture->read(image);
}
