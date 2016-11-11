#include "SerialManager.h"
#include <pololu/orangutan>

SerialManager::SerialManager(){
	this->receive_buffer_position = 0;
    OrangutanSerial::setBaudRate(9600);
    OrangutanSerial::receiveRing(this->receive_buffer, sizeof(this->receive_buffer));	
}

char SerialManager::getNextByte(){
	char byte;
	while (OrangutanSerial::getReceivedBytes() == this->receive_buffer_position);
	byte = this->receive_buffer[this->receive_buffer_position];
	this->incrementBufferPosition();
	return byte;
}

void SerialManager::incrementBufferPosition(){
	if (receive_buffer_position == sizeof(receive_buffer)-1)
    {
        receive_buffer_position = 0;
    }
    else
    {
        receive_buffer_position++;
    }
}

void SerialManager::waitForSendingToFinish(){
    while(!OrangutanSerial::sendBufferEmpty());
}