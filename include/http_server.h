# pragma once

#include <string>
#include <boost/beast.hpp> // for parsing http requests

namespace beast = boost::beast;   
namespace http = beast::http;     

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
    void handleClient(int clientSocket);
    void parseRequest(const char(&buffer)[1024], int clientSocket);
    void handleRequest(const http::request<http::string_body>& req, int clientSocket);
};
