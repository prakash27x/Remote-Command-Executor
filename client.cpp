#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define BUFFER_SIZE 4096

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // Ask user for IP and port
    std::string serverIP;
    int serverPort;
    std::cout << "Enter server IP address: ";
    std::getline(std::cin, serverIP);
    std::cout << "Enter server port: ";
    std::cin >> serverPort;
    std::cin.ignore(); // Clear newline

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server.\n";

    while (true) {
        std::cout << "\nEnter command (or 'exit' to quit): ";
        std::cin.getline(buffer, BUFFER_SIZE);

        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        char recvBuffer[BUFFER_SIZE];
        ZeroMemory(recvBuffer, BUFFER_SIZE);

        int bytesReceived = recv(clientSocket, recvBuffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            recvBuffer[bytesReceived] = '\0';
            std::cout << "Output:\n" << recvBuffer;
        } else {
            std::cout << "No response or connection closed.\n";
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}