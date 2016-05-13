#pragma once
#include "Globals.h"

bool loadConsoleFrame(char *FileName);
void saveConsoleToFile(char * FileName);
void readStringFromConsole(int CrnStr, ConsoleColor a, ConsoleColor b);
void WindowFrame(short top, short left, short bottom, short right, ConsoleColor background);
void showWindow(CHAR_INFO ** chiBuffer, short top, short left, short bottom, short right, ConsoleColor background);
void hideWindow(CHAR_INFO * chiBuffer, short top, short left, short bottom, short right);
void SetBufferSize();
void getFramePath(const char * argv[]);
bool renameWindow(char * FileName);
void showError(char * buffer1, char * buffer2);
void selectDisk(int CrntStr, bool select);
void listDisk(bool * Disk);
bool chooseDisk(bool * Disk);
void HelpWindow(char *FileHelp);
void ConsoleFrame();
void txtRead(char * FileName, _fsize_t FileSize);