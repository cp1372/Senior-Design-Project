#ifndef SOCKETSERVERMANAGER_H_
#define SOCKETSERVERMANAGER_H_


class CommandQueue;
class SocketServerManager
{
public:
	SocketServerManager(){};
	~SocketServerManager(){};

	void run(CommandQueue* cq);
	


private:
	CommandQueue * pr_cq;

};
#endif
