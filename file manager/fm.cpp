#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include <chrono>
#include <ctime>
#include <thread>
#include "Globals.h"
#include "fm.h"
#include "fwin.h"
#include "ffiles.h"

using namespace std;

bool loadConsoleFrame(char *FileName)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 0;    // top left: row 2, col 1 
	srctReadRect.Left = 0;
	srctReadRect.Bottom = ConsoleSize.Y; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X;
	coordBufSize.Y = ConsoleSize.Y;
	coordBufSize.X = ConsoleSize.X;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	FILE *fLoad;
	fLoad = fopen(FileName, "r+b");
	if (!fLoad) return 0;
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X*ConsoleSize.Y];
	fread(chiBuffer, ConsoleSize.X*ConsoleSize.Y, sizeof(CHAR_INFO), fLoad);
	fclose(fLoad);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
	return 1;
}
void ConsoleFrame()
{

	SetCursorPosition(0, 0);
	setlocale(LC_CTYPE, "C");
	SetColor(Blue, White);
	ConsoleSize.X -= 2;
	printf("%c", 201);
	for (int i = 0; i < (5 * ConsoleSize.X / 6 - 1); ++i)
		printf("%c", 205);
	printf("%c", 209);
	for (int i = 0; i < (ConsoleSize.X / 6); ++i)
		printf("%c", 205);
	printf("%c%c", 187, 186);
	SetColor(Blue, Yellow);
	printf("Name");
	SetColor(Blue, White);
	for (int i = 0; i < (5 * ConsoleSize.X / 6 - 5); ++i)
		printf(" ");
	printf("%c", 179);
	SetColor(Blue, Yellow);
	printf("Size");
	SetColor(Blue, White);
	for (int i = 0; i < (ConsoleSize.X / 6 - 4); ++i)
		printf(" ");
	printf("%c", 186);
	ConsoleSize.Y -= 5;
	clearStr[5 * ConsoleSize.X / 6 - 1] = 179;
	for (int i = 0; i < ConsoleSize.Y; ++i)
	{
		printf("%c", 186);
		for (int j = 0; j < (5 * ConsoleSize.X / 6 - 1); ++j)
			printf(" ");
		printf("%c", 179);
		for (int j = 0; j < (ConsoleSize.X / 6); ++j)
			printf(" ");
		//for (int j = 0; j < ConsoleSize.X; ++j)
		//	printf(" ");
		
		printf("%c", 186);
	}
	clearStr[5 * ConsoleSize.X / 6 - 1] = 166;
	printf("%c", 200);
	for (int i = 0; i < (5 * ConsoleSize.X / 6 - 1); ++i)
		printf("%c", 205);
	printf("%c", 207);
	for (int i = 0; i < (ConsoleSize.X / 6); ++i)
		printf("%c", 205);
	printf("%c", 188);
	ConsoleSize.X += 2;
	ConsoleSize.Y += 5;
	printf("\n");
	SetColor(Red, White);
	printf("F1-HELP");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F2-RENAME");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F3-COPY");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F4-PASTE");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F5-NEWFOLDER");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F6-CHANGEDISK");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F7-ARCH");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F8-UNARCH");
	if (ConsoleSize.X == 80) return;
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F9-DELETE");
	setlocale(LC_CTYPE, "RUS");
}
void txtRead(char * FileName, _fsize_t FileSize)
{
	FILE *fTxt;
	fTxt = fopen(FileName, "r+b");
	if (!fTxt) {
		showError("Ошибка открытия файла", "");
		return;
	}
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X * (ConsoleSize.Y - 2)];
	short top = 0;
	short bottom = ConsoleSize.Y - 2;
	short left = 0;
	short right = ConsoleSize.X;
	showWindow(&chiBuffer, top, left, bottom, right, Green);
	int length;
	for (length = 0; FileName[length]; length++);
	SetColor(Green, Yellow);
	if (length > ConsoleSize.X - 2)
	{
		SetCursorPosition(1, 1);
		printf("%.*s..", ConsoleSize.X - 4, FileName);
	}
	else
	{
		SetCursorPosition((ConsoleSize.X - length) / 2, 1);
		printf("%s", FileName);
	}
	unsigned char * text = new unsigned char[ConsoleSize.X - 2];
	char *clrStr = new char[ConsoleSize.X - 1];
	for (int i = 0; i < ConsoleSize.X - 2; ++i)
		clrStr[i] = ' ';
	clrStr[ConsoleSize.X - 2] = '\0';
	int downPos = 0, tmp,upPos = 0;
	bool nextStr;
	SetColor(Green, White);
	for (int i = 0; i < ConsoleSize.Y - 5; ++i)
	{
		nextStr = false;
		SetCursorPosition(1, 2 + i);
		fseek(fTxt, downPos, SEEK_SET);
		fread(text, ConsoleSize.X - 2, sizeof(char), fTxt);
		for (int j = 0; j < ConsoleSize.X - 2; ++j)
		{
			if (text[j] == '\n')
			{
				nextStr = true;
				tmp = j;
				break;
			}
			else
				if (((text[j] >= 0x00) && (text[j] <= 0x0f)) || (text[j] == 0x95))
					printf(".");
				else printf("%c", text[j]);
		}
		if (nextStr) downPos += tmp + 1;
		else downPos += ConsoleSize.X - 2;
	}
	int key;
	do
	{
		key = _getch();
		if (key == 224)
			key = _getch();
		switch (key)
		{
		case 80:
			readBlockUp(Green);
			SetCursorPosition(1, ConsoleSize.Y - 4);
			printf("%s", clrStr);
			SetCursorPosition(1, ConsoleSize.Y - 4);
			fseek(fTxt, downPos, SEEK_SET);
			fread(text, ConsoleSize.X - 2, sizeof(char), fTxt);
			for (int j = 0; j < ConsoleSize.X - 2; ++j)
			{
				if (text[j] == '\n')
				{
					nextStr = true;
					tmp = j;
					break;
				}
				else
					if (((text[j] >= 0x00) && (text[j] <= 0x0f)) || (text[j] == 0x95)) printf(".");
					else printf("%c", text[j]);
			}
			if (nextStr) downPos += tmp + 1;
			else downPos += ConsoleSize.X - 2;
			break;
		case 72:
			readBlockDown(Green);
			SetCursorPosition(1, 2);
			fseek(fTxt, downPos, SEEK_SET);
			fread(text, ConsoleSize.X - 2, sizeof(char), fTxt);	
			for (int j = 0; j < ConsoleSize.X - 2; ++j)
			{
				if (text[j] == '\n')
				{
					nextStr = true;
					tmp = j;
					break;
				}
				else
					if (((text[j] >= 0x00) && (text[j] <= 0x0f)) || (text[j] == 0x95)) printf(".");
					else printf("%c", text[j]);
			}
			if (nextStr) downPos += tmp + 1;
			else downPos += ConsoleSize.X - 2;
			break;
		default:
			break;
		}
	} while (key != 27);


	hideWindow(chiBuffer, top, left, bottom, right);
	fclose(fTxt);
	delete[] chiBuffer;
	delete[] text;
	delete[] clrStr;
}
void saveConsoleToFile(char *FileName)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X*ConsoleSize.Y]; // [1][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	BOOL fSuccess;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 0;    // top left: row 2, col 1 
	srctReadRect.Left = 0;
	srctReadRect.Bottom = ConsoleSize.Y; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X;
	coordBufSize.Y = ConsoleSize.Y;
	coordBufSize.X = ConsoleSize.X;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	FILE *fSave;
	fSave = fopen(FileName, "w+b");
	fwrite(chiBuffer, ConsoleSize.X*ConsoleSize.Y, sizeof(CHAR_INFO), fSave);
	fclose(fSave);
	delete[] chiBuffer;
}
void readStringFromConsole(int CrnStr, ConsoleColor a, ConsoleColor b)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X - 2]; // [1][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	BOOL fSuccess;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = CrnStr + 2;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = CrnStr + 2; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;

	// The temporary buffer size is 1 rows x 120 columns.

	coordBufSize.Y = 1;
	coordBufSize.X = ConsoleSize.X - 2;

	// The top left destination cell of the temporary buffer is 
	// row 0, col 0. 

	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 

	for (int i = 0; i < (ConsoleSize.X - 2); ++i) chiBuffer[i].Attributes = (WORD)((a << 4) | b);
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void WindowFrame(short top, short left, short bottom, short right, ConsoleColor background)
{
	setlocale(LC_CTYPE, "C");
	SetColor(background, White);
	SetCursorPosition(left, top);
	printf("%c", 201);
	for (int i = 0; i < (right - left - 2); ++i)
		printf("%c", 205);
	printf("%c", 187);
	char *str = new char[right - left];
	sprintf(str,"%c", 186);
	for (int i = 0; i < (right - left - 2); ++i)
		sprintf(str + i + 1, " ");
	sprintf(str+right-left-1,"%c", 186);
	for (short j = 0; j < (bottom - top - 2); ++j)
	{
		SetCursorPosition(left, top + j + 1);
		printf("%s", str);
	}
	SetCursorPosition(left, bottom - 1);
	printf("%c", 200);
	for (int i = 0; i < (right - left - 2); ++i)
		printf("%c", 205);
	printf("%c", 188);
	setlocale(LC_CTYPE, "rus");
}
void showWindow(CHAR_INFO **chiBuffer, short top, short left, short bottom, short right, ConsoleColor background)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordBufSize;
	coordBufSize.Y = bottom - top;
	coordBufSize.X = right - left;
	SMALL_RECT srctReadRect;
	srctReadRect.Bottom = bottom;
	srctReadRect.Left = left;
	srctReadRect.Right = right;
	srctReadRect.Top = top;
	COORD coordBufCoord;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	BOOL fSuccess;
	fSuccess = ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		*chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	WindowFrame(top, left, bottom, right, background);
}
void hideWindow(CHAR_INFO *chiBuffer, short top, short left, short bottom, short right)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordBufSize;
	coordBufSize.Y = bottom - top;
	coordBufSize.X = right - left;
	SMALL_RECT srctReadRect;
	srctReadRect.Bottom = bottom;
	srctReadRect.Left = left;
	srctReadRect.Right = right;
	srctReadRect.Top = top;
	COORD coordBufCoord;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	BOOL fSuccess;
	fSuccess = WriteConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
}
void SetBufferSize()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT src = { 0, 0, ConsoleSize.X - 1, ConsoleSize.Y - 1 };
	SetConsoleScreenBufferSize(hConsole, ConsoleSize);
	SetConsoleWindowInfo(hConsole, TRUE, &src);
}
void getFramePath(const char *argv[])
{
	int i;
	for (i = 0; argv[0][i]; ++i);
	i -= 4;
	frameFile = new char[i + 20];
	memcpy(frameFile, argv[0], i - 12);
	sprintf(frameFile + i - 12, "Settings\\ConsoleFrame%03dx%d.txt", ConsoleSize.X, ConsoleSize.Y);
}
bool renameWindow(char *FileName)
{
	CHAR_INFO *chiBuffer = new CHAR_INFO[80 * 25];
	short top = ConsoleSize.Y / 2 - 2;
	short bottom = ConsoleSize.Y / 2 + 2;
	short left = ConsoleSize.X / 2 - 40;
	short right = ConsoleSize.X / 2 + 40;
	showWindow(&chiBuffer, top, left, bottom, right, DarkGray);
	SetCursorPosition(left + 1, top + 1);
	printf("Введите новое имя:");
	SetCursorPosition(left + 1, top + 2);
	SetColor(Black, White);
	printf("                                                                              ");
	char NewName[260];
	for (int i = 0; i < 260; i++)
		NewName[i] = '\0';
	int i = 0;
	for (; FileName[i]; i++)
		NewName[i] = FileName[i];
	SetCursorPosition(left + 1, top + 2);
	EnableCursor(true);
	int key;
	char temp;
	unsigned lchar = 0;
	if (i < 78) printf("%s", NewName);
	else {
		printf("%s", NewName + i - 77);
		lchar = i - 77;
		SetCursorPosition(right - 2, top + 2);
	}
	do
	{
		key = _getch();
		if ((key == 224) && (_kbhit()))
		{
			key = _getch();
			switch (key)
			{
			case 75:
				if (i)
				{
					if (lchar != i) i--;
					else
					{
						for (i--; (i - lchar < 77) || (i - lchar > 240); i++)
							printf("%c", NewName[i]);
						i -= 78;
						lchar--;
					}
				}
				break;
			case 77:
				if (NewName[i])
				{
					if (i - lchar < 77) i++;
					else
					{
						SetCursorPosition(left + 1, top + 2);
						for (i -= 76; i - lchar < 78; i++)
							printf("%c", NewName[i]);
						printf("%c", NewName[i]);
						lchar++;
					}
				}
				break;
			default:
				break;
			}

			SetCursorPosition(left + 1 + i - lchar, top + 2);
		}
		else
			if (((key >= 32) && (key <= 126) && (key != 34) && (key != 47) && (key != 58) && (key != 60) && (key != 62) && (key != 63) && (key != 42) && (key != 92) && (key != 124)) ||
				((key >= 128) && (key <= 175)) || ((key >= 224) && (key <= 241)))
			{
				if ((key >= 224) && (key <= 241)) key += 16;
				if ((key >= 128) && (key <= 175)) key += 64;
				if ((i < 77) && (!lchar))
					if (NewName[i])
					{
						for (int j = i; NewName[j - 1]; j++)
						{
							temp = NewName[j];
							if (j < 78)
								printf("%c", key);
							NewName[j] = key;
							key = temp;
						}
						i++;
						SetCursorPosition(left + 1 + i, top + 2);
					}
					else
					{
						printf("%c", key);
						NewName[i++] = key;
					}
				else
					if (NewName[i])
					{
						if (!NewName[259])
						{
							SetCursorPosition(left + 1, top + 2);
							lchar++;
							for (int j = lchar; j < i; j++) printf("%c", NewName[j]);
							printf("%c", key);
							for (int j = i; NewName[j - 1]; j++)
							{
								temp = NewName[j];
								NewName[j] = key;
								key = temp;
							}
							i++;
							SetCursorPosition(left + 1 + i - lchar, top + 2);
						}
						else
						{
							showError("Длина имени не может привышать 260 символов", "");
							SetColor(Black, White);
						}
					}
					else
					{
						if (!NewName[259])
						{
							SetCursorPosition(left + 1, top + 2);
							for (i -= 76; NewName[i]; i++)
								printf("%c", NewName[i]);
							printf("%c", key);
							NewName[i++] = key;
							lchar++;
						}
						else
						{
							showError("Длина имени не может привышать 260 символов", "");
							SetColor(Black, White);
						}
					}
			}
		if ((key == 8) && (i != 0))
		{
			if (!lchar)
				if (NewName[i])
				{
					i--;
					SetCursorPosition(left + 1 + i, top + 2);
					for (int j = i; NewName[j]; j++)
					{
						NewName[j] = NewName[j + 1];
						if (j < 78)
							printf("%c", NewName[j]);
					}
					SetCursorPosition(left + 1 + i, top + 2);
				}
				else
				{
					SetCursorPosition(left + i, top + 2);
					printf(" ");
					NewName[--i] = '\0';
					SetCursorPosition(left + 1 + i, top + 2);
				}
			else
				if (NewName[i])
				{
					SetCursorPosition(left + 1, top + 2);
					i--;
					lchar--;
					for (int j = lchar; j < i; j++) printf("%c", NewName[j]);
					for (int j = i; NewName[j]; j++) NewName[j] = NewName[j + 1];
					NewName[259] = '\0';
					SetCursorPosition(left + 1 + i - lchar, top + 2);
				}
				else
				{
					SetCursorPosition(left + 1, top + 2);
					for (i -= 78; NewName[i + 1]; i++)
						printf("%c", NewName[i]);
					NewName[i] = '\0';
					lchar--;
				}
		}
		if (key == 13)
		{
			if (!rename(FileName, NewName)) {
				addLog(FileName, "RENAME", NewName);
				for (i = 0; NewName[i]; i++)
					FileName[i] = NewName[i];
				FileName[i] = NewName[i];
				EnableCursor(false);
				hideWindow(chiBuffer, top, left, bottom, right);
				delete[] chiBuffer;
				return 1;
			}
			else {
				showError("Файл с таким именем уже существует", "");
				SetColor(Black, White);
				SetCursorPosition(left + 1 + i - lchar, top + 2);
			}
		}
	} while (key != 27);
	EnableCursor(false);
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
	return 0;
}
void showError(char *buffer1, char *buffer2)
{
	short top = ConsoleSize.Y / 2 - 4;
	short bottom = ConsoleSize.Y / 2 + 3;
	short left = ConsoleSize.X / 2 - 31;
	short right = ConsoleSize.X / 2 + 31;
	CHAR_INFO *chiBuffer = new CHAR_INFO[7 * 62];
	showWindow(&chiBuffer, top, left, bottom, right, Red);
	SetCursorPosition(left + 28, top + 1);
	printf("ERROR!");
	SetCursorPosition(left + 2, top + 2);
	printf("%s", buffer1);
	SetCursorPosition(left + 2, top + 3);
	printf("%s", buffer2);
	SetCursorPosition(left + 2, top + 5);
	printf("Нажмите на любую клавишу...");
	_getch();
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}
void selectDisk(int CrntStr, bool select)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO *chiBuffer = new CHAR_INFO[8]; // [1][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = ConsoleSize.Y / 2 - 13 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 4;
	srctReadRect.Bottom = ConsoleSize.Y / 2 - 13 + CrntStr; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 4;
	coordBufSize.Y = 1;
	coordBufSize.X = 8;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	if (select)
		for (int i = 0; i < 8; i++) chiBuffer[i].Attributes = (WORD)((LightBlue << 4) | White);
	else
		for (int i = 0; i < 8; i++) chiBuffer[i].Attributes = (WORD)((Cyan << 4) | Yellow);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void listDisk(bool *Disk)
{

	int n;
	DWORD dr = GetLogicalDrives();
	for (int x = 0; x < 26; x++)
	{
		n = ((dr >> x) & 1);
		if (n)
			*(Disk + x) = true;
		else
			*(Disk + x) = false;
	}

}
bool chooseDisk(bool *Disk)
{
	int j = 0, CrntStr = 0, kolve = 0, key = 0, podkl = 0;
	CHAR_INFO *chiBuffer = new CHAR_INFO[10 * 29];
	short top = ConsoleSize.Y / 2 - 15;
	short bottom = ConsoleSize.Y / 2 + 14;
	short left = ConsoleSize.X / 2 - 5;
	short right = ConsoleSize.X / 2 + 5;
	showWindow(&chiBuffer, top, left, bottom, right, Cyan);
	SetCursorPosition(left + 3, top + 1);
	SetColor(Cyan, Yellow);
	printf("Диск");
	for (int i = 0; i < 26; ++i)
	{
		if (Disk[i])
		{
			SetCursorPosition(left + 1, top + 2 + j);
			printf("%C:\\", (65 + i));
			++j;
			++kolve;
		}
	}
	selectDisk(CrntStr, true);
	do
	{
		key = _getch();
		if (key == 224)
		{
			key = _getch();
			switch (key)
			{
			case 80:
				if (CrntStr < (kolve - 1))
				{
					selectDisk(CrntStr, false);
					CrntStr++;
					selectDisk(CrntStr, true);
				}
				break;
			case 72:
				if (CrntStr)
				{
					selectDisk(CrntStr, false);
					CrntStr--;
					selectDisk(CrntStr, true);
				}
				break;
			default:
				break;
			}
		}

		if (key == 13)
		{
			for (int i = 0; i < 26; ++i)
			{
				if (Disk[i])
					++podkl;
				if ((CrntStr + 1) == podkl)
				{
					char path[5] = "C:\\";
					*path = 65 + i;
					if (_chdir(path) == -1)
					{
						showError("Не получается подключиться к данному диску", "");
						break;
					}
					hideWindow(chiBuffer, top, left, bottom, right);
					delete[] chiBuffer;
					return true;
				}
			}
		}
	} while (key != 27);

	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
	return false;
}