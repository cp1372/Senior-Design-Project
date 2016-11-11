#include "PiCommandFactory.h"
#include "SerialManager.h"
#include "../pi2core/pi2serialenums.h"
#include "PiCommand.h"
#include "M1Speed.h"
#include "M2Speed.h"
#include "Print.h"

PiCommand* PiCommandFactory::createCommand(){
	char message = this->_serial->getNextByte();
	PiCommand * command = new Print("NoCmd");
	switch(message){
	case pi2::SETM1SPEED: {
		command = new M1Speed(this->_serial->getNextByte());
		} break;
	case pi2::SETM2SPEED: {
		// command = new M2Speed(this->_serial->getNextByte());
		M2Speed speed(this->_serial->getNextByte());
		command = &(PiCommand)speed;
		} break;
	case pi2::PRINT: {
		command = new Print("DRRR!x2");
		} break; 
	default: {
		command = new Print("InvalidCmd");
		} break;
	}

	return command;
}