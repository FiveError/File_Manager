#pragma once;
#include "Globals.h"

void EnableCursor(bool mode);
void SetColor(ConsoleColor a, ConsoleColor b);
void SetCursorPosition(short x, short y);
void readBlockDown(ConsoleColor ColorLastStr);
void readBlockUp(ConsoleColor ColorLastStr);
bool deleteBlockUp(int CrntStr);
void addBlockDown(int y);
void addBlockUp(int y);