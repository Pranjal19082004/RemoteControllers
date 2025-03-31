#define WIN32_LEAN_AND_MEAN
#ifndef UNICODE
#define UNICODE
#endif
#include <fstream>
#include <iostream>
#include "ServerNetworking.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
using namespace std;
SOCKET nfd;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LogMessage(const std::string &message);
void sendKeyBoardEvents(HWND &hwnd, UINT &uMsg, WPARAM &wParam, LPARAM &lParam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Learn to Program Windows",
		WS_OVERLAPPEDWINDOW,

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,	   // Parent window
		NULL,	   // Menu
		hInstance, // Instance handle
		NULL	   // Additional application data
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Failed to create window!", L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	RECT rect;
	GetWindowRect(hwnd, &rect);
	ShowWindow(hwnd, nCmdShow);
	WSADATA d;
	if (WSAStartup(MAKEWORD(2, 2), &d) != 0)
	{
		MessageBox(NULL, L"WSAstartup", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	SOCKET sfd;
	struct sockaddr_storage theirAddr;
	int addrLen = INET6_ADDRSTRLEN;
	if ((sfd = find_a_listener()) == INVALID_SOCKET)
	{
		MessageBox(NULL, L"find_a_listener", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	};

	if ((nfd = accept(sfd, (sockaddr *)&theirAddr, &addrLen)) == INVALID_SOCKET)
	{
		MessageBox(NULL, L"accept", L"Error", MB_OK | MB_ICONERROR);
		closesocket(sfd);
		PostQuitMessage(0);
		WSACleanup();
		return 0;
	}
	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	WSACleanup();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char *messageToSend = new char[10];
	memset(messageToSend, 0, 10);
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	cout << "height" << height << " " << "width" << width << endl;
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		closesocket(nfd);

		PostQuitMessage(0);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		messageToSend[0] = 1;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		messageToSend[0] = 2;
		break;
	}
	case WM_LBUTTONUP:
	{
		messageToSend[0] = 3;
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		messageToSend[0] = 4;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		messageToSend[0] = 5;
		break;
	}
	case WM_RBUTTONUP:
	{
		messageToSend[0] = 6;
		break;
	}
	case WM_RBUTTONDBLCLK:
	{
		messageToSend[0] = 7;

		break;
	}
	case WM_KEYDOWN:
	{
		messageToSend[0] = 8;
		POINT pt;

		ClientToScreen(hwnd, &pt);

		messageToSend[1] = ((wParam) & (0xFF));
		messageToSend[2] = (((wParam) >> 8) & (0xff));
		messageToSend[3] = 0;
		string s = to_string(static_cast<UINT16>(wParam));
		LogMessage(s);
		send(nfd, messageToSend, 10, 0);
		return 0;
	}
	case WM_KEYUP:
	{
		messageToSend[0] = 8;
		POINT pt;

		ClientToScreen(hwnd, &pt);

		messageToSend[1] = ((wParam) & (0xFF));
		messageToSend[2] = (((wParam) >> 8) & (0xff));
		messageToSend[3] = 1;
		string s = "";
		s.push_back(static_cast<unsigned char>(static_cast<UINT16>(wParam)));
		LogMessage(s);
		send(nfd, messageToSend, 10, 0);
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	ClientToScreen(hwnd, &pt);
	messageToSend[1] = ((pt.x) & (0xFF));
	messageToSend[2] = (((pt.x) >> 8) & (0xff));
	messageToSend[3] = ((pt.y) & (0xFF));
	messageToSend[4] = (((pt.y) >> 8) & (0xff));
	messageToSend[5] = ((width) & (0xFF));
	messageToSend[6] = (((width) >> 8) & (0xff));
	messageToSend[7] = ((height) & (0xFF));
	messageToSend[8] = ((height) >> 8) & (0xff);
	send(nfd, messageToSend, 10, 0);

	return 0;
}
void LogMessage(const std::string &message)
{
	std::ofstream logFile("log.txt", std::ios::app); // Open file in append mode
	if (logFile.is_open())
	{
		logFile << message << std::endl;
		logFile.close();
	}
}
