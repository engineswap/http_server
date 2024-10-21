# pragma once

#include <string>
#include <boost/beast.hpp> // for parsing http requests

class TcpServer{
private:
    std::string m_ip;
    int m_port;

    int m_socket;
    int m_clientSocket;
    boost::beast::http::request<boost::beast::http::string_body> m_req;
public:
    TcpServer(std::string ip, int port);
    ~TcpServer();

    void openSocket();
    void closeSocket();
    void startListen();
    void parseRequest(const char(&buffer)[1024]);
    void handleRequest();
};
