/****************************
* pi2serialenums.cpp
* File containing common enums for the 3pi and RPi
* 
****************************/

namespace pi2 {
	enum Serial {
		SETM1SPEED = 0,
		SETM2SPEED = 1,
		PRINT = 2
	};
    
    enum Type{
	NOCOMMAND,
	INIT,
	FORWARD,
	BACKWARD,
	STOP,
	SHOOT,
	TIMEUP
    };

    enum HSV{
        HMAX,
        HMIN,
        SMAX,
        SMIN,
        VMAX,
        VMIN
    };
}
