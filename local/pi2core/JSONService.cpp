#include "JSONService.h"
#include "CommandQueue.h"
#include <iostream>
#include <cppcms/rpc_json.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>

JSONService::JSONService(cppcms::service &srv, CommandQueue* cq) : cppcms::rpc::json_rpc_server(srv) {
	this->jsn_cq = cq;
	bind("place",cppcms::rpc::json_method(&JSONService::place,this),notification_role);
}

void JSONService::place(std::string msg){
	// Command c;
	// c.value = 0;
	// c.type = msg;
	// this->jsn_cq->putCommand(c);

	std::cout << msg << std::endl;
}