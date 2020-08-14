//
// Created by kandabi on 1/7/20.
//

#include "../include/StompClient.h"
#include <stdlib.h>

#include <map>
#include <vector>

#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/

bool StompClient::checkBook(std::string book,std::string genre) {

    std::vector<std::string> book_vec= genre_books[genre];
    for(std::string b : book_vec){
        if(book==b){
            return true;
        }
    }
    return false;
}


std::string StompClient::getBookOwner(std::string book) {
    std::string owner=books_owner[book];
    return owner;
}

void StompClient::removeBook(std::string book, std::string genre) {
    std::lock_guard<std::mutex> lock(mutex_Genre);
    int count=0;
    for(const std::string& str: genre_books[genre]){
        if(book==str){
            genre_books[genre].erase( genre_books[genre].begin()+count);
            return;
        }
        count++;
    }
}

std::string StompClient::getSubIdbyTopic(std::string topic) {
    return topics[topic];

}

void StompClient::addWishList(std::string book) {
    std::lock_guard<std::mutex> lock(mutex_wish);
    wishList.push_back(book);
}

std::string StompClient::getTopicByReceipt(std::string receiptId) {
    return receipt[receiptId];

}

void StompClient::addBookOwner(std::string book, std::string owner) {
    std::lock_guard<std::mutex> lock(mutex_owner);
    if(books_owner.find(book)==books_owner.end()){
        books_owner[book]= owner;
    }

}

void StompClient::addBookGenre(std::string book, std::string genre){
    std::lock_guard<std::mutex> lock(mutex_Genre);
    if(genre_books.find(genre) != genre_books.end()){
        genre_books[genre].push_back(book);
    }
    else{
        std::vector<std::string> vec;
        vec.push_back(book);
        genre_books[genre] = vec;
    }
}


bool StompClient::checkInWish(std::string book) {
    for(std::string str: wishList){
        if(str==book){
            return true;
        }
    }
    return false;
}

void StompClient::keyboardReaderWriter() {

    while (connectionHandler.isConnected()) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        if (connectionHandler.isConnected()) {
            std::string ToSend = stringToFrameString(line);
            if (ToSend == "false") {
                std::cout << "user cant login" << std::endl;
            } else if (ToSend != "") {
                if (!connectionHandler.sendLine(ToSend)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
            }
        }
    }

}

void StompClient::ServerReaderWriter() {
    while (connectionHandler.isConnected()) {
        std::string answer;
        //std::cout << "try to get message" << std::endl;
        Frame frame = connectionHandler.getFrame();
        std::cout << frame.toString() << std::endl;

        answer = menageFrame(frame);
        if (answer == "") {

        } else if (answer == "disconnect") {
            connectionHandler.disconnect();
            std::cout << "write bye" << std::endl;
            connectionHandler.close();
            return;
        } else {
            connectionHandler.sendLine(answer);
        }

    }
}





std::string StompClient::stringToFrameString(std::string &ss) {
    std::vector<std::string> vector;
    boost::split(vector, ss, boost::is_any_of(" "));
    std::string message;
    message=vector.at(0);
    if(message=="login"){
        return loginFrame(ss, vector);

    }
    else if(message=="join"){
        return joinFrame(vector,SubscriptionId++,receiptId++);
    }
    else if(message=="add"){
        return addFrame(vector);
    }
    else if(message=="borrow"){
        return borrowFrame(vector);
    }
    else if(message=="return"){
        return returnFrame(vector);
    }
    else if(message=="status"){
        return statusFrame(vector);
    }
    else if(message=="exit") {
        return exitFrame(vector, receiptId++);
    }
    else if(message=="logout") {
        return logoutFrame(vector,receiptId++);
    }
    return "";
}

std::string StompClient::loginFrame(std::string str, std::vector<std::string>list) {
    std::string name = "CONNECT";
    std::vector<std::string> hostPort;
    boost::split(hostPort, list.at(1), boost::is_any_of(":"));
    std:: string host = hostPort.at(0);
    Header header("accept-version","1.2");
    Header header2("host", host);
    Header header3("login",list.at(2));
    Header header4("passcode",list.at(3));
    std::list<Header> headers;
    headers.push_back(header);
    headers.push_back(header2);
    headers.push_back(header3);
    headers.push_back(header4);
    std::string body = "";
    Frame frame(name,body,headers);
    return frame.toString();
}

