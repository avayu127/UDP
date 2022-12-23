#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>

#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    sockaddr_in send_addr;
    const size_t s_warning_size = 1024;

    SYSTEMTIME lt;
    GetLocalTime(&lt);

    if (lt.wHour >= 4 && lt.wHour <= 11)
    {
        char s_warning[s_warning_size] = "Good morning!";
        int sendResult = sendto(udpSocket, s_warning, strlen(s_warning), 0, (SOCKADDR*)&send_addr, sizeof(send_addr));
    }
    else if (lt.wHour >= 12 && lt.wHour <= 15)
    {
        char s_warning[s_warning_size] = "Good day!";
        int sendResult = sendto(udpSocket, s_warning, strlen(s_warning), 0, (SOCKADDR*)&send_addr, sizeof(send_addr));
    }
    else if (lt.wHour >= 16 && lt.wHour <= 21)
    {
        char s_warning[s_warning_size] = "Good evening!";
        int sendResult = sendto(udpSocket, s_warning, strlen(s_warning), 0, (SOCKADDR*)&send_addr, sizeof(send_addr));
    }
    else 
    {
        char s_warning[s_warning_size] = "Good night!";
        int sendResult = sendto(udpSocket, s_warning, strlen(s_warning), 0, (SOCKADDR*)&send_addr, sizeof(send_addr));
    }


    while (1)
    {
        const size_t receiveBufSize = 1024;
        char receiveBuf[receiveBufSize];

        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);

        cout << "Receiving data...." << endl;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        receiveBuf[bytesReceived] = '\0';
        cout << "Received from " << int(senderAddr.sin_addr.S_un.S_un_b.s_b1) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b2) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b3) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b4) << '.' << endl;
        cout << "Data: " << receiveBuf << endl;


        const size_t sendBufSize = 1024;
        //char sendBuf[sendBufSize] = "Hello from UDPExample";
        char sendBuf[sendBufSize];
        cin.getline(sendBuf, sendBufSize);

        int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        if (strcmp(receiveBuf, "bye") == 0)
        {
            break;
            closesocket(udpSocket);
            WSACleanup();
        }
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}

