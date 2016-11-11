/*
 * vision.h
 *
 *  Created on: Jul 4, 2014
 *      Author: Colten
 */

#ifndef RPICAMERA_H_
#define RPICAMERA_H_

#include "visionInterface.h"
#include <raspicam/raspicam_cv.h>

class RPiCamera : public visionInterface {
public:
	RPiCamera();
	~RPiCamera();

	void getFrame(cv::Mat &);

private:
	raspicam::RaspiCam_Cv* vCapture;
};

#endif /* RPICAMERA_H_ */
