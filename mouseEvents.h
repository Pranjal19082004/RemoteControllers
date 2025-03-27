// mouseEvents.h
#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include <windows.h>
void mouseLeftClick(UINT16 x, UINT16 y);
void mouseRightClick(UINT16 x, UINT16 y);
void rightDoubleClick(UINT16 x, UINT16 y);
void leftDoubleClick(UINT16 x, UINT16 y);
void holdLeft(UINT16 x, UINT16 y);
void ReleaseLeft(UINT16 x, UINT16 y);
void holdRight(UINT16 x, UINT16 y);
void ReleaseRight(UINT16 x, UINT16 y);
void moveCursorSmooth(UINT16 x, UINT16 y);
#endif
