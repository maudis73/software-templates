#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class SimpleRestApi {
public:
    SimpleRestApi() {}

    std::string handleRequest(const std::string& request) {
        // 1. Parse the request (very basic example)
        std::istringstream iss(request);
        std::string method, path, version;
        iss >> method >> path >> version;

        // 2. Handle the request (simplified)
        if (method == "GET" && path == "/") {
            return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
        } else {
            return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found"; 
        }
    }
};

int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    // Define server address
    struct sockaddr_in address; 
    memset(&address, 0, sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(8080); 

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }

    std::cout << "Server started on port 8080" << std::endl;

    while (true) {
        int new_socket; 
        struct sockaddr_in client_address; 
        socklen_t client_len = sizeof(client_address); 
        new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len); 
        if (new_socket < 0) { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }

        char buffer[1024] = {0}; 
        read(new_socket, buffer, 1024); 

        // Handle request using SimpleRestApi
        SimpleRestApi api;
        std::string request(buffer); 
        std::string response = api.handleRequest(request);

        send(new_socket, response.c_str(), response.length(), 0); 

        close(new_socket); 
    }

    return 0;
}
