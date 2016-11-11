#include "CommandQueue.h"
#include "pi2serialenums.h"
#include <iostream>


CommandQueue::CommandQueue(){
	Command c;
    c.type = pi2::NOCOMMAND;
	c.m1speed = 0;
    c.m2speed = 0;
	this->cq_command = c; 
}

void CommandQueue::putCommand(Command command){
    //std::cout << "Push Command ---" << command.type << "  m1.Speed: " << command.m1speed << "  m2.speed: " << command.m2speed << std::endl;
    this->cq_mutex.lock();
	this->cq_command = command;
	this->cq_mutex.unlock();
}

Command CommandQueue::getCommand(){
	Command replace_c;
	replace_c.type = pi2::NOCOMMAND;

	this->cq_mutex.lock();
	Command c = this->cq_command;
    //std::cout << "Get Command --- " << "Type: " <<c.type << "  m1.Speed: " << c.m1speed << "  m2.speed: " << c.m2speed << std::endl;
	this->cq_command = replace_c;
	this->cq_mutex.unlock();

	return c;

}
