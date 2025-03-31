#include <stdio.h>
#include <iostream>
#include <fstream>
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#include <ws2tcpip.h>
#include "mouseEvents.h"
#define PORT "3490"
#define MAXDATASIZE 100
#define BACKLOG 10

using namespace std;
SOCKET find_server(char *NODENAME);
void HandleEventForMouse(int eventNumber, int16_t x, int16_t y, int16_t width, int16_t height, int16_t clientWidth, int16_t clientHeight);

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
	cout << sizeof(WPARAM) << endl;

	while (true)
	{
		long long n = 0;
		n = recv(cfd, messageBuffer, 10, 0);
		if (n == 0)
		{
			cout << "done" << endl;
			break;
		}

		int eventType = static_cast<int>(messageBuffer[0]);
		if (eventType == 8)
		{
			INPUT input = {0};
			UINT16 wvk1 = static_cast<unsigned char>(messageBuffer[1]);
			UINT8 wvk2 = static_cast<unsigned char>(messageBuffer[2]);
			UINT16 wvk = ((wvk1) | (wvk2 << 8));
			UINT8 flag = static_cast<unsigned char>(messageBuffer[3]);
			if (flag == 1)
			{
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = wvk;
				input.ki.wScan = MapVirtualKey(wvk, MAPVK_VK_TO_VSC);
				input.ki.dwFlags = KEYEVENTF_SCANCODE;
				if (wvk >= VK_LEFT && wvk <= VK_DOWN)
				{ // Handle extended keys
					input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
				}
				input.ki.dwFlags |= KEYEVENTF_KEYUP;
				// Press key
				SendInput(1, &input, sizeof(INPUT));
				continue;
			}
			iput.type = INPUT_KEYBOARD;
			input.ki.wVk = wvk;
			inut.ki.wScan = MapVirtualKey(wvk, MAPVK_VK_TO_VSC);
			input.ki.dwFlags = KEYEVENTF_SCANCODE;

			if (wvk >= VK_LEFT && wvk <= VK_DOWN)
			{ // Handle extended keys
				input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
			}
			SendInput(1, &input, sizeof(INPUT));

			if (flag == 0)
			{
				cout << (char)(wvk) << endl;
			}
			continue;
		}
		int16_t x1 = (static_cast<unsigned char>(messageBuffer[1]));
		int16_t x2 = (static_cast<unsigned char>(messageBuffer[2]));
		int16_t x = (x1 | (x2 << 8));
		int16_t y1 = (static_cast<unsigned char>(messageBuffer[3]));
		int16_t y2 = static_cast<unsigned char>(messageBuffer[4]);
		int16_t y = (y1 | (y2 << 8));
		int16_t width1 = static_cast<unsigned char>(messageBuffer[5]);
		int16_t width2 = static_cast<unsigned char>(messageBuffer[6]);
		int16_t clientWidth = (width1 | (width2 << 8));
		int16_t height1 = static_cast<unsigned char>(messageBuffer[7]);
		int16_t height2 = static_cast<unsigned char>(messageBuffer[8]);
		int16_t clientHeight = (height1 | (height2 << 8));
		cout << x << " " << y << endl;
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		HandleEventForMouse(eventType, x, y, width, height, clientWidth, clientHeight);
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
void HandleEventForMouse(int eventNumber, int16_t x, int16_t y, int16_t width, int16_t height, int16_t clientWidth, int16_t clientHeight)
{

	unsigned int xtemp = (x * width);
	xtemp = (unsigned int)(((double)(xtemp)) / clientWidth);
	int ytemp = (y * height);
	ytemp = (unsigned int)(((double)(ytemp)) / (clientHeight));
	x = static_cast<unsigned int16_t>(xtemp);
	y = static_cast<unsigned int16_t>(ytemp);
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
		cout << "left button up" << endl;
		break;
	case 4:
		leftDoubleClick(x, y);
		cout << "left button double click" << endl;
		break;
	case 5:
		holdRight(x, y);
		cout << "right button down " << endl;
		break;
	case 6:
		ReleaseRight(x, y);
		cout << "right button up" << endl;
		break;
	case 7:
		rightDoubleClick(x, y);
		cout << "right button double clicked" << endl;
		break;
	case 8:
		cout << "keyboard is pressed ";
		break;
	default:
		break;
	}
	return;
}