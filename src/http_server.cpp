#include "../include/http_server.h"
#include <sys/types.h>    // Contains definitions for data types used in system calls (e.g., `socket` function)
#include <sys/socket.h>   // Contains socket-related functions and structures (e.g., `socket`, `bind`, `sockaddr`)
#include <netinet/in.h>   // Contains constants and structures needed for internet domain addresses (e.g., `sockaddr_in`)
#include <arpa/inet.h>    // Contains definitions for internet operations (e.g., `htons`, `inet_pton`)
#include <unistd.h>       // Contains functions for close() and other system calls (e.g., `read`, `write`)
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <boost/beast.hpp> // for parsing http requests

using std::cout;

TcpServer::TcpServer(std::string ip, int port)
    :m_ip{ip}, m_port{port} {
    openSocket();
}

TcpServer::~TcpServer(){
    closeSocket();
}

void TcpServer::openSocket(){
    // Create a tcp socket 
    m_socket = socket(AF_INET, SOCK_STREAM, 0);   
    if (m_socket==-1){
        std::cerr << "Error socket creation";
        throw std::runtime_error("Failed to create socket");
    }

    // Define port info
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ip.c_str(), &serverAddress.sin_addr);

    // bind the socket
    int bind_res = bind(m_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (bind_res==-1){
        std::cerr << "Error binding socket\n";
        throw std::runtime_error("Failed to bind socket");
    }

    cout << "Created socket on " << m_ip << ":" << m_port << "\n";
}

void TcpServer::handleRequest(){
    namespace beast = boost::beast;   
    namespace http = beast::http;     
    cout << "Handling request " << m_req.target() << std::endl;

    if (m_req.method() == http::verb::get){
        std::string file_path = std::string("./resources")+std::string(m_req.target());
        cout << file_path << "\n";
        std::ifstream file(file_path);

        if(!file.is_open()){
            // 404 resource not found
            cout << "Unable to open file at " << file_path << "\n";
            std::string response = "HTTP/1.1 404 Not Found\r\n"
                       "Content-Type: text/html\r\n"
                       "\r\n"
                       "<html><body><h1>404 Not Found</h1><p>Resource not found.</p></body></html>"; 
            send(m_clientSocket, response.c_str(), response.length(), 0);
            return;
        }
        
        // 200 OK
        std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::string response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(file_content.length()) + "\r\n\r\n";

        response += file_content;
            
        send(m_clientSocket, response.c_str(), response.length(), 0);
        return;
    } else{
        // 501 We dont support non-get requests
        std::string response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
        send(m_clientSocket, response.c_str(), response.length(), 0);
        return;
    }
}

void TcpServer::parseRequest(const char(&buffer)[1024]) {
    namespace beast = boost::beast;   
    namespace http = beast::http;     

    // Get our char[] into a flat buffer
    beast::flat_buffer flatBuffer;
    size_t buffLen = std::strlen(buffer);

    // Ensure buffer is properly terminated
    std::string str = static_cast<std::string>(buffer);  
    std::size_t found = str.find("\r\n\r\n");
    if (found == std::string::npos){
        std::cerr << "Request not properly terminated" << std::endl;
        return;
    }

    // Move buff to flatbuffer
    flatBuffer.commit(boost::asio::buffer_copy(
        flatBuffer.prepare(buffLen),
        boost::asio::buffer(buffer, buffLen)));

    // create http parser
    http::request_parser<http::string_body> parser;

    beast::error_code ec;
    // feeds data from flatbuffer to parser
    parser.put(flatBuffer.data(), ec);

    if (ec){
        std::cerr << "Error with parsing HTTP request:\n" << ec.message()
            << std::endl;
        return;
    }

    if (parser.is_done()){
        // get the request object
        m_req = parser.release();

        cout << "Successfully parsed HTTP " << m_req.method_string()
            << " request." << std::endl;

        handleRequest();
    }else{
        std::cerr << "Parser couldn't finish parsing request";
    }
}

void TcpServer::startListen(){
    cout << "Listening for connections\n";

    while(true){
        listen(m_socket, 5);

        m_clientSocket = accept(m_socket, nullptr, nullptr);

        char buffer[1024] = {};
        recv(m_clientSocket , buffer, 1024, 0);
        std::cout << "Recieved request:\n" << buffer << std::endl; 

        // Parse http request with boost beast
        parseRequest(buffer);

        // Close socket
        close(m_clientSocket );
    }
}


void TcpServer::closeSocket(){
    close(m_socket);
    close(m_clientSocket);
    cout << "Closed socket";
}
