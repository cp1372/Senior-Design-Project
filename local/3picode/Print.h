#ifndef PRINT_H_
#define PRINT_H_

#include "PiCommand.h"

class Print : public PiCommand {
public:
	Print(char* string): _string(string){};

	void execute();
private:
	char* _string;
};

#endif /* PRINT_H_ */