#include "M2Speed.h"
#include <pololu/orangutan>

void M2Speed::execute(){
	OrangutanMotors::setM2Speed(_speed);
}