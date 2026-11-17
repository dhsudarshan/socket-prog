#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#define PORT 5556
int main(){
    int client_fd;
    struct sockaddr_in address;
    char buffer[1024];
    

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
        std::cerr << "Socket creation failed";
        exit(1);
    }

    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        std::cerr << "\nInvalid address / Address not supported\n";
        exit(1);
    }


    if(connect(client_fd, (sockaddr *)&address, sizeof(address))<0){
        std::cerr << "Connection failed";
        exit(1);
    }
    std::cout << "Connection successful\n";
    int valread = read(client_fd, buffer, 1024);
    if (valread > 0) {
        buffer[valread] ='\0';
        std::cout << "Received from server:\n" << buffer << std::endl;
    }
    write(client_fd, buffer, valread);
    close(client_fd);
    return 0;
}