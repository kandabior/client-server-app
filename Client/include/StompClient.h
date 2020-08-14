//
// Created by kandabi on 1/7/20.
//

#ifndef BOOST_ECHO_CLIENT_STOMPCLIENT_H
#define BOOST_ECHO_CLIENT_STOMPCLIENT_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include <mutex>
#include "Header.h"
#include "Frame.h"
#include "../include/connectionHandler.h"


class StompClient{
private:
    std::string name;
    std::string passcode;
    std::map<std::string,std::string> books_owner; //book names and owners
    std::map<std::string,std::vector<std::string>> genre_books; //genere and list of books
    std::map<std::string,std::string> receipt; //recipt id and commend-topic
    std::map<std::string,std::string> topics; // topic and id subscription
    std::vector<std::string> wishList;
    ConnectionHandler connectionHandler;
    std::mutex mutex_owner;
    std::mutex mutex_Genre;
    std::mutex mutex_wish;
    int SubscriptionId;
    int receiptId;


public:
    StompClient(std::string name_,std::string passcode_,std::string host, short port): name(name_), passcode(passcode_), books_owner(), genre_books(), receipt(), topics(), wishList()
        ,connectionHandler(host,port), mutex_owner(), mutex_Genre(),mutex_wish(),SubscriptionId(1),receiptId(1){}

    std::string getName(){return name;}
    ConnectionHandler& getConnectionHandler(){ return connectionHandler;}
    std::string getSubIdbyTopic(std::string topic);
    void addBookOwner(std::string book,std::string owner);
    void addBookGenre(std::string book,std::string genere);
    void addWishList(std::string book);

    void removeBook(std::string book, std:: string genre);
    bool checkBook(std::string book,std::string genre);
    std::string getBookOwner(std::string book);
    void removeTopic(std::string id){topics.erase(id);}
    void addReceipt(std::string id,std::string str){receipt[id]=str;}
    void addTopic(std::string topic,std::string id){topics[topic]=id;}
    std::vector<std::string>& getBooksBygen(std::string genere){return genre_books[genere];}

    std::string getTopicByReceipt(std::string basicString);

    bool checkInWish(std::string basicString);

    std::string stringToFrameString(std::string & str );
    std::string loginFrame(std::string basicString, std::vector<std::string>);

    std::string joinFrame(std::vector<std::string> vector,int,int );


    std::string addFrame(std::vector<std::string> vector);
    std::string borrowFrame(std::vector<std::string> vector);

    std::string returnFrame(std::vector<std::string> vector);

    std::string statusFrame(std::vector<std::string> vector);

    std::string logoutFrame(std::vector<std::string> vector,int);

    std::string exitFrame(std::vector<std::string> vector,int);

    std::string menageFrame(Frame& frame);

    Frame createStatusFrame(std::string basicString);

    Frame createBorowFrame(std::string ,std::string&);

    std::string getBookNameHas(std::vector<std::string> vector);

    Frame createTakingFrame(std::string , std::string , std::string );

    void keyboardReaderWriter();
    void ServerReaderWriter();

    void sendLine(std::string basicString);



    bool connect();



    void removeWish(std::string basicString);

    void removebookOwner(std::basic_string<char, std::char_traits<char>, std::allocator<char>> basicString);
};
#endif //BOOST_ECHO_CLIENT_STOMPCLIENT_H
