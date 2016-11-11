#ifndef M1SPEED_H_
#define M1SPEED_H_

#include "PiCommand.h"

class M1Speed : public PiCommand {
public:
	M1Speed(int speed):_speed(speed) {};
	void execute();

private:
	int _speed;
};

#endif /* M1SPEED_H_ */