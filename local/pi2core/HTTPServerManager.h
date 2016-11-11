#ifndef HTTPSERVERMANAGER_H_
#define HTTPSERVERMANAGER_H_

class SharedVariables;

class HTTPServerManager
{
public:
	HTTPServerManager(){};
	~HTTPServerManager(){};

	void run(int argc, char ** argv, SharedVariables* sv, char* alg);
};

#endif

