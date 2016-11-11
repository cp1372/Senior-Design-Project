#include "HTTPServer.h"
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/json.h>
#include <iostream>
#include <vector>
#include <opencv/cv.h>

#include "FrameQueue.h"
#include "CommandQueue.h"
#include "JSONService.h"
#include "pi2serialenums.h"
#include "SharedVariables.h"

// See for help: http://cppcms.com/wikipp/en/page/cppcms_1x_tut_url_mapping#Introduction
HTTPServer::HTTPServer(cppcms::service &srv, SharedVariables* sv, char* alg): cppcms::application(srv) {
    this->srv_fq = sv->fq;
    this->srv_cq = sv->cq;
    this->localScore = sv->localScore;
    this->alg = std::string(alg);

    // attach(new JSONService(srv, this->srv_cq), 
    //     "/command", "/command{1}",
    //     "/command(/(.*))?", 1);

    dispatcher().assign("/stream",&HTTPServer::stream,this);
    mapper().assign("stream","/stream");

    dispatcher().assign("/placecom/((.*))?", &HTTPServer::placecom, this, 1);
    mapper().assign("placecom", "/placecom/{1}");

    dispatcher().assign("/color/(\\d+)/((.*))?", &HTTPServer::color, this, 1, 2);
    mapper().assign("color", "/color/{1}/{2}");

    dispatcher().assign("/frame/(\\d+)",&HTTPServer::frame,this,1);
    mapper().assign("frame", "/frame/{1}");

    dispatcher().assign("/algType",&HTTPServer::algType,this);
    mapper().assign("algType","/algType");

    dispatcher().assign("/score",&HTTPServer::score,this,1);
    mapper().assign("score","/score");

    mapper().root("/");
}

void HTTPServer::frame(std::string frameNum){
    int num = atoi(frameNum.c_str());
    if (num == 1 || num == 2){
        std::vector<uchar> frame = srv_fq->getFrame(num);
        response().set_content_header("image/jpeg");
        for (int i = 0; i < frame.size(); ++i){
            response().out()<< frame[i];
        }
    } 
}

void HTTPServer::color(std::string val, std::string type){
    Command c;
    int num = atoi(val.c_str());
    if (type == "HMax") {
        c.type = pi2::HMAX;
        c.m1speed = num;
    } else if (type == "HMin"){
        c.type = pi2::HMIN;
        c.m1speed = num;
    } else if (type == "SMax"){
        c.type = pi2::SMAX;
        c.m1speed = num;
    } else if (type == "SMin"){
        c.type = pi2::SMIN;
        c.m1speed = num;
    } else if (type == "VMax"){
        c.type = pi2::VMAX;
        c.m1speed = num;
    } else if (type == "VMin"){
        c.type = pi2::VMIN;
        c.m1speed = num;
    }
    this->srv_cq->putCommand(c);
}

void HTTPServer::stream(){
        std::vector<uchar> frame = srv_fq->getFrame();
        response().set_content_header("image/jpeg");
        for (int i = 0; i < frame.size(); ++i)
        {
            response().out()<< frame[i];
        }
}

void HTTPServer::placecom(std::string msg){
    std::cout << "Placing command with msg: " << msg << std::endl;
    Command c;
    c.type = -1;

    if (msg == "init") {
        c.type = pi2::INIT;
    } else if (msg == "forward") {
        c.type = pi2::FORWARD;
    } else if (msg == "backward") {
        c.type = pi2::BACKWARD;
    } else if (msg == "stop") {
        c.type = pi2::STOP;
    } else if (msg == "shoot") {
	c.type = pi2::SHOOT;
    } else if (msg == "timeup") {
	c.type = pi2::TIMEUP;
    }

    if (c.type != -1) {
        std::cout << "test: " << c.type << std::endl;
        this->srv_cq->putCommand(c);
    }
}

void HTTPServer::algType(){
    cppcms::json::value algInfo;
    algInfo["algorithm"] = this->alg;
    if (alg.compare("remote") == 0){
        algInfo["frames"] = 1;
        algInfo["framedelay"] = 250;
    } else if (alg.compare("colorball") == 0) {
        algInfo["frames"] = 2;
        algInfo["framedelay"] = 500;
    }
    response().set_content_header("application/json");
    response().out() << algInfo;
}

void HTTPServer::score(std::string val) {
	response().set_header("Access-Control-Allow-Origin","*");
	response().out() << *(this->localScore);
}
