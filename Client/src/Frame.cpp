//
// Created by kandabi on 1/7/20.
//

#include "../include/Frame.h"

Frame::Frame(std::string name, std::string body, std::list<Header> headers) : name(name), headers(headers) ,body(body){}

Frame::Frame(): name(), headers() ,body(){}

std::string Frame::getValueHeaders(std::string name) {
    for(Header header: headers){
        if(header.getName() == name){
            return header.getValue();

        }
    }
    return nullptr;
}

std::string Frame::toString() {
    std::string output;
    output=output+name+"\n";
    for(Header header: headers){
        output=output+header.toString()+"\n";
    }
    output=output+"\n";
    output=output+body+"\n";
    output=output+"\0";
    return output;

}

void Frame::setName(std::string string) {
    name=string;
}

