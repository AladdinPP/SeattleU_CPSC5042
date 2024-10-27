/**
 * @author  Hongru He
 * @file    coordinator.h
 * @version 2.0
 * @date    05/31/2024
 */

#ifndef P2_TCPCLIENT_H
#define P2_TCPCLIENT_H

#pragma once
#include <string>
#include <iostream>

class TCPClient {
public:
    // Default constructor for initializing the vector
    TCPClient() = default;
    TCPClient(const std::string &server_host, u_short server_port);
    TCPClient(TCPClient &&other) noexcept;  // we may want this for std::vector building
    TCPClient& operator=(TCPClient &&) noexcept;    // for vector assignment
    virtual ~TCPClient();

    // don't allow any of these:
    TCPClient(const TCPClient &) = delete;
    TCPClient& operator=(const TCPClient &) = delete;

    void send_request(const std::string &request);
    std::string get_response();

private:
    int server;  // socket
};


#endif //P2_TCPCLIENT_H
