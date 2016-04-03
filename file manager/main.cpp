#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;
char *logFile;
COORD ConsoleSize = { 122,40 };
struct files{
	_finddata_t file;
	files *prev;
	files *next;
}flist, slist;
enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
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

	for (int i = 0; i < (ConsoleSize.X-2); i++) chiBuffer[i].Attributes = (WORD)((a << 4) | b);
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void readBlockDown()
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	BOOL fSuccess;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;

	// The temporary buffer size is 1 rows x 120 columns.

	coordBufSize.Y = ConsoleSize.Y - 6;
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

	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;

	for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[i].Attributes = (WORD)((Blue << 4) | White);
	fSuccess = WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}
void readBlockUp()
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	int size = (ConsoleSize.Y - 6)*(ConsoleSize.X - 2);
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	BOOL fSuccess;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = 3;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 3; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;

	// The temporary buffer size is 1 rows x 120 columns.

	coordBufSize.Y = ConsoleSize.Y - 6;
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

	srctReadRect.Top = 2;    // top left: row 2, col 1 
	srctReadRect.Left = 1;
	srctReadRect.Bottom = ConsoleSize.Y - 4; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X - 1;

	for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[size-i-1].Attributes = (WORD)((Blue << 4) | White);
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
	for (short j = 0; j < (bottom - top - 2); ++j)
	{
		SetCursorPosition(left, top + j + 1);
		printf("%c", 186);
		for (int i = 0; i < (right - left - 2); ++i)
			printf(" ");
		printf("%c", 186);
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
void SetBufferSize()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT src = { 0, 0, ConsoleSize.X-1, ConsoleSize.Y-1 };
	SetConsoleScreenBufferSize(hConsole, ConsoleSize);
	SetConsoleWindowInfo(hConsole, TRUE, &src);
}
void SelectStr(int a)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 1, a + 2 };
	SetConsoleCursorPosition(hConsole, position);
}	 
void addFiles(_finddata_t a, files **b)
{
	files *adding = new files;
	adding->file = a;
	files *last = *b;
	adding->next = NULL;
	if (last)               
	{
		while (last->next) last = last->next;
		adding->prev = last;
		last->next = adding;
	}
	else
		*b = adding;
}
void add(_finddata_t a, files **b)
{
	files *adding = new files;
	adding->file = a;
	adding->prev = NULL;
	adding->next = *b;     //Список в обратном порядке
	*b = adding;
	if ((adding->next)) (*b)->next->prev = adding;
}
void showStr(char *FileName, _fsize_t FileSize, unsigned int attrib, int x)
{
	x -= 2;
	int FileNameLen;
	for (FileNameLen = 0; FileName[FileNameLen]; FileNameLen++);
	if (FileNameLen > (5 * x / 6 - 1))
	{
		for (int i = 0; i < (5 * x / 6 - 3); i++)
			printf("%c", FileName[i]);
		printf("..");
	}
	else
	{
		for (int i = 0; FileName[i]; i++)
			printf("%c", FileName[i]);
		for (int i = 0; i < (5 * x / 6 - 1 - FileNameLen); i++)
			printf(" ");
	}
	//setlocale(LC_ALL, "C");
	printf("%c", 166);  // 179 166
	//setlocale(LC_ALL, "rus");
	if (attrib & _A_SUBDIR)
	{
		printf("FOLDER");
		for (int i = 0; i < x / 6 - 6; i++)
			printf(" ");
	}
	else
	{
		
		
			if (FileSize / 10000000)
			{
				FileSize /= 1024 * 1024;
				printf("%dMB", FileSize);
			}
			else
				if (FileSize / 10000)
				{
					FileSize /= 1024;
					printf("%dKB", FileSize);
				}
				else printf("%dB ", FileSize);
				int k = 0;
				if (FileSize == 0) k = 1;
				else
					while (FileSize)
					{
						FileSize /= 10;
						k++;
					}
				for (int i = 0; i < (x / 6 - 2 - k); i++)
					printf(" ");
		
	}
}
void showClearStr(int x)
{
	x -= 2;
	for (int i = 0; i < (5 * x / 6 - 1); i++)
		printf(" ");
	printf("%c", 166);      //179
	for (int i = 0; i < (x / 6); i++)
		printf(" ");
}
void show(files *first, int a, bool b)	
{
	files *showing = first;
	for (int i = 0; i < a; i++) showing = showing->next;
	int i = ConsoleSize.Y - 5;
	if (!b)
	{
		SetColor(Cyan, White);
		SelectStr(ConsoleSize.Y - 5 - i);
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X);
		showing = showing->next;
		i--;
		SetColor(Blue, White);
		while ((showing && i))
		{
			SelectStr(ConsoleSize.Y - 5 - i);
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X);
			showing = showing->next;
			i--;
		}
	}
	else
	{
		SetColor(Blue, White);
		while ((showing && i-1))
		{
			SelectStr(ConsoleSize.Y - 5 - i);
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X);
			showing = showing->next;
			i--;
		}
		SetColor(Cyan, White);
		SelectStr(ConsoleSize.Y - 5 - i);
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X);
		i--;
	}

	if (i) for (; i > 0; i--)
	{
		SelectStr(ConsoleSize.Y - 5 - i);
		showClearStr(ConsoleSize.X);
	}
}	
void deleteAll(files **flast)
{
	files *t = *flast;
	files *del;
	if (flast == NULL) return;
	while (t)
	{
		del = t;
		t = t->next;
		delete del;
	}
	*flast = NULL;
}
void sortAlph(files **flast)
{
	files *pointer = *flast;
	files *sortFiles = slist.next;
	while (pointer)
	{
		if (pointer->file.attrib & _A_SUBDIR) addFiles(pointer->file, &sortFiles);
		pointer = pointer->next;
	}
	pointer = *flast;
	while (pointer)
	{
		if (!(pointer->file.attrib & _A_SUBDIR)) addFiles(pointer->file, &sortFiles);
		pointer = pointer->next;
	}
	deleteAll(flast);
	*flast = sortFiles;
	pointer = flist.next;
	flist.next = slist.next;
	slist.next = pointer;
}
void searchFiles(files **flast, unsigned int *a) // принимает два параметра: путь и указатель на начало списка
{
	*a = 0;
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst("*.*", &myfile); 
	if ((p != -1) && (myfile.name[0] != '.') && (myfile.name[1] != '\0'))
		{
			addFiles(myfile, flast);
			(*a)++;
		}
		while (_findnext(p, &myfile) != -1)
		{
			addFiles(myfile, flast);
			(*a)++;
		} 
		_findclose(p);
		sortAlph(flast);
}
void FileCopy(FILE *source, FILE *dist)
{
	unsigned int fsize, n;
	fseek(source, 0, SEEK_END);
	fsize = ftell(source);
	rewind(source);
	char *buffer;
	n = fsize / 65536;
	if (n > 0)
	{
		buffer = new char[65536];
		for (int i = 0; i < n; ++i)
		{
			fread(buffer, 65536, sizeof(char), source);
			fwrite(buffer, 65536, sizeof(char), dist);
		}
		delete[] buffer;
	}
	int ost = fsize - n * 65536;
	buffer = new char[ost];
	fread(buffer, ost, sizeof(char), source);
	fwrite(buffer, ost, sizeof(char), dist);
	delete[] buffer;
}
void RefreshFiles(files **flast, unsigned int *fCount, int *CrntStr, int *CrntFile, files **fCrnt)
{
	deleteAll(flast);
	searchFiles(flast, fCount);
	show(*flast, 0, FALSE);
	*CrntStr = 0;
	*CrntFile = 0;
	*fCrnt = *flast;
}
void addLog(char *message, char  *typemessage, char *extramessage = "")
{
	FILE *fLog;
	if (message != "Программа запущена")
		for (int j = 0; j < 20; j++)
		{
			fLog = fopen(logFile, "a");
			if (fLog) break;
			this_thread::sleep_for(chrono::milliseconds(20));
		}
	else fLog = fopen(logFile, "w");
	if (!fLog) return;
	time_t timer;
	char * timestring;
	time(&timer);
	timestring = asctime((localtime(&timer)));
	int i;
	for (i = 0; timestring[i]; ++i);
	fwrite(timestring, i-1, sizeof(char), fLog);
	fwrite(" ", 1, sizeof(char), fLog);
	for (i = 0; typemessage[i]; ++i);
	fwrite(typemessage, i+1, sizeof(char), fLog);
	for (i = 0; message[i]; ++i);
	fwrite(message, i+1, sizeof(char), fLog);
	fwrite(" ", 1, sizeof(char), fLog);
	for (i = 0; extramessage[i]; ++i);
	fwrite(extramessage, i+1, sizeof(char), fLog);
	fwrite("\n", 1, sizeof(char), fLog);
	fclose(fLog);
}
void ConsoleFrame()
{
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
	for (int i = 0; i < ConsoleSize.Y; ++i)
	{
		printf("%c", 186);
		/*for (int j = 0; j < (5 * ConsoleSize.X / 6 - 1); ++j)
			printf(" ");
		printf("%c", 179);
		for (int j = 0; j < (ConsoleSize.X / 6); ++j)
			printf(" ");*/
		for (int j = 0; j < ConsoleSize.X; ++j)
			printf(" ");
		printf("%c", 186);
	}
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
	printf("F2-NAME");
	SetColor(Black, White);
	printf(" ");
	SetColor(Red, White);
	printf("F3-COPY");
	SetColor(Black, White);
	printf(" ");


}
void getLogPath(const char *argv[])
{
	int i;
	for (i = 0; argv[0][i]; ++i);
	i -= 4;
	logFile = new char[i];
	memcpy(logFile, argv[0], i-12);
	memcpy(logFile+i-12, "logFile.log", 12);
}
void ExistFile(char (*str)[260])
{
	int i;
	for (i = 0; (*str)[i]; ++i);
	if (i > 256)
	{
		showError("Файл с таким именем уже существует", "и его имя слишком длинное");
		return;
	}
	int k = i;
	for (; (*str)[k] != '.'; --k);
	char *str1 = new char[k + 2];
	char *str2 = new char[i - k + 2];
	memcpy(str1, *str, k);
	str1[k] = '\0';
	memcpy(str2, (*str) + k, i - k);
	str2[i - k] = '\0';
	i = 1;
	FILE *f;
	while (1)
	{
		sprintf(*str, "%s(%d)%s", str1, i, str2);
		f = fopen(*str, "r");
		if (!f) break;
		fclose(f);
		i++;
	}
	delete[] str1;
	delete[] str2;
}
void newFolder()
{
	char str[25];
	int i = 1;
	char str1[] = "Новая папка";
	if (_chdir(str1))
	{
		_mkdir(str1);
		return;
	}
	_chdir("..");
	while (1)
	{
		sprintf(str, "%s(%d)", str1, i);
		if (_chdir(str))
		{
			_mkdir(str);
			return;
		}
		_chdir("..");
		i++;
	}
}
void deleteFolder(char *path)
{
	int error;
	errno = 0;
	error = _rmdir(path);
	if (error == -1)
	{
		switch (errno)
		{
		case EACCES:
			showError("У вас не прав к удалению данной папки", "");
			break;
		case EBUSY:
			showError("Данная папка открыта", "Закройте ее для удаления");
			break;
		case 41:
			_chdir(path);
			do
			{
				_finddata_t myfile;  intptr_t p;
				p = _findfirst("*.*", &myfile);
				_findnext(p, &myfile);
				_findnext(p, &myfile);
				_findclose(p);
				if ((myfile.name[0] == '.')  && (myfile.name[1] == '.') &&(myfile.name[2] == '\0')) break;
				if (myfile.attrib & _A_SUBDIR) deleteFolder(myfile.name);
				else remove(myfile.name);
			} while (1);
			_chdir("..");
			_rmdir(path);
			break;
		default:
			break;
		}
	
	}

}
void FolderCopy(char *path, char *fCopy)
{
	_mkdir(fCopy);
	_chdir(fCopy);
	_finddata_t myfile;  intptr_t p;
	int k1, k2;
	FILE *source, *dist;
	for (k1 = 0; path[k1]; ++k1);
	char *pathCopy = new char[k1 + 5];
	sprintf(pathCopy, "%s\\*.*", path);
	p = _findfirst(pathCopy, &myfile);
	delete[] pathCopy;
	_findnext(p, &myfile);
	while (_findnext(p, &myfile) != -1)
	{
		for (k2 = 0; myfile.name[k2]; ++k2);
		pathCopy = new char[k1 + k2 + 2];
		sprintf(pathCopy, "%s\\%s", path, myfile.name);
		if (myfile.attrib & _A_SUBDIR) FolderCopy(pathCopy, myfile.name);
		else {
			source = fopen(pathCopy, "r+b");
			if (source != NULL)
			{
				dist = fopen(myfile.name, "w+b");
				FileCopy(source, dist);
				fclose(source);
				fclose(dist);
			}
			else showError(myfile.name, "Данный файл не может быть скопирован");
		}
		delete[] pathCopy;
	}
	_findclose(p);
	_chdir("..");
}
void selectHEX(int CrntStr, int CrntStl, bool select)
{
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO *chiBuffer = new CHAR_INFO[ConsoleSize.X - 2]; // [1][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = ConsoleSize.Y / 2 - 11 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 29 + 3 * CrntStl;
	srctReadRect.Bottom = ConsoleSize.Y / 2 - 10 + CrntStr; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 27 + 3 * CrntStl;
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
	srctReadRect.Top = ConsoleSize.Y / 2 - 11 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 39;
	srctReadRect.Bottom = ConsoleSize.Y / 2 - 10 + CrntStr; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 31;
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
	srctReadRect.Top = ConsoleSize.Y / 2 - 12;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 29 + 3 * CrntStl;
	srctReadRect.Bottom = ConsoleSize.Y / 2 - 11; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 - 27 + 3 * CrntStl;
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
	srctReadRect.Top = ConsoleSize.Y / 2 - 11 + CrntStr;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 + 20 + CrntStl;
	srctReadRect.Bottom = ConsoleSize.Y / 2 - 10 + CrntStr; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 21 + CrntStl;
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
	int size = 21*78;
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = ConsoleSize.Y / 2 - 11;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 39;
	srctReadRect.Bottom = ConsoleSize.Y / 2 + 10; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 39;
	coordBufSize.Y = 21;
	coordBufSize.X = 78;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 

	srctReadRect.Top = ConsoleSize.Y / 2 - 10;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y / 2 + 11; // bot. right: row 2, col 120 

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
	int size = 21 * 78;
	CHAR_INFO *chiBuffer = new CHAR_INFO[size]; // [][122]; 
	COORD coordBufSize;
	COORD coordBufCoord;												/*Чтение из консоли*/
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	srctReadRect.Top = ConsoleSize.Y / 2 - 10;    // top left: row 2, col 1 
	srctReadRect.Left = ConsoleSize.X / 2 - 39;
	srctReadRect.Bottom = ConsoleSize.Y / 2 + 11; // bot. right: row 2, col 120 
	srctReadRect.Right = ConsoleSize.X / 2 + 39;
	coordBufSize.Y = 21;
	coordBufSize.X = 78;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		hStdout,        // screen buffer to read from 
		chiBuffer,      // buffer to copy into 
		coordBufSize,   // col-row size of chiBuffer 
		coordBufCoord,  // top left dest. cell in chiBuffer 
		&srctReadRect); // screen buffer source rectangle 

	srctReadRect.Top = ConsoleSize.Y / 2 - 11;    // top left: row 2, col 1 
	srctReadRect.Bottom = ConsoleSize.Y / 2 + 10; // bot. right: row 2, col 120 

												  //for (int i = 0; i < (ConsoleSize.X); i++) chiBuffer[i].Attributes = (WORD)((Blue << 4) | White);
	WriteConsoleOutput(
		hStdout, // screen buffer to write to 
		chiBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctReadRect);  // dest. screen buffer rectangle 
	delete[] chiBuffer;
}

