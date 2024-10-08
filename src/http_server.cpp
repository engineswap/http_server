#include "../include/http_server.h"
#include <sys/types.h>    // Contains definitions for data types used in system calls (e.g., `socket` function)
#include <sys/socket.h>   // Contains socket-related functions and structures (e.g., `socket`, `bind`, `sockaddr`)
#include <netinet/in.h>   // Contains constants and structures needed for internet domain addresses (e.g., `sockaddr_in`)
#include <arpa/inet.h>    // Contains definitions for internet operations (e.g., `htons`, `inet_pton`)
#include <unistd.h>       // Contains functions for close() and other system calls (e.g., `read`, `write`)
#include <iostream>
#include <stdexcept>

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

void TcpServer::startListen(){
    cout << "Listening for connections\n";

    while(true){
        listen(m_socket, 5);

        int clientSocket = accept(m_socket, nullptr, nullptr);

        char buffer[1024] = {};
        recv(clientSocket, buffer, 1024, 0);
        std::cout << "Recieved request:\n" << buffer << "\n"; 
    }
}


void TcpServer::closeSocket(){
    close(m_socket);
    cout << "Closed socket";
}
