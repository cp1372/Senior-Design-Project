#ifndef FRAMEQUEUE_H_
#define FRAMEQUEUE_H_ 

#include <mutex>
#include <vector>
#include <opencv/cv.h>

class FrameQueue
{
public:
	FrameQueue();
	~FrameQueue(){};

	std::vector<uchar> getFrame();
	std::vector<uchar> getFrame(int);

	void putFrame(std::vector<uchar> f);
	void putFrame(std::vector<uchar> f, int);
	
private:
	std::vector <uchar> fq_frame1, fq_frame2;
	std::mutex fq_mutex1, fq_mutex2;
};
#endif