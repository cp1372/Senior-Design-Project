#ifndef JSONSERVICE_H_
#define JSONSERVICE_H_ 

#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/applications_pool.h>
#include <cppcms/rpc_json.h>

class CommandQueue;

class JSONService : public cppcms::rpc::json_rpc_server {
public:
	JSONService(cppcms::service &srv, CommandQueue* cq);
	~JSONService(){};

	void main(std::string){};

	void place(std::string msg);


private:
	CommandQueue* jsn_cq;
	
};

#endif