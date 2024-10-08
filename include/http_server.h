# pragma once

#include <string>

class TcpServer{
private:
    std::string m_ip;
    int m_port;

    int m_socket;

public:
    TcpServer(std::string ip, int port);
    ~TcpServer();

    void openSocket();
    void closeSocket();
    void startListen();
};