void renameWindow(char *FileName)
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
						for (i--; i-lchar < 77; i++)
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
					for (i -= 78; NewName[i+1]; i++)
						printf("%c", NewName[i]);
					NewName[i] = '\0';
					lchar--;
				}
		}
		if (key == 13)
		{
			rename(FileName, NewName);
			addLog(FileName, "RENAME", NewName);
			break;
		}
	} while (key != 27);
	EnableCursor(false);
	hideWindow(chiBuffer, top, left, bottom, right);
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
void runHEX(char *FileName, _fsize_t FileSize)

{
	CHAR_INFO *chiBuffer = new CHAR_INFO[80 * 25];
	short top = ConsoleSize.Y / 2 - 13;
	short bottom = ConsoleSize.Y / 2 + 12;
	short left = ConsoleSize.X / 2 - 40;
	short right = ConsoleSize.X / 2 + 40;
	showWindow(&chiBuffer, top, left, bottom, right, Magenta);
	FILE *fHex, *fNew;
	unsigned int lastAdress = FileSize / 16;
	int lastStl;
	if (FileSize % 16 < 16) lastStl = FileSize % 16 + 1;
	else
	{
		lastStl = 1;
		lastAdress++;
	}
	unsigned char c;
	bool secondChar = false; 
	rename(FileName, "BufferFile");
	fHex = fopen("BufferFile", "r+b");
	fNew = fopen(FileName, "w+b");
	FileCopy(fHex, fNew);
	rewind(fHex);
	SetCursorPosition(left + 11, top + 1);
	SetColor(Magenta, Yellow);
	unsigned int adress;
	for (unsigned int i = 0; i < 16; i++) printf("%02X ", i);
	for (adress = 0; adress < 22 && !feof(fHex); adress++)
	{
		SetCursorPosition(left + 1, top + 2 + adress);
		SetColor(Magenta, Yellow);
		printf("%08X", adress);
		for (int i = 0; i < 16; i++)
		{
			fread(&c, sizeof(char), 1, fHex);
			if (feof(fHex)) break;
			SetCursorPosition(left + 11 + i * 3, top + 2 + adress);
			SetColor(Magenta, White);
			printf("%02X", c);
			SetCursorPosition(left + 60 + i, top + 2 + adress);
			SetColor(Magenta, Yellow);
			if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
			else
				printf("%c", c);
		}
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
					if (CrntStr < 21)
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
						fseek(fHex, adress * 16, SEEK_SET);
						SetCursorPosition(left + 1, bottom - 2);
						SetColor(Magenta, Yellow);
						printf("%08X", adress);
						for (int i = 0; i < 16; i++)
						{
							fread(&c, sizeof(char), 1, fHex);
							if (feof(fHex))
							{
								while (i < 16)
								{
									SetCursorPosition(left + 11 + i * 3, bottom - 2);
									printf("  ");
									SetCursorPosition(left + 60 + i, bottom - 2);
									printf(" ");
									i++;
								}
								break;
							}
							SetCursorPosition(left + 11 + i * 3, bottom - 2);
							SetColor(Magenta, White);
							printf("%02X", c);
							SetCursorPosition(left + 60 + i, bottom - 2);
							SetColor(Magenta, Yellow);
							if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
							else
								printf("%c", c);
						}
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
						fseek(fHex, adress * 16, SEEK_SET);
						SetCursorPosition(left + 1, top + 2);
						SetColor(Magenta, Yellow);
						printf("%08X", adress);
						for (int i = 0; i < 16; i++)
						{
							fread(&c, sizeof(char), 1, fHex);
							SetCursorPosition(left + 11 + i * 3, top + 2);
							SetColor(Magenta, White);
							printf("%02X", c);
							SetCursorPosition(left + 60 + i, top + 2);
							SetColor(Magenta, Yellow);
							if (((c >= 0x00) && (c <= 0x0f)) || (c == 0x95)) printf(".");
							else
								printf("%c", c);
						}
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
		if (key == 8)
		{

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
				fseek(fNew, adress * 16 + CrntStl, SEEK_SET);
				fwrite(&c, 1, sizeof(char), fNew);
				secondChar = true;
				SetColor(Yellow, Black);
				SetCursorPosition(left + 60 + CrntStl, top + 2 + CrntStr);
				printf("%c", c);
				SetColor(Magenta, White);
				SetCursorPosition(left + 11 + CrntStl * 3, top + 2 + CrntStr);
				printf("%X", key);
			}
			else
			{
				printf("%X", key);
				c = c / 16 * 16 + key;
				fseek(fNew, adress * 16 + CrntStl, SEEK_SET);
				fwrite(&c, 1, sizeof(char), fNew);
				SetColor(Magenta, Yellow);
				SetCursorPosition(left + 60 + CrntStl, top + 2 + CrntStr);
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
					if (CrntStr < 21)
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
						SetCursorPosition(left + 1, bottom - 2);
						SetColor(Magenta, Yellow);
						printf("%08X", adress);
						for (int i = 0; i < 16; i++)
						{
							fread(&c, sizeof(char), 1, fHex);
							if (feof(fHex))
							{
								while (i < 16)
								{
									SetCursorPosition(left + 11 + i * 3, bottom - 2);
									printf("  ");
									SetCursorPosition(left + 60 + i, bottom - 2);
									printf(" ");
									i++;
								}
								break;
							}
							SetCursorPosition(left + 11 + i * 3, bottom - 2);
							SetColor(Magenta, White);
							printf("%02X", c);
							SetCursorPosition(left + 60 + i, bottom - 2);
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
	fclose(fNew);
	fclose(fHex);
	remove("BufferFile");
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}

	int main(int argc, const char * argv[]) 
	{
		int a = sizeof(_finddata_t);
		bool Disk[26];
		listDisk(Disk);
		getLogPath(argv);	
		SetConsoleTitle(L"File Manager");
		addLog("Программа запущена","INFO");
		SetBufferSize();
		EnableCursor(false);
		_chdir("C:\\");
		FILE *source = NULL, *dist;
		unsigned int fCount;
		char buffer[260], fCopy[260], pathCopy[MAX_PATH];
		pathCopy[0] = '\0';
		ConsoleFrame();
		setlocale(LC_ALL, "rus");
		int CrntStr = 0, key, CrntFile = 0;
		files *flast = flist.next;
		searchFiles(&flast, &fCount);
		files *fCrnt = flast;
		show(flast, 0,FALSE);
		do
		{
			key = _getch();
			switch (key)
			{
			case 224:
				key = _getch();
				if ((key == 80) && (CrntFile < (fCount - 1)))
				{
					fCrnt = fCrnt->next;
					if (CrntStr + 1 == (ConsoleSize.Y - 5))
					{
						readBlockUp();
						CrntFile++;
						SelectStr(CrntStr);
						SetColor(Cyan, White);
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.X);
					}
					else
					{
						readStringFromConsole(CrntStr, Blue, White);
						CrntStr++;
						CrntFile++;
						SelectStr(CrntStr);
						readStringFromConsole(CrntStr, Cyan, White);
					}
				}
				if ((key == 72) && (CrntFile)) {
					fCrnt = fCrnt->prev;
					if (!CrntStr)
					{
						readBlockDown();
						CrntFile--;
						SelectStr(CrntStr);
						SetColor(Cyan, White);
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.X);
					}
					else
					{
						readStringFromConsole(CrntStr, Blue, White);
						CrntStr--;
						CrntFile--;
						SelectStr(CrntStr);
						readStringFromConsole(CrntStr, Cyan, White);
					}
				}
				if (key == 83)
				{
					if ((fCrnt->file.name[0] == '.') && (fCrnt->file.name[1] == '.') && (fCrnt->file.name[2] == '\0')) showError("Путь назад невозможно удалить","");
					else
					{
						if (fCrnt->file.attrib & _A_SUBDIR)
						{
							deleteFolder(fCrnt->file.name); 
							addLog(fCrnt->file.name, "INFO", "Удалена");
						}																			//удаление папки
						else
							if (remove(fCrnt->file.name) == -1) showError("Данный файл не может быть удален", "");
							else addLog(fCrnt->file.name, "INFO", "Удален");
						RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					}
				}
				break;
			case 13:
				if (fCrnt->file.attrib & _A_SUBDIR)
				_chdir(fCrnt->file.name);
				addLog("Перешли в папку", "INFO", fCrnt->file.name);
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				break;
			case 8:
				_chdir("..");
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				break;
			case 'c':
				if (source != NULL) fclose(source);
				pathCopy[0] = '\0';
				memcpy(fCopy, fCrnt->file.name, 260);
				if (fCrnt->file.attrib & _A_SUBDIR)
				{
					_chdir(fCopy);
					_getcwd(pathCopy, MAX_PATH);
					_chdir("..");
				}
				else 
				{
					source = fopen(fCopy, "r+b");
					if (source != NULL)  addLog(fCopy,"COPY", "Добавлен в буфер");
					else
					{
						addLog("Ошибка открытия файла", "ERROR");
						showError("Файл не может быть скопирован", "");
					}
				}
				break;
			case 'v':
				if (source != NULL)
				{
					dist = fopen(fCopy, "r+b");
					memcpy(buffer, fCopy, 260);
					if (dist != NULL)
					{
						fclose(dist);
						ExistFile(&fCopy);
					}
					dist = fopen(fCopy, "w+b");
					FileCopy(source, dist);
					fclose(dist);
					memcpy(fCopy, buffer, 260);
					addLog(fCopy, "COPY", "Успешно скопирован");
				}
				if (pathCopy[0]) FolderCopy(pathCopy, fCopy);
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				addLog(fCopy, "COPY", "Успешно скопирован");
				break;
			case 'e':
				showError("Вы нажали не на ту клавишу", "О БОЖЕ!!!");
				addLog("Нажата клавиша ошибки", "ERROR");
				break;
			case 'h':
				if (!(fCrnt->file.attrib & _A_SUBDIR))
				{
					runHEX(fCrnt->file.name, fCrnt->file.size);
					addLog(fCrnt->file.name, "INFO", "Открыт в HEX редакторе");
				}
				break;
			case 't':
				newFolder();
				addLog("Созданна новая папка", "INFO");
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				break;
			case 'r':
				renameWindow(fCrnt->file.name);
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				break;
			default:
				break;
			}
			//SetColor(Blue, White);
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("Программа выключена","INFO");
		delete[] logFile;
		return 0;
	}
