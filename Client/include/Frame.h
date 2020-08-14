//
// Created by kandabi on 1/7/20.
//

#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H


#include <string>
#include <list>
#include <vector>
#include "Header.h"

class Frame {
private:

    std::string name;
    std::list<Header> headers;
    std::string body;

public:
    ~Frame()= default;
    Frame(std::string name, std::string body, std::list<Header> headers);
    Frame();
    std::string getBody(){return body;}
    std::string getName(){return name;}
    std::list<Header> getHeaders(){return headers;}
    std::string getValueHeaders(std::string name);
    std::string toString();
    std::string check(std::vector<std::string>);


    void setName(std::string string);
};


#endif //BOOST_ECHO_CLIENT_FRAME_H
