#ifndef SERIALMANAGER_H_
#define SERIALMANAGER_H_

class SerialManager
{
public:
	SerialManager();
	~SerialManager();

	char getNextByte();

private:
	void incrementBufferPosition();
	void waitForSendingToFinish();

	char receive_buffer[32];
	char send_buffer[32];
	unsigned char receive_buffer_position;
};

#endif /* SERIALMANAGER_H_ */