/*
 * vision.h
 *
 *  Created on: Jul 4, 2014
 *      Author: Colten
 */

#ifndef OPENCVCAM_H_
#define OPENCVCAM_H_

#include "visionInterface.h"

class OpenCVCam : public visionInterface {
public:
	//constructors
	OpenCVCam();
	~OpenCVCam();

	void getFrame(cv::Mat &);

private:
	cv::VideoCapture* vCapture;
};

#endif /* OPENCVCAM_H_ */
