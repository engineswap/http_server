#include "../include/http_server.h"
#include <iostream>
#include <memory>

// using std::cout;
using std::unique_ptr;

unique_ptr<TcpServer> server_ptr;

// handle cntl-c kill signal
void signalHandler(int signal){
    if (signal==SIGINT){
        server_ptr.reset();
    }
    exit(0);
}


int main(){
    // register signal handler
    signal(SIGINT, signalHandler);

    server_ptr = std::make_unique<TcpServer>("127.0.0.1", 8080);
    server_ptr->startListen();
    return 0;
}
