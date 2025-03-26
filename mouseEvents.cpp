#include "mouseEvents.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>

void mouseLeftClick(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void mouseRightClick(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void rightDoubleClick(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT inputArray[4] = {0};

    inputArray[0].type = INPUT_MOUSE;
    inputArray[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    inputArray[1].type = INPUT_MOUSE;
    inputArray[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    inputArray[2].type = INPUT_MOUSE;
    inputArray[2].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    inputArray[3].type = INPUT_MOUSE;
    inputArray[3].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    SendInput(4, inputArray, sizeof(INPUT));
}

void holdLeft(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}

void ReleaseLeft(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void holdRight(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}

void ReleaseRight(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void moveCursorSmooth(UINT16 x, UINT16 y)
{
    SetCursorPos(x, y);
    // INPUT input = {0};
    // input.type = INPUT_MOUSE;
    // // input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    // // input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN);
    // input.mi.dx = x;
    // input.mi.dy = y;
    // std::cout << "move to :" << input.mi.dx << " " << input.mi.dy << std::endl;

    // input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    // SendInput(1, &input, sizeof(INPUT));
}
