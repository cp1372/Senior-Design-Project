#include "M1Speed.h"
#include <pololu/orangutan>

void M1Speed::execute(){
	OrangutanMotors::setM1Speed(_speed);
}