CPPFLAGS = -std=c++20 -Wall -Werror -pedantic -ggdb -pthread

all: participant coordinator

participant: participant.o TCPServer.o
	g++ $(CPPFLAGS) -o participant participant.o TCPServer.o

coordinator: coordinator.o TCPClient.o
	g++ $(CPPFLAGS) -o coordinator coordinator.o TCPClient.o

participant.o: participant.cpp TCPServer.h
	g++ $(CPPFLAGS) -c participant.cpp

coordinator.o: coordinator.cpp TCPClient.h
	g++ $(CPPFLAGS) -c coordinator.cpp

TCPServer.o: TCPServer.cpp TCPServer.h
	g++ $(CPPFLAGS) -c TCPServer.cpp

TCPClient.o: TCPClient.cpp TCPClient.h
	g++ $(CPPFLAGS) -c TCPClient.cpp

clean:
	rm -f *.o *_log.txt log.txt participant coordinator