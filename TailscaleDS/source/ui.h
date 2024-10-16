#ifndef UI_H
#define UI_H

#include <3ds/types.h>

void initUI();
void displayMessage(const char* message, float progress);
void waitForKeyPress(u32 key);
void cleanupUI();

#endif