//
// Created by simon on 5/14/2024.
//

#ifndef P2_TCPSERVER_H
#define P2_TCPSERVER_H


class TCPServer {
public:
    explicit TCPServer(u_short listening_port);
    virtual ~TCPServer();

    // don't allow anything other than explicit construction:
    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    virtual void serve();

protected:
    virtual void start_client(const std::string &their_host, u_short their_port) {}
    virtual bool process(const std::string &incoming_stream_piece) {return false;}
    virtual void respond(const std::string &response);

private:
    int server;  // socket for listening
    int client;  // sockets for a single client
};


#endif //P2_TCPSERVER_H