std::string StompClient::joinFrame(std::vector<std::string> vector, int SubscriptionId, int receipt) {
    std::string name="SUBSCRIBE";
    Header header("destination",vector.at(1));
    Header header2("id", std::to_string(SubscriptionId));
    Header header3("receipt",std::to_string(receipt));
    std::list<Header> headers;
    headers.push_back(header);
    headers.push_back(header2);
    headers.push_back(header3);
    addReceipt(std::to_string(receipt),"subscribe "+vector.at(1) + " " + std::to_string(SubscriptionId));
    Frame frame(name,"",headers);
    return frame.toString();
}

std::string StompClient::addFrame(std::vector<std::string> vector) {
    std::string name="SEND";
    Header header("destination",vector.at(1));
    std::list<Header> headers;
    headers.push_back(header);
    std::string bookName;
    for(size_t i=2; i<vector.size(); i++){
        bookName=bookName+" "+vector.at(i);
    }

    std::string body=getName()+" has added the book"+bookName;
    Frame FAQ(name,body,headers);
    addBookGenre(bookName.substr(1), vector.at(1));
    addBookOwner(bookName.substr(1),getName());
    return FAQ.toString();
}

std::string StompClient::borrowFrame(std::vector<std::string> vector) {
    std::string name="SEND";
    Header header("destination",vector.at(1));
    std::list<Header> headers;
    headers.push_back(header);
    std::string bookName;
    for(size_t i=2; i<vector.size(); i++){
        bookName=bookName+" "+vector.at(i);
    }

    std::string body=getName()+" wish to borrow"+bookName;
    Frame FAQ(name,body,headers);
    addWishList(bookName.substr(1));
    return FAQ.toString();
}

std::string StompClient::returnFrame(std::vector<std::string> vector) {

    std::string name="SEND";
    Header header("destination",vector.at(1));
    std::list<Header> headers;
    headers.push_back(header);
    std::string bookName;
    for(size_t i=2; i<vector.size();i++){
        bookName = bookName +" "+ vector.at(i);

    }
    if(checkBook(bookName.substr(1),vector.at(1))) {
        std::string body = "Returning" + bookName + " to " + getBookOwner(bookName.substr(1));
        removeBook(bookName.substr(1), vector.at(1));
        removebookOwner(bookName.substr(1));
        Frame frame(name, body, headers);
        return frame.toString();

    }
    else{
        return "";
    }
}

std::string StompClient::statusFrame(std::vector<std::string> vector) {
    std::string name="SEND";
    Header header("destination",vector.at(1));
    std::list<Header> headers;
    headers.push_back(header);
    std::string body="book status";
    Frame frame(name,body,headers);
    return frame.toString();

}

std::string StompClient::logoutFrame(std::vector<std::string> vector, int receipt) {
    std::string name="DISCONNECT";
    Header header("receipt",std::to_string(receipt));
    std::list<Header> headers;
    headers.push_back(header);
    std::string body="";
    addReceipt(std::to_string(receipt),"disconnect");
    Frame frame(name,body,headers);
    return frame.toString();
}

std::string StompClient::exitFrame(std::vector<std::string> vector, int receipt) {
    std::string name="UNSUBSCRIBE";
    Header header("receipt",std::to_string(receipt));
    std::string id = getSubIdbyTopic(vector.at(1));
    Header header2("id", id);
    std::list<Header> headers;
    headers.push_back(header);
    headers.push_back(header2);
    std::string body="";

    addReceipt(std::to_string(receipt),"unsebscribe "+vector.at(1));
    Frame frame(name,body,headers);
    return frame.toString();

}
std::string getBookNameReturned(std::vector<std::string>& vec){
    std::string output="";
    for(size_t i=1; i<vec.size()-2; i++){
        output=output+" "+vec.at(i);
    }
    return output.substr(1);
}

std::string getBookNameBorrow(std::vector<std::string> &vec){
    std::string output="";
    for(size_t i=4; i<vec.size(); i++){
        output=output+" " +vec.at(i);
    }
    return output.substr(1);
}

