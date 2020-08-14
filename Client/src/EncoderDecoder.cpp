//
// Created by kandabi on 1/7/20.
//
#include <boost/asio/ip/tcp.hpp>


#include "../include/EncoderDecoder.h"
#include <bits/stdc++.h>
#include <list>
#include <sstream>
#include <iterator>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;


Frame EncoderDecoder::stringToFrame(std::string & str) {
    std::vector<std::string> svec;
    boost::split(svec,str,boost::is_any_of("\n"));
    std::string name = svec.at(0);
    std::list<Header> headers;
    size_t i;
    for(i=1; i<svec.size() && svec[i]!= ""; i++){
        headers.push_back(stringToHeader(svec.at(i)));
    }
    i++;
    std::string body;
    for(;i<svec.size();i++) {
        body = body+ svec.at(i)+"\n";

    }
    body.erase(std::remove(body.begin(), body.end(), '\n'), body.end());

    Frame frame(name,body,headers);
    return frame;

}


Header EncoderDecoder::stringToHeader(std::string & str) {
    std::vector<std::string> svec;
    boost::split(svec,str,boost::is_any_of(":"));
    Header h(svec.at(0),svec.at(1));
    return h;


}
