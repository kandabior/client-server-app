//
// Created by kandabi@wincs.cs.bgu.ac.il on 16/01/2020.
//


#include <iostream>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <StompClient.h>
#include <thread>

int main (int argc, char *argv[]) {
    bool connected(false);
    while(!connected) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        std::string ss = line;
        std::vector<std::string> vector;
        boost::split(vector, ss, boost::is_any_of(" "));

        std::string message;
        message = vector.at(0);
        if (message == "login") {

            std::vector<std::string> hostPort;
            boost::split(hostPort, vector.at(1), boost::is_any_of(":"));

            short port = std::stoi(hostPort.at(1));
            std::string host = hostPort.at(0);

            StompClient stompClient (vector.at(2),vector.at(3),host,port);
            if (stompClient.connect()) {

                std::string s = stompClient.stringToFrameString(ss);
                stompClient.sendLine(s);

                std::string answer;
                //std::cout<<"try to get message"<<std::endl;
                Frame frame=stompClient.getConnectionHandler().getFrame();
                std::cout<<frame.toString()<<std::endl;
                answer= stompClient.menageFrame(frame);
                if(answer=="connected") {
                    connected = true;
                    std::thread th1(&StompClient::keyboardReaderWriter, &stompClient);
                    std::thread th2(&StompClient::ServerReaderWriter, &stompClient);
                    th2.join();
                    std::cout << "th1 joined" << std::endl;
                    th1.join();
                    std::cout << "th2 joined" << std::endl;
                }
                else{
                    //std::cout<<"user already logged in"<<std::endl;
                    return 0;
                }
            }
        }
        else{
            std::cout<<"user must login first"<<std::endl;
        }
    }

    return 0;


}