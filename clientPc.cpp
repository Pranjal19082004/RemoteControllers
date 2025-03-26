#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "mouseEvents.h"
#define PORT "3490"
#define MAXDATASIZE 100
#define BACKLOG 10

using namespace std;
SOCKET find_server(char *NODENAME);
void HandleEvent(int eventNumber, int16_t x, int16_t y);

int main(int argc, char *argv[])
{

    WSADATA d;
    WSAStartup(MAKEWORD(2, 2), &d);
    SOCKET cfd;
    if ((cfd = find_server(argv[1])) == INVALID_SOCKET)
    {
        WSACleanup();
        cout << "findServer()" << endl;
        return 1;
    }
    cout << "Connected " << "\n";
    char *messageBuffer = new char[10];

    while (true)
    {
        long long n = 0;
        n = recv(cfd, messageBuffer, 5, 0);
        if (n == 0)
        {
            cout << "done" << endl;
            break;
        }

        int eventType = static_cast<int>(messageBuffer[0]);
        int16_t x1 = (static_cast<unsigned char>(messageBuffer[1]));
        int16_t x2 = (static_cast<unsigned char>(messageBuffer[2]));
        int16_t x = (x1 | (x2 << 8));

        int16_t y1 = (static_cast<unsigned char>(messageBuffer[3]));
        int16_t y2 = static_cast<unsigned char>(messageBuffer[4]);
        int16_t y = (y1 | (y2 << 8));
        cout << x << " " << y << endl;
        // cout<< messageBuffer[1] |

        HandleEvent(eventType, x, y);
    }
    closesocket(cfd);
    WSACleanup();
    return 0;
}
SOCKET find_server(char *NODENAME)
{
    SOCKET clientFd;
    struct addrinfo hints, *clientAddrInfoLinkedList, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    if ((getaddrinfo(NODENAME, PORT, &hints, &clientAddrInfoLinkedList)) != 0)
    {
        perror("getaddrinfo()\n");
        return INVALID_SOCKET;
    }
    for (p = clientAddrInfoLinkedList; p != NULL; p = p->ai_next)
    {
        if ((clientFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET)
        {
            perror("socket()\n");
            continue;
        }
        if ((connect(clientFd, p->ai_addr, p->ai_addrlen)) == -1)
        {
            perror("connect()\n");
            continue;
        }
        break;
    }
    freeaddrinfo(clientAddrInfoLinkedList);
    if (p == NULL)
    {
        perror("cant connect to server : \n");
        return INVALID_SOCKET;
    }
    return clientFd;
}
void HandleEvent(int eventNumber, int16_t x, int16_t y)
{

    switch (eventNumber)
    {
    case 1:
        moveCursorSmooth(x, y);
        break;
    case 2:
        holdLeft(x, y);
        cout << "left button down" << endl;
        break;
    case 3:
        ReleaseLeft(x, y);
        cout<< "left button up" << endl;
        break;
    case 4:
        cout << "left button double click" << endl;
        break;
    case 5:
        cout << "right button down " << endl;
        break;
    case 6:
        cout << "right button up" << endl;
        break;
    case 7:
        cout << "right button double clicked" << endl;
        break;
    default:
        break;
    }
    return;
}