#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

#define PORT 8080
#define BUFSIZE 1024

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Connected to server. Use commands: SET key value | GET key | EXIT\n";

    char buffer[BUFSIZE];
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;

        send(sock, input.c_str(), input.size(), 0);

        memset(buffer, 0, BUFSIZE);
        int bytes_received = recv(sock, buffer, BUFSIZE - 1, 0);
        if (bytes_received <= 0) break;

        std::cout << buffer;

        if (input == "EXIT") break;
    }

    close(sock);
    return 0;
}
