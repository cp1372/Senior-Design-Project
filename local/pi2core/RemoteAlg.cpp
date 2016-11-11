#include "RemoteAlg.h"

#include <opencv/cv.h>
#include <iostream>

#include "rpicamera.h"
#include "opencvcam.h"
#include "CommandQueue.h"
#include "FrameQueue.h"
#include "pi2serialenums.h"

#include <wiringPi.h>
#include <wiringSerial.h>

int RemoteAlg::pi2Main(){
	RPiCamera camera;

	int fd = 0;

        if ((fd = serialOpen("/dev/ttyAMA0", 9600)) < 0){
                std::cout << "Unable to open serial device" << std::endl;
                return 1 ;
        }

        if (wiringPiSetup() == -1 ){
                std::cout << "Unable to start wiringPi" << std::endl;
		return 1;
        }

	while(true){
		//Create variables
		cv::Mat frame;

		//Load image
		camera.getFrame(frame);

		//Handle Msg
	        
		Command c = this->commandQueue->getCommand();
		//std::cout << c.type << std::endl;		
		if (c.type != pi2::NOCOMMAND){
			std::cout << "Setting Command: " << c.type << std::endl; 
			serialPutchar(fd, pi2::SETM1SPEED);
        		serialPutchar(fd, (char)c.m1speed);

        		serialPutchar(fd, pi2::SETM2SPEED);
        		serialPutchar(fd, (char)c.m2speed);
		}

		//place img to be read
		std::vector<uchar> buffer;
		cv::imencode(".jpg", frame, buffer);
		this->frameQueue->putFrame(buffer);
	}
	return 0;
}
