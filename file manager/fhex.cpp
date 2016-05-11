#include "fhex.h"
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
#include "huffman.h"
#include "fm.h"
#include "fwin.h"
#include "Globals.h"
#include "ffiles.h"
unsigned int char2int_(char chr)
{
	if (chr >= '0' && chr <= '9')
		return chr - '0';
	else if (chr >= 'A' && chr <= 'F')
		return chr - 'A' + 10;
	else if (chr >= 'a' && chr <= 'f')
		return chr - 'a' + 10;
	return -1;
}

void selectHEX(int CrntStr, int CrntStl, bool select)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X - 2]; // [1][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = CrntStr + 2;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 28 + 3 * CrntStl;
	srctReadRect.Bottom = CrntStr + 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 26 + 3 * CrntStl;
	coordBufSize.Y = 1;
	coordBufSize.X = 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	if (select)
		for (int i = 0; i < 2; i++) chiBuffer[i].Attributes = (WORD)((White << 4) | Black);
	else
		for (int i = 0; i < 2; i++) chiBuffer[i].Attributes = (WORD)((Magenta << 4) | White);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
	chiBuffer = new CHAR_INFO[8];
	srctReadRect.Top = CrntStr + 2;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 38;
	srctReadRect.Bottom = CrntStr + 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 30;
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
		for (int i = 0; i < 8; i++) chiBuffer[i].Attributes = (WORD)((Magenta << 4) | White);
	else
		for (int i = 0; i < 8; i++) chiBuffer[i].Attributes = (WORD)((Magenta << 4) | Yellow);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
	chiBuffer = new CHAR_INFO[2];
	srctReadRect.Top = 1;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 28 + 3 * CrntStl;
	srctReadRect.Bottom = 2; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 26 + 3 * CrntStl;
	coordBufSize.Y = 1;
	coordBufSize.X = 2;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	if (select)
		for (int i = 0; i < 2; i++) chiBuffer[i].Attributes = (WORD)((Magenta << 4) | White);
	else
		for (int i = 0; i < 2; i++) chiBuffer[i].Attributes = (WORD)((Magenta << 4) | Yellow);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
	chiBuffer = new CHAR_INFO[1];
	srctReadRect.Top = CrntStr + 2;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 + 21 + CrntStl;
	srctReadRect.Bottom = CrntStr + 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 22 + CrntStl;
	coordBufSize.Y = 1;
	coordBufSize.X = 1;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 
	if (select)
		chiBuffer[0].Attributes = (WORD)((Yellow << 4) | Black);
	else
		chiBuffer[0].Attributes = (WORD)((Magenta << 4) | Yellow);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void readBlockDownHEX()
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6) * 78;
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 39;
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 39;
	coordBufSize.Y = ConsoleSize.Y - 6;
	coordBufSize.X = 78;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 

	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 

												  //for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[i].Attributes = (WORD)((Blue << 4) | White);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void readBlockUpHEX()
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6) * 78;
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 39;
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 39;
	coordBufSize.Y = ConsoleSize.Y - 6;
	coordBufSize.X = 78;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 

	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 

												  //for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[i].Attributes = (WORD)((Blue << 4) | White);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void showStrHEX(FILE * fHex, unsigned int adress, int CrntStr, int CharCount)
{
	fseek(fHex, adress * 16, SEEK_SET);
	unsigned char *ch = new unsigned char[CharCount];
	fread(ch, CharCount,sizeof(char), fHex);
	SetCursorPosition(ConsoleSize.X / 2 - 38,  2 + CrntStr);
	SetColor(Magenta, Yellow);
	printf("%08X  ", adress);
	SetColor(Magenta, White);
	for (int i = 0; i < CharCount; i++)
		printf("%02X ", ch[i]);
	SetCursorPosition(ConsoleSize.X / 2 + 21, 2 + CrntStr);
	SetColor(Magenta, Yellow);
	for (int i = 0; i < CharCount; i++)
		if (((ch[i] >= 0x00) && (ch[i] <= 0x0f)) || (ch[i] == 0x95)) printf(".");
		else
			printf("%c", ch[i]);
	delete[] ch;
	if (CharCount < 16)
	{
		SetCursorPosition(ConsoleSize.X / 2 - 28 + 3*CharCount, 2 + CrntStr);
		for (int i = CharCount; i < 16; i++) printf("   ");
		SetCursorPosition(ConsoleSize.X / 2 + 21 + CharCount, 2 + CrntStr);
		for (int i = CharCount; i < 16; i++) printf(" ");
	}
}
void showAllHEX(FILE * fHex, int adress)
{
	fseek(fHex, adress * 16, SEEK_SET);
	unsigned char *ch = new unsigned char[16 * (ConsoleSize.Y - 5)];
	fread(ch, 16 * (ConsoleSize.Y - 5), sizeof(char), fHex);
	for (int adress = 0; adress < (ConsoleSize.Y - 5); adress++)
	{
		SetCursorPosition(ConsoleSize.X / 2 - 38, 2 + adress);
		SetColor(Magenta, Yellow);
		printf("%08X  ", adress);
		SetColor(Magenta, White);
		for (int i = 0; i < 16; i++)
			printf("%02X ", ch[adress * 16 + i]);
		printf(" ");
		SetColor(Magenta, Yellow);
		for (int i = 0; i < 16; i++)
			if (((ch[adress * 16 + i] >= 0x00) && (ch[adress * 16 + i] <= 0x0f)) || (ch[adress * 16 + i] == 0x95)) printf(".");
			else
				printf("%c", ch[adress * 16 + i]);
	}
	delete[] ch;
}
void runHEX(char *FileName, _fsize_t FileSize)
{
	//preOption();
	FILE *fHex;
	fHex = fopen(FileName, "r+b");
	if (!fHex) {
		showError("Ошибка открытия файла", "");
		return;
	}
	CHAR_INFO *chiBuffer = new CHAR_INFO[80 * (ConsoleSize.Y-2)];
	short top = 0;
	short bottom = ConsoleSize.Y - 2;
	short left = ConsoleSize.X / 2 - 40;
	short right = ConsoleSize.X / 2 + 40;
	showWindow(&chiBuffer, top, left, bottom, right, Magenta);
	unsigned int lastAdress = FileSize / 16;
	int lastStl;
	if (FileSize % 16 == 15)
	{
		lastStl = 0;
		lastAdress++;
	}
	else lastStl = FileSize % 16 + 1;
	unsigned char c, *ch;
	bool secondChar = false;
	SetCursorPosition(left + 12, top + 1);
	SetColor(Magenta, Yellow);
	unsigned int adress;
	for (unsigned int i = 0; i < 16; i++) printf("%02X ", i);
	if (lastAdress > ConsoleSize.Y - 6) showAllHEX(fHex, 0);
	else
	{
		for (adress = 0; adress < lastAdress; adress++) showStrHEX(fHex, adress, adress, 16);
		showStrHEX(fHex, adress, adress, lastStl - 1);
	}
	adress = 0;
	int key, CrntStr = 0, CrntStl = 0;
	selectHEX(CrntStr, CrntStl, 1);
	do
	{
		key = _getch();
		if (key == 224)
		{
			key = _getch();
			switch (key)
			{
			case 80:
				if ((adress < lastAdress) && !((adress == lastAdress - 1) && (CrntStl >= lastStl)))
					if (CrntStr < ConsoleSize.Y-6)
					{
						selectHEX(CrntStr, CrntStl, 0);
						CrntStr++;
						adress++;
						selectHEX(CrntStr, CrntStl, 1);
					}
					else
					{
						selectHEX(CrntStr, CrntStl, 0);
						adress++;
						readBlockUpHEX();
						if (adress == lastAdress) showStrHEX(fHex, adress, CrntStr, lastStl - 1);
						else showStrHEX(fHex, adress,CrntStr, 16);
						selectHEX(CrntStr, CrntStl, 1);
					}
				break;
			case 72:
				if (adress)
					if (CrntStr)
					{
						selectHEX(CrntStr, CrntStl, 0);
						CrntStr--;
						adress--;
						selectHEX(CrntStr, CrntStl, 1);
					}
					else
					{
						selectHEX(CrntStr, CrntStl, 0);
						adress--;
						readBlockDownHEX();
						showStrHEX(fHex, adress, CrntStr, 16);
						selectHEX(CrntStr, CrntStl, 1);
					}
				break;
			case 75:
				if (CrntStl)
				{
					selectHEX(CrntStr, CrntStl, 0);
					CrntStl--;
					selectHEX(CrntStr, CrntStl, 1);
				}
				break;
			case 77:
				if ((CrntStl < 15) && !((adress >= lastAdress) && (CrntStl >= lastStl - 1)))
				{
					selectHEX(CrntStr, CrntStl, 0);
					CrntStl++;
					selectHEX(CrntStr, CrntStl, 1);
				}
				break;
			default:
				break;
			}
			secondChar = false;
		}
		if (key == 0)
		{
			key = _getch();
			switch (key)
			{
			case 59:
				searchHEX(fHex);
				break;
			default:
				break;
			}
		}
		if (((key >= '0') && (key <= '9')) || ((key >= 'a') && (key <= 'f')))
		{
			if ((key >= '0') && (key <= '9')) key -= 48;
			if ((key >= 'a') && (key <= 'f')) key -= 87;
			if (!secondChar)
			{
				fseek(fHex, adress * 16 + CrntStl, SEEK_SET);
				fread(&c, 1, sizeof(char), fHex);
				c = c % 16 + key * 16;
				fseek(fHex, adress * 16 + CrntStl, SEEK_SET);
				fwrite(&c, 1, sizeof(char), fHex);
				secondChar = true;
				SetColor(Yellow, Black);
				SetCursorPosition(left + 61 + CrntStl, top + 2 + CrntStr);
				if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
				else
					printf("%c", c);
				SetColor(Magenta, White);
				SetCursorPosition(left + 12 + CrntStl * 3, top + 2 + CrntStr);
				printf("%X", key);
			}
			else
			{
				printf("%X", key);
				c = c / 16 * 16 + key;
				fseek(fHex, adress * 16 + CrntStl, SEEK_SET);
				fwrite(&c, 1, sizeof(char), fHex);
				SetColor(Magenta, Yellow);
				SetCursorPosition(left + 61 + CrntStl, top + 2 + CrntStr);
				if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
				else
					printf("%c", c);
				secondChar = false;
				selectHEX(CrntStr, CrntStl, 0);
				if ((adress == lastAdress) && (CrntStl == lastStl - 1))
					if (lastStl < 16) lastStl++;
					else
					{
						lastStl = 1;
						lastAdress++;
					}
				if (CrntStl < 15) CrntStl++;
				else {
					CrntStl = 0;
					if (CrntStr < (ConsoleSize.Y - 6))
					{
						CrntStr++;
						adress++;
					}
					else
					{
						selectHEX(CrntStr, CrntStl, 0);
						adress++;
						readBlockUpHEX();
						fseek(fHex, adress * 16, SEEK_SET);
						SetCursorPosition(left + 2, bottom - 2);
						SetColor(Magenta, Yellow);
						printf("%08X", adress);
						for (int i = 0; i < 16; i++)
						{
							fread(&c, sizeof(char), 1, fHex);
							if (feof(fHex))
							{
								while (i < 16)
								{
									SetCursorPosition(left + 12 + i * 3, bottom - 2);
									printf("  ");
									SetCursorPosition(left + 61 + i, bottom - 2);
									printf(" ");
									i++;
								}
								break;
							}
							SetCursorPosition(left + 12 + i * 3, bottom - 2);
							SetColor(Magenta, White);
							printf("%02X", c);
							SetCursorPosition(left + 61 + i, bottom - 2);
							SetColor(Magenta, Yellow);
							if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
							else
								printf("%c", c);
						}
					}
				}
				selectHEX(CrntStr, CrntStl, 1);
			}
		}
	} while (key != 27);
	fclose(fHex);
	remove("BufferFile");
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}
void preOption()
{

}
void searchHEX(FILE * HexFile)
{
	EnableCursor(true);
	CHAR_INFO *chiBuffer = new CHAR_INFO[40 * 8];
	short top = ConsoleSize.Y / 2 - 4;
	short bottom = ConsoleSize.Y / 2 + 4;
	short left = ConsoleSize.X / 2 - 20;
	short right = ConsoleSize.X / 2 + 20;
	showWindow(&chiBuffer, top, left, bottom, right, DarkGray);
	SetCursorPosition(left + 1, top + 2);
	printf("S");
	SetCursorPosition(left + 3, top + 2);
	SetColor(Black, White);
	printf("                                   ");
	SetCursorPosition(left + 1, top + 4);
	SetColor(DarkGray, White);
	printf("H");
	SetCursorPosition(left + 3, top + 4);
	SetColor(Black, White);
	printf("                                   ");
	SetColor(DarkGray, White);
	SetCursorPosition(left + 4, top + 6);
	SetColor(White, DarkGray);
	printf(" Искать ");
	SetCursorPosition(right - 13, top + 6);
	SetColor(DarkGray, White);
	printf(" Отмена ");
	SetCursorPosition(left + 3, top + 2);
	SetColor(Black, White);
	bool bSearch = true, bString = true;
	char instring[260],htos[2];
	setlocale(LC_CTYPE, "RUS");
	int key, i = 0, tmp, j = 0;
	do
	{
		key = _getch();
		if (key == 224)
		{
			key = _getch();
			switch (key)
			{

			case 80:
				SetCursorPosition(left + 3 + 2 * i, top + 4);
				bString = false;
				break;
			case 72:
				SetCursorPosition(left + 3 + i, top + 2);
				bString = true;
			default:
				break;
			}
		}
		else
		{
			if (bString)
			{
				printf("%c", key);
				SetCursorPosition(left + 3 + 2 * i, top + 4);
				printf("%X", key);
				i++;
				SetCursorPosition(left + 3 + i, top + 2);
				sprintf(instring + i - 1, "%c", key);
			}
			else
			{
				if (((key >= '0') && (key <= '9')) || ((key >= 'A') && (key <= 'F')))
				{
					printf("%c", key);
					sprintf(htos+j, "%c", key);
					i++;
					j++;
					SetCursorPosition(left + 3 + i, top + 4);
					
					if ((i % 2 == 0) && (i != 0))
					{
						j = 0;
						key = char2int_(htos[0]) * 16 + char2int_(htos[1]);
						SetCursorPosition(left + 3 + i/2 -1, top + 2);
						printf("%c", key);
						sprintf(instring + i - 1, "%c", key);
						SetCursorPosition(left + 3 + i, top + 4);
					}


				}
			}
		}
	} while (key != 27);
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
	EnableCursor(false);
}
