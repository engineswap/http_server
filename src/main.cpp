#include "../include/http_server.h"

int main(){
    TcpServer server{"127.0.0.1", 8080};
    server.startListen();
    return 0;
}
