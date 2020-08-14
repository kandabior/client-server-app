#include "../include/connectionHandler.h"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>


using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_)
                    , encoderDecoder(),isConected(false),mutex(){}
    
ConnectionHandler::~ConnectionHandler() {
    close();

}

 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
            //std::cout<<"try to reed"<<std::endl;
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed ahalan (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
Frame ConnectionHandler::getFrame() {

    std::string line;
    Frame frame;
    if (getFrameAscii(line, '\0')) {
        frame = encoderDecoder.stringToFrame(line);
        return frame;
    }
    else{
        frame.setName("code 0");
        return frame;
    }

}


    bool ConnectionHandler::sendLine(std::string &s) {
        std::lock_guard<std::mutex> lock(mutex);
        return sendFrameAscii(s, '\0');


    }


    bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter) {
        char ch;
        // Stop when we encounter the null character.
        // Notice that the null character is not appended to the frame string.
        try {
            do {
                if (!getBytes(&ch, 1)) {
                    return false;
                }
                if (ch != '\0')
                    frame.append(1, ch);
            } while (delimiter != ch);
        } catch (std::exception &e) {
            std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
            return false;
        }
        return true;
    }


    bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
        bool result = sendBytes(frame.c_str(), frame.length());
        if (!result) return false;
        return sendBytes(&delimiter, 1);
    }

// Close down the connection properly.
    void ConnectionHandler::close() {
        try {
            isConected=false;
            socket_.close();
        } catch (...) {
            std::cout << "closing failed: connection already closed" << std::endl;
        }
    }

void ConnectionHandler::disconnect() {
    isConected=false;

}


