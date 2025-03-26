#include <stdio.h>
#include <iostream>
#include <winsock.h>
#include <windows.h>
using namespace std;
void moveCursorSmooth(int dx, int dy)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}
void mouseEvent(int eventNumber)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    switch (eventNumber)
    {

    case 1:
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input, sizeof(INPUT));
        break;

    case 2:
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1, &input, sizeof(INPUT));
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1, &input, sizeof(INPUT));
        break;
    }
}
int main()
{
    while (true)
    {
        cout << "can you write co-ordinate of the cursor:" << endl;
        int x, y;
        cin >> x >> y;
        moveCursorSmooth(x, y);
        cout << "enter [1] :" << endl
             << "to click left ." << endl
             << "enter [2]:\n to click right" << endl
             << "enter [0]:\n to end the program" << endl;
        int mouseClickNumber = 0;
        cin >> mouseClickNumber;
        if (mouseClickNumber ==100)
        {
            break;
        }
        else
        {
            mouseEvent(mouseClickNumber);
        }
    }
}