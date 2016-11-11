#ifndef REMOTEALG_H_
#define REMOTEALG_H_

#include "AlgorithmInterface.h"

class CommandQueue;
class FrameQueue;

class RemoteAlg : public AlgorithmInterface
{
public:
	RemoteAlg(FrameQueue* fq, CommandQueue* cq) : frameQueue(fq), commandQueue(cq) {};
	~RemoteAlg(){};
	
	int pi2Main();

private:
	CommandQueue* commandQueue;
	FrameQueue* frameQueue;
};

#endif