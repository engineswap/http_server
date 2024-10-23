# pragma once

#include <condition_variable>
#include <string>
#include <boost/beast.hpp> // for parsing http requests
#include <queue>
#include <mutex>

namespace beast = boost::beast;   
namespace http = beast::http;     

class TcpServer{
private:
    std::string m_ip;
    int m_port;

    int m_socket;
    const int m_thread_count = 6;
    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cond_var;
    std::queue<int> m_task_queue;
public:
    TcpServer(std::string ip, int port);
    ~TcpServer();
    
    void worker(int thread_id);
    void openSocket();
    void closeSocket();
    void startListen();
    void handleClient(int clientSocket);
    void parseRequest(const char(&buffer)[1024], int clientSocket);
    void handleRequest(const http::request<http::string_body>& req, int clientSocket);
};