std::string StompClient::menageFrame(Frame &frame) {
    std:: string name = frame.getName();
    if(name =="CONNECTED") {
        connectionHandler.SetConected();
        std::cout << "login successful" << std::endl;
        return "connected";
    }
    else if(frame.getName()=="RECEIPT") {
        std::string topic = getTopicByReceipt(frame.getValueHeaders("receipt-id"));
        std::vector<std::string> vector;
        boost::split(vector, topic, boost::is_any_of(" "));
        if (vector.at(0) == "disconnect") {
            return "disconnect";
        }
        if (vector.at(0) == "subscribe") {
            std::string genre = vector.at(1);
            std::cout << "Joined club " + genre << std::endl;
            addTopic(genre,vector.at(2));
        }
        if (vector.at(0)== "unsubscribe") {
            std::string genre = vector.at(1);
            std::cout << "Exited club " + genre << std::endl;
            std::string id = getSubIdbyTopic(genre);
            removeTopic(id);
        }
    }
    else if(name =="MESSAGE"){
        std::string ss(frame.getBody());
        std::vector<std::string> vector;
        boost::split(vector, ss, boost::is_any_of(" "));
        if(frame.getBody()=="Book status" || frame.getBody()=="book status"){
            Frame StatusFrame= createStatusFrame(frame.getValueHeaders("destination"));
            return StatusFrame.toString();
        }
        else if(frame.getBody().substr(0,9) == "Returning"){
            if(vector.at(vector.size()-1)==getName()){
                std::string bookName=getBookNameReturned(vector);
                addBookGenre(bookName,frame.getValueHeaders("destination"));
                addBookOwner(bookName, getName());
                std::cout<<"the book "<<bookName<<" has been returned"<<std::endl;
            }
        }
        else if(vector.size()>1 && vector.at(1)=="wish"){
            std::string bookName=getBookNameBorrow(vector);
            if(checkBook(bookName,frame.getValueHeaders("destination"))){
                Frame Newframe=createBorowFrame(frame.getValueHeaders("destination"),bookName);
                return Newframe.toString();
            }
        }
        else if(vector.size()>1 &&vector.at(1)=="has"){
            std::string bookName=getBookNameHas(vector);
            if(checkInWish(bookName)){
                removeWish(bookName);
                addBookOwner(bookName, vector.at(0));
                addBookGenre(bookName,frame.getValueHeaders("destination"));
                Frame Newframe=createTakingFrame(frame.getValueHeaders("destination"),bookName,vector.at(0));
                return Newframe.toString();
            }
        }
        else if(vector.size()>0 && vector.at(0)=="Taking" && vector.at(vector.size()-1)==getName()){
            std::string bookName= getBookNameReturned(vector);
            removeBook(bookName,frame.getValueHeaders("destination"));
        }
    } else if(name=="ERROR"){
        return "disconnect";
    }
    return "";
}

Frame StompClient::createStatusFrame(std::string destination) {
    std:: string name = "SEND";
    std::string body = getName() +":";
    for (std:: string book: getBooksBygen(destination)){
        body+=book+",";
    }
    body = body.substr(0,body.size()-1);
    std::list<Header> headers;
    Header header("destination",destination);
    headers.push_back(header);
    Frame frame(name,body,headers);
    return frame;
}

Frame StompClient::createBorowFrame(std::string destination, std::string &bookName) {
    std:: string name = "SEND";
    std::string body = getName() +" has "+ bookName;
    std::list<Header> headers;
    Header header("destination",destination);
    headers.push_back(header);
    Frame frame(name,body,headers);
    return frame;
}

std::string StompClient::getBookNameHas(std::vector<std::string> vector) {
    std::string output="";
    for(size_t i=2; i<vector.size(); i++){
        output=output+" "+vector.at(i);
    }
    return output.substr(1);
}

Frame StompClient::createTakingFrame(std::string destination, std::string book,std::string owner) {
    std:: string name = "SEND";
    std::string body = "Taking "+book+" from "+owner;
    std::list<Header> headers;
    Header header("destination",destination);
    headers.push_back(header);
    Frame frame(name,body,headers);
    return frame;
}

void StompClient::sendLine(std::string basicString) {
    connectionHandler.sendLine(basicString);
}

bool StompClient::connect() {
    return connectionHandler.connect();
}



void StompClient::removeWish(std::string book) {
    std::lock_guard<std::mutex> lock(mutex_wish);
    int count=0;
    for(std::string str: wishList){
        if(book==str){
            wishList.erase( wishList.begin()+count);
            return;
        }
        count++;
    }
}

void StompClient::removebookOwner(std::string book) {
    std::lock_guard<std::mutex> lock(mutex_owner);
    books_owner.erase(book);

}








