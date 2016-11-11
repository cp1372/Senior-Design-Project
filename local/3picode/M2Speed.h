#ifndef M2SPEED_H_
#define M2SPEED_H_

#include "PiCommand.h"

class M2Speed : public PiCommand {
public:
	M2Speed(int speed):_speed(speed) {};
	void execute();

private:
	int _speed;
};

#endif /* M2SPEED_H_ */