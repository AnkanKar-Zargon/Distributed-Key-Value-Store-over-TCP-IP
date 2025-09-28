#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#define PORT 8080
#define BACKLOG 10
#define BUFSIZE 1024

std::unordered_map<std::string, std::string> kv_store; // in-memory store

// Helper: split string by spaces
std::vector<std::string> split(const std::string &s) {
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Handle each client connection
void handle_client(int client_fd) {
    char buffer[BUFSIZE];
    while (true) {
        memset(buffer, 0, BUFSIZE);
        int bytes_received = recv(client_fd, buffer, BUFSIZE - 1, 0);
        if (bytes_received <= 0) break;

        std::string request(buffer);
        auto tokens = split(request);
        std::string response;

        if (tokens.size() >= 3 && tokens[0] == "SET") {
            kv_store[tokens[1]] = tokens[2];
            response = "OK\n";
        } else if (tokens.size() >= 2 && tokens[0] == "GET") {
            if (kv_store.find(tokens[1]) != kv_store.end()) {
                response = kv_store[tokens[1]] + "\n";
            } else {
                response = "NOT_FOUND\n";
            }
        } else if (tokens.size() >= 1 && tokens[0] == "EXIT") {
            response = "BYE\n";
            send(client_fd, response.c_str(), response.size(), 0);
            break;
        } else {
            response = "ERROR: Use SET key value | GET key | EXIT\n";
        }

        send(client_fd, response.c_str(), response.size(), 0);
    }
    close(client_fd);
    exit(0); // child exits
}

int main() {
    signal(SIGCHLD, SIG_IGN); // prevent zombie processes

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_addr{}, client_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "🚀 Server started on port " << PORT << "\n";

    while (true) {
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // Fork a process per client
        if (fork() == 0) {
            close(server_fd); // child doesn’t need listener
            handle_client(client_fd);
        } else {
            close(client_fd); // parent doesn’t need client socket
        }
    }
    close(server_fd);
    return 0;
}
