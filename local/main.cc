#include <iostream>
#include <thread>

#include "pi2core/ColorBallAlgorithm.h"
#include "pi2core/BarcodeAlgorithm.h"
#include "pi2core/AlgorithmInterface.h"
#include "pi2core/TestAlg.h"
#include "pi2core/RemoteAlg.h"

#include <cppcms/applications_pool.h>
#include <cppcms/service.h>

#include "pi2core/HTTPServer.h"
#include "pi2core/SocketServerManager.h"
#include "pi2core/CommandQueue.h"
#include "pi2core/FrameQueue.h"
#include "pi2core/pi2serialenums.h"
#include "pi2core/HTTPServerManager.h"
#include "pi2core/SharedVariables.h"


int main(int argc,char ** argv){
	//Setup Shared Vars
	FrameQueue* fq = new FrameQueue();
	CommandQueue* cq = new CommandQueue();
	int localScore = 0;
	SharedVariables* sv = new SharedVariables(fq, cq, &localScore);

	if (argc != 4){
		std::cout << "Usage is ./testExec <algorithm> -c <path_to_config>" << std::endl;
		std::cout << "Available algorithms:" << std::endl;
		std::cout << "\tremote" << std::endl;
		std::cout << "\tcolorball" << std::endl;
		return 1;
	}

	AlgorithmInterface* alg = NULL;
	char* algName = argv[1];
	
	if (strcmp(algName, "remote") == 0){
		alg = new RemoteAlg(fq,cq);
	} else if (strcmp(algName, "colorball") == 0){
		alg = new ColorBallAlgorithm(fq,cq);
	} else if (strcmp(algName, "barcode") == 0) {
		alg = new BarcodeAlgorithm(fq,cq,&localScore);
	} else {
		std::cout << "Available algorithms:" << std::endl;
		std::cout << "\tremote" << std::endl;
		std::cout << "\tcolorball" << std::endl;
		std::cout << "\tbarcode" << std::endl;
		return 1;		
	}

	//Create and run threads
	std::cout << "Running Algorithm" << std::endl;
	std::thread algThread(&AlgorithmInterface::pi2Main, alg);

	std::cout << "Running HTTP Server" << std::endl;
	HTTPServerManager httpSrv;
	std::thread httpThread(&HTTPServerManager::run, &httpSrv, argc, argv, sv, algName);

	std::cout << "Running Socket Server" << std::endl;
	SocketServerManager socSrv;
	std::thread socThread(&SocketServerManager::run, &socSrv, cq);

	//Wait for execution to finish
	algThread.join();
	httpThread.join();
	socThread.join();
    
	return 0;
}
