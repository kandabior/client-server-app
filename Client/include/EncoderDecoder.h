//
// Created by kandabi on 1/7/20.
//
#ifndef BOOST_ECHO_CLIENT_ENCODERDECODER_H
#define BOOST_ECHO_CLIENT_ENCODERDECODER_H

#include <list>
#include <string>
#include <vector>
#include "Frame.h"
#include "Header.h"



class EncoderDecoder{
private:
    std::list<char> bytes;
    int len=0;

public:

    EncoderDecoder():bytes(),len(){}
    Frame* decodeNextByte(char nextByte);
    std::list<char> encode(Frame frame);
    Frame stringToFrame(std::string& );

    Header stringToHeader(std::string &basicString);
};


#endif //BOOST_ECHO_CLIENT_ENCODERDECODER_H