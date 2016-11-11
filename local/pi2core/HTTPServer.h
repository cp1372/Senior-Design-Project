#ifndef HELLO_H_
#define HELLO_H_

#include <cppcms/application.h>
#include <cppcms/service.h>

class FrameQueue;
class CommandQueue;
class SharedVariables;

class HTTPServer : public cppcms::application {
public:
    HTTPServer(cppcms::service &srv, SharedVariables* sv, char* algType);
    void stream();
    void placecom(std::string msg);
    void algType();
    void frame(std::string frameNum);
    void color(std::string type, std::string val);
    void score(std::string scoreNum);


private:
    FrameQueue* srv_fq;
    CommandQueue* srv_cq;
    std::string alg;
    int* localScore;
};

#endif
