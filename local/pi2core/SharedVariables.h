#ifndef SHAREDVARIABLES_H_
#define SHAREDVARIABLES_H_ 

class FrameQueue;
class CommandQueue;

class SharedVariables
{
public:
	SharedVariables(){};
	SharedVariables(FrameQueue* fq, CommandQueue* cq, int* localScore): fq(fq), cq(cq), localScore(localScore) {};
	~SharedVariables();

	CommandQueue* cq;
	FrameQueue* fq;
	int* localScore;
};

#endif
