#include "../pi2core/pi2serialenums.h"
#include "C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\avr\include\pololu\Pololu3pi\Pololu3pi.h"
   
char receive_buffer[32];
char send_buffer[32];
unsigned char receive_buffer_position = 0;

// sounds
const char starting[] PROGMEM = "!L16 V15 cdefgab>cbagfedc";
const char reloading[] PROGMEM = "!L16 V15 MS cccccccccccc";
const char gameover[] PROGMEM = "!L8 V15 accbbddRaccbbddRaccbbddRaccbbddR";

void process_messages();
void initialize();
void move();
void increment_buffer_position();
bool get_next_byte(char& byte);
unsigned int max(unsigned int* arr, unsigned int len);

int speed = 40;
int direction = 1;
int lastDir = 1;
bool needToTurn = false;
int oobRevTime = 1000;
unsigned int sensors[5]; // an array to hold sensor values
unsigned int reverseThreshold = 300;
int last_proportional = 0;
long integral = 0;
int last_proportional_reverse = 0;
long integral_reverse = 0;

// 0 = currently stopped, 1 = currently moving
int moving = 0;

int main()
{
	OrangutanSerial::setBaudRate(9600);
	OrangutanSerial::receiveRing(receive_buffer, sizeof(receive_buffer));
	
    while(1)
    {
        process_messages();
    }
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	unsigned int counter; // used as a simple timer
	
	// This must be called at the beginning of 3pi code, to set up the
	// sensors.  We use a value of 2000 for the timeout, which
	// corresponds to 2000*0.4 us = 0.8 ms on our 20 MHz processor.
	pololu_3pi_init(2000);
	
	// Auto-calibration: turn right and left while calibrating the
	// sensors.
	for(counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
		set_motors(40,-40);
		else
		set_motors(-40,40);
		
		// This function records a set of sensor readings and keeps
		// track of the minimum and maximum values encountered.  The
		// IR_EMITTERS_ON argument means that the IR LEDs will be
		// turned on during the reading, which is usually what you
		// want.
		calibrate_line_sensors(IR_EMITTERS_ON);
		
		// Since our counter runs to 80, the total delay will be
		// 80*20 = 1600 ms.
		delay_ms(20);
	}
	set_motors(0,0);
	play_from_program_space(starting);
}

void move() {
	// Get the position of the line.  Note that we *must* provide
	// the "sensors" argument to read_line() here, even though we
	// are not interested in the individual sensor readings.
	unsigned int position = read_line(sensors,IR_EMITTERS_ON);
	int velocity = direction * speed;
	unsigned int maxSensor = max(sensors, 5);
		    
	/*if (maxSensor < reverseThreshold) {		
		if (!needToTurn) {
			needToTurn = true;
			lastDir = direction;
			integral = 0;
			integral_reverse = 0;
			set_motors(0, 0);
		}
		else {
			if (direction != lastDir) {
				velocity = direction * speed;
				lastDir = direction;
				needToTurn = false;
				set_motors(velocity, velocity);
				delay_ms(oobRevTime);
			}
		}
	}*/
	
	if (maxSensor < reverseThreshold) {
		//make the robot go backwards when the sensors leave the black tape?
		direction = -direction;
		velocity = direction * speed;
		integral = 0;
		integral_reverse = 0;
		set_motors(velocity, velocity);
		delay_ms(oobRevTime);
	}
		    
	//we are going in reverse
	else if (direction == -1) {
		
		unsigned int position = read_line(sensors, IR_EMITTERS_ON);
		// The "proportional" term should be near 0 when we are on the line
		int proportional = ( (int) position ) - 2000;

		// Compute the derivative (change) and integral (sum) of the position.
		int derivative = proportional - last_proportional_reverse;
		integral_reverse += proportional;
		
		// Remember the last position.
		last_proportional_reverse = proportional;

		// Compute the corrective difference between the two motor power settings,

		//int power_difference = proportional/200 + integral/5000 + derivative*3/4; might be good
//		int power_difference = proportional*(0.0006) + integral_reverse*(0) + derivative*(1.6); // integral_reverse * 6.4 based on k=1000
		double ku = 0.0025;
		double tu = 8.05;
		int power_difference = proportional*(0.6*ku) + derivative*(tu/8);
		
		// Compute the actual motor settings.
		if (power_difference > speed)
		power_difference = speed;
		if (power_difference < -speed)
		power_difference = -speed;
		if (power_difference < 0)
		set_motors(direction*(speed+power_difference), direction*speed);
		else
		set_motors(direction*speed, direction*(speed-power_difference));
	}
	
	else {
		unsigned int position = read_line(sensors, IR_EMITTERS_ON);
		// The "proportional" term should be 0 when we are on the line.
		int proportional = ( (int) position ) - 2000;
		
		// Compute the derivative (change) and integral (sum) of the position.
		int derivative = proportional - last_proportional;
		integral += proportional;
		
		// Remember the last position.
		last_proportional = proportional;
		// Compute the difference between the two motor power settings,
		// m1 - m2.  If this is a positive number the robot will turn
		// to the right.  If it is a negative number, the robot will
		// turn to the left, and the magnitude of the number determines the sharpness of the turn.
		int power_difference = proportional/20 + integral/10000 + derivative*3/2;

		// Compute the actual motor settings.
		if(power_difference > speed)
		power_difference = speed;
		if(power_difference < -speed)
		power_difference = -speed;
		if(power_difference < 0)
		set_motors(direction*(speed+power_difference), direction*speed);
		else
		set_motors(direction*speed, direction*(speed-power_difference));
	}
}

unsigned int max(unsigned int* arr, unsigned int len) {
	unsigned int ret = 0;
	
	for (unsigned int i = 0; i < len; i++) {
		if (arr[i] > ret) {
			ret = arr[i];
		}
	}
	
	return ret;
}

void process_messages() {
	//get one byte
	char message = pi2::STOP;
	bool newByte = get_next_byte(message);
	
	if (newByte) {
		switch (message) {
			case pi2::INIT: {
				initialize();
			} break;
		
			case pi2::FORWARD: {
				direction = 1;				
				moving = 1;
			} break;
		
			case pi2::BACKWARD: {
				direction = -1;
				moving = 1;
			} break;
		
			case pi2::STOP: {
				set_motors(0, 0);
				moving = 0;
			} break;
			
			case pi2::SHOOT: {
				play_from_program_space(reloading);
			} break;
			
			case pi2::TIMEUP: {
				set_motors(0,0);
				moving = 0;
				play_from_program_space(gameover);
				while (1) {};
			} break;
			
			default: {
				// invalid instruction, ignore
			} break;
		}
	}

	if (moving) {
		move();
	}
}

bool get_next_byte(char& byte) {
	if (OrangutanSerial::getReceivedBytes() == receive_buffer_position) {
		return false;
	}
	
	byte = receive_buffer[receive_buffer_position];
	increment_buffer_position();
	return true;
}

void increment_buffer_position()
{
	if (receive_buffer_position == sizeof(receive_buffer)-1)
        {
            receive_buffer_position = 0;
        }
        else
        {
            receive_buffer_position++;
        }
}
