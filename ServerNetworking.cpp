#define WIN32_LEAN_AND_MEAN
#include "ServerNetworking.h"
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
#define PORT "3490"
#define BACKLOG 10
SOCKET find_a_listener()
{
    SOCKET serverFd;
    struct addrinfo hints, *ServerAddrInfoLinkList, *p;
    int yes = 1;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    if ((getaddrinfo(NULL, PORT, &hints, &ServerAddrInfoLinkList)) != 0)
    {
        perror("getaddrinfo()\n");
        return INVALID_SOCKET;
    }
    for (p = ServerAddrInfoLinkList; p != NULL; p = p->ai_next)
    {
        if ((serverFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET)
        {
            perror("socket()\n");
            continue;
        }
        if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, (char *)(&yes), sizeof(int)) == SOCKET_ERROR)
        {
            perror("setsockopt\n");
            closesocket(serverFd);
            continue;
        }
        if ((bind(serverFd, p->ai_addr, p->ai_addrlen)) == SOCKET_ERROR)
        {
            closesocket(serverFd);
            perror("bind()\n");
            continue;
        }
        if (listen(serverFd, BACKLOG) == SOCKET_ERROR)
        {
            closesocket(serverFd);
            perror("lsiten()");
            continue;
        }
        break;
    }
    freeaddrinfo(ServerAddrInfoLinkList);
    if (p == NULL)
    {
        perror("cant find a listener on which we can listen : \n");
        return INVALID_SOCKET;
    }
    return serverFd;
}