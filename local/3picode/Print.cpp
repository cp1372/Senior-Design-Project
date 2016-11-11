#include "Print.h"
#include <pololu/orangutan>

void Print::execute(){
	OrangutanLCD::clear();
	OrangutanLCD::print(_string);
}