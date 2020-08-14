//
// Created by kandabi on 1/7/20.
//

#ifndef BOOST_ECHO_CLIENT_HEADER_H
#define BOOST_ECHO_CLIENT_HEADER_H

#include <string>

class Header{

private:
    std::string name;
    std::string value;

public:
    ~Header()= default;
    Header(std::string,std::string);
    std::string getName(){ return name;};
    std::string getValue(){ return value;};
    void setValue(std::string){this->value=value;}
    std::string toString();

};

#endif //BOOST_ECHO_CLIENT_HEADER_H
