/**
* ColorBallAlgorithm.h
*
* Created by: Colten Normore
*	Created on: Feb 17th, 2015 
*
* BarcodeAlgorithm inherits from AlgorithmInterface. It encompases the logic
* needed to track a barcode. The logic is kept in the pi2Main fucntion, and
* loops until interruption. 
*/

#ifndef BARCODEALGORITHM_H_
#define BARCODEALGORITHM_H_

#include "AlgorithmInterface.h"
#include <opencv/cv.h>

class FrameQueue;
class CommandQueue;

class BarcodeAlgorithm : public AlgorithmInterface
{
public:
	BarcodeAlgorithm(FrameQueue* fq, CommandQueue* cq, int* localScore) : frameQueue(fq), commandQueue(cq), localScore(localScore) {};
	~BarcodeAlgorithm(){};
	
	int pi2Main();
private:
	int* localScore;
	CommandQueue* commandQueue;
	FrameQueue* frameQueue;
	void morphologicalImgOpen(cv::Mat &frame);
	void morphologicalImgClose(cv::Mat &frame);
	double angleBetween(cv::Point &p1, cv::Point &p2);
	void getContours(cv::Mat const &src, cv::Mat &dest, std::vector<std::vector<cv::Point> > & contours);
	int getLargestIndex(std::vector<std::vector<cv::Point> > const &contours);
	int getMotorSpeeds(int &l_motor, int &r_motor, int com_x, int bbox_len, int frameWidth, int frameHeight);
};

#endif
