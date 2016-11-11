#include "SocketServerManager.h"
#include "tcpstream.h"
#include "tcpacceptor.h"
#include <iostream>
#include "CommandQueue.h"
#include "pi2serialenums.h"

//this function received the command from ios, and push them to the queue.
void SocketServerManager::run(CommandQueue* cq){
/*    this->pr_cq = cq;
    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    Command rcvCmd;
    acceptor = new TCPAcceptor(8081);
    int speedSet = 30;
    cout << "initialize the speed to 30" << endl;
    if (acceptor->start() == 0) {
        while (1) {
            stream = acceptor->accept();
            if (stream != NULL) {
                ssize_t len;
                char line[256];
                while ((len = stream->receive(line, sizeof(line))) > 0) {
                    line[len] = 0; 
                    std::string cmd = std::string(line);
                    std::cout << "running Command: " << cmd  << std::endl;
                    std::string speedBool = cmd.substr(0,5);
		    std::string f = cmd.substr(0,7);
	            std::string b = cmd.substr(0,4);
		    std::string l = cmd.substr(0,4);
	   	    std::string r = cmd.substr(0,5);
		    cout << speedBool << endl;
		    if( speedBool == "speed" ){
		    	cout << "Setting the speed NOW!" << endl;
		        speedSet = std::stoi(cmd.substr(5));
		        cout << speedSet << endl;
		    }
		    if( cmd == "forward" || f == "forward"){
			rcvCmd.type = pi2::FORWARD;
			if( f == "forward" )
			{
			   std::cout << "motion direction: " + f << endl;
		           std::string s =  cmd.substr(7);
			   std:: cout << "testing speed " << s << endl;
			   if ( s != "" ){
			      double forwardS = std::stod(s);
			      forwardS = forwardS * 100;
			      std::cout << "motion speed: " <<  forwardS << endl;
   		              rcvCmd.m1speed = forwardS;
			      rcvCmd.m2speed = forwardS; 
			   }   
			}
			if(cmd == "forward" ){
			   std::cout << "testing control" << endl;
                           rcvCmd.m1speed = speedSet;
                           rcvCmd.m2speed = speedSet;
                   	}     
                    }
                    else if( cmd == "back" || b == "back"){
                        rcvCmd.type = pi2::BACK;
			if( b == "back" )
			{
			   std::string s = cmd.substr(4);
			   if( s != ""){
			      double backS = std::stod(s);
			      backS = backS * 100;
		    	      rcvCmd.m1speed = backS;
   			      rcvCmd.m2speed = backS;
			   }
			}
			if( cmd == "back" ){
                           rcvCmd.m1speed = -1 * speedSet;
                           rcvCmd.m2speed = -1 * speedSet;
			}
                    }
                    else if( cmd == "left" || l == "left" ){
                        rcvCmd.type = pi2::LEFT;
			if( l == "left" )
			{
			   std::string s = cmd.substr(4);
			   if( s != "" ){
			      double leftS = std::stod(s);
			      leftS = leftS * 100;
			      rcvCmd.m1speed =  -1 * leftS;
			      rcvCmd.m2speed = -1*leftS-30;
			   }
			}
			if( cmd == "left"){
                           rcvCmd.m1speed = speedSet - 30;
                           rcvCmd.m2speed = -1 * speedSet;
			}
                    }
                    else if( cmd == "right" || r == "right"){
                        rcvCmd.type = pi2::RIGHT;
			if( r == "right" )
			{
			   std::string s = cmd.substr(5);
			   if( s != ""){
			      double rightS = std::stod(s);
			      rightS = rightS * 100;
			      rcvCmd.m1speed =  1*rightS-30;
			      rcvCmd.m2speed = 1*rightS;
			   }
			}
			if( cmd == "right"){
                           rcvCmd.m1speed = -1 * speedSet;
                           rcvCmd.m2speed = speedSet - 30;
			}
                   
                    }
                    else if( cmd == "stop"){
                        rcvCmd.type = pi2::STOP;
                        rcvCmd.m1speed = 0;
                        rcvCmd.m2speed = 0;
                   
                    }
                    else if( cmd == "conndone"){
                        rcvCmd.type = pi2::CONNDONE;
                        rcvCmd.m1speed = 0;
                        rcvCmd.m2speed = 0;
                   
                    }
                    else{
                        rcvCmd.type = pi2::NOCOMMAND;
                        rcvCmd.m1speed = 0;
                        rcvCmd.m2speed = 0;
                   
                   
                     }
	            this->pr_cq->putCommand(rcvCmd);
                    stream->send(line, len);
                }
		delete stream;
            }

        }
    }*/
}


