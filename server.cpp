#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    int server_fd,new_socket;
    struct sockaddr_in address;
    const char* message = "Hello, World\0";


    if((server_fd=socket(AF_INET, SOCK_STREAM,0 ))<0 ){
        std::cerr << "Socket creation failed";
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(5556);
    address.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        std::cerr << "Bind failed";
        close(server_fd);
        exit(1);

    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 5556...\n");

    while(true){
    socklen_t addrlen = sizeof(address);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    write(new_socket,message,strlen(message));
    std::cout << "Client connected: Message sent\n";
    char buffer[1024];
    int valread = read(new_socket, buffer, 1024);
    if(valread > 0){
        buffer[valread] = '\0';
        std::cout << "Client echoed back: \n" << buffer << "\n";
    }
    close(new_socket);

}

    close(server_fd);
    return 0;
}