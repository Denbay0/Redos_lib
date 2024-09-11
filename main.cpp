#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

#define PORT 8080

std::string process_calculation(const std::string& command) {
    std::istringstream iss(command);
    std::string op;
    double num1, num2;
    char operation;

    iss >> num1 >> operation >> num2;

    switch (operation) {
    case '+':
        return std::to_string(num1 + num2);
    case '-':
        return std::to_string(num1 - num2);
    case '*':
        return std::to_string(num1 * num2);
    case '/':
        if (num2 != 0) {
            return std::to_string(num1 / num2);
        }
        else {
            return "Error: Division by zero";
        }
    default:
        return "Error: Unknown operation";
    }
}

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    std::string response;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Binding failed" << std::endl;
        return;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        read(new_socket, buffer, 1024);
        std::string command(buffer);
        std::cout << "Received command: " << command << std::endl;

        response = process_calculation(command);

        send(new_socket, response.c_str(), response.size(), 0);
        std::cout << "Response sent: " << response << std::endl;

        close(new_socket);
    }

    close(server_fd);
}

int main() {
    start_server();
    return 0;
}