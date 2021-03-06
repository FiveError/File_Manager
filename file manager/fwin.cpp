#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include "Globals.h"
#include "fwin.h"
#pragma pack(1)

using namespace std;


void EnableCursor(bool mode)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = mode;
	CCI.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);
}
void SetColor(ConsoleColor a, ConsoleColor b)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((a << 4) | b));
}
void SetCursorPosition(short x, short y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x,y };
	SetConsoleCursorPosition(hConsole, position);
}
void readBlockDown(ConsoleColor ColorLastStr)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*������ �� �������*/
	BOOL fSuccess;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;
	coordBufSize.Y = ConsoleSize.Y - 6;
	coordBufSize.X = ConsoleSize.X - 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	for (int i = 0; i < (ConsoleSize.X); ++i) chiBuffer[i].Attributes = (WORD)((ColorLastStr << 4) | White);
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void readBlockUp(ConsoleColor ColorLastStr)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*������ �� �������*/
	BOOL fSuccess;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;
	coordBufSize.Y = ConsoleSize.Y - 6;
	coordBufSize.X = ConsoleSize.X - 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[size - i - 1].Attributes = (WORD)((ColorLastStr << 4) | White);
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}

bool deleteBlockUp(int CrntStr)
{
	if (CrntStr == 0) return 0;
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6 - CrntStr)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*������ �� �������*/
	BOOL fSuccess;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 3 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;
	coordBufSize.Y = ConsoleSize.Y - 6 - CrntStr;
	coordBufSize.X = ConsoleSize.X - 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	srctReadRect.Top = 2 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
	return 1;
}

void addBlockDown(int y)
{
	if (y == ConsoleSize.Y - 3) return;
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 4 - y)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*������ �� �������*/
	BOOL fSuccess;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = y;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;
	coordBufSize.Y = ConsoleSize.Y - 4 - y;
	coordBufSize.X = ConsoleSize.X - 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	srctReadRect.Top = y + 1;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void addBlockUp(int y)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (y - 3)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*������ �� �������*/
	BOOL fSuccess;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = y; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;
	coordBufSize.Y = y - 3;
	coordBufSize.X = ConsoleSize.X - 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Bottom = y - 1; // bot. right: row 2, col 120 
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
