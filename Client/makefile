CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

#All Targets
all: Client
Client: bin/Main.o bin/StompClient.o bin/connectionHandler.o bin/Frame.o bin/EncoderDecoder.o bin/Header.o

	g++ -pthread -o bin/Client bin/Main.o bin/StompClient.o bin/connectionHandler.o bin/Frame.o bin/EncoderDecoder.o bin/Header.o $(LDFLAGS)

bin/Main.o: src/Main.cpp
	g++	-pthread	$(CFLAGS) -o bin/Main.o src/Main.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++	-pthread	$(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/StompClient.o: src/StompClient.cpp
	g++	-pthread	$(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

bin/Frame.o: src/Frame.cpp
	g++	-pthread	$(CFLAGS) -o bin/Frame.o src/Frame.cpp
	
bin/EncoderDecoder.o: src/EncoderDecoder.cpp
	g++	-pthread	$(CFLAGS) -o bin/EncoderDecoder.o src/EncoderDecoder.cpp
	
bin/Header.o: src/Header.cpp
	g++	-pthread	$(CFLAGS) -o bin/Header.o src/Header.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
