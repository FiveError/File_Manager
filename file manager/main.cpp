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
void DisableCursor()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = false;
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
			FileSize /= 1024*1024;
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
		if (!pointer->file.size) addFiles(pointer->file, &sortFiles);
		pointer = pointer->next;
	}
	pointer = *flast;
	while (pointer)
	{
		if (pointer->file.size) addFiles(pointer->file, &sortFiles);
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
void GetFileName(files *flast, char *buffer, int CrntStr)
{
	for (; CrntStr > 0; --CrntStr) flast=flast->next;
	memcpy(buffer, flast->file.name, 260);
}
_fsize_t GetFileSize(files *flast, int CrntStr)
{
	_fsize_t a;
	for (; CrntStr > 0; --CrntStr) flast = flast->next;
	a = flast->file.size;
	return a;
}
unsigned int GetFileAttrib(files *flast, int CrntStr)
{
	unsigned int a;
	for (; CrntStr > 0; --CrntStr) flast = flast->next;
	a = flast->file.attrib;
	return a;
}
void FileCopy(FILE *source, FILE *dist)
{
	unsigned int fsize, n;
	fseek(source, 0, SEEK_END);
	fsize = ftell(source);
	rewind(source);
	char *buffer;
	n = fsize / 65536;
	for (int i = 0; i < n; ++i)
	{
		buffer = new char[65536];
		fread(buffer, 65536, sizeof(char), source);
		fwrite(buffer, 65536, sizeof(char), dist);
		delete[] buffer;
	}
	int ost = fsize - n * 65536;
	buffer = new char[ost];
	fread(buffer, ost, sizeof(char), source);
	fwrite(buffer, ost, sizeof(char), dist);
	delete[] buffer;
}
void RefreshFiles(files **flast, unsigned int *fCount)
{
	deleteAll(flast);
	searchFiles(flast, fCount);
	show(*flast, 0, FALSE);
}
void addLog(char *message, char  *typemessage)
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
void runHEX(char *FileName)
{
	CHAR_INFO *chiBuffer = new CHAR_INFO[80 * 25];
	short top = ConsoleSize.Y / 2 - 13;
	short bottom = ConsoleSize.Y / 2 + 12;
	short left = ConsoleSize.X / 2 - 40;
	short right = ConsoleSize.X / 2 + 40;
	showWindow(&chiBuffer, top, left, bottom, right, Magenta);
	FILE *fHex;
	unsigned char c;
	fHex = fopen(FileName, "r+b");
	SetCursorPosition(left + 11, top + 1);
	SetColor(Magenta, Yellow);
	for (unsigned int i = 0; i < 16; i++) printf("%02X ",i);
	for (unsigned int j = 0; j < 22 && !feof(fHex); j++)
	{
		SetCursorPosition(left + 1, top + 2 + j);
		SetColor(Magenta, Yellow);
		printf("%08X  ", j);
		for (int i = 0; i < 16 && !feof(fHex); i++)
		{

			fread(&c, sizeof(char), 1, fHex);
			SetColor(Magenta, White);
			printf("%02X ", c);
		}
		fseek(fHex, -16, SEEK_CUR);
		for (int i = 0; i < 16 && !feof(fHex); i++)
		{

			fread(&c, sizeof(char), 1, fHex);
			SetColor(Magenta, Yellow);
			if ((c >= 0x00)&&(c <= 0x0f)) printf(".");
			else
			printf("%c", c);
		}
	}
	fclose(fHex);
	_getch();


	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}

	int main(int argc, const char * argv[]) 
	{
		getLogPath(argv);	
		SetConsoleTitle(L"File Manager");
		addLog("Программа запущена","INFO");
		SetBufferSize();
		DisableCursor();
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
		show(flast, 0,FALSE);
		do
		{
			SelectStr(CrntStr);
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				if ((key == 80) && (CrntFile < (fCount - 1)))
				{
					if (CrntStr + 1 == (ConsoleSize.Y - 5))
					{
						readBlockUp();
						CrntFile++;
						GetFileName(flast, buffer, CrntFile);
						SetColor(Cyan, White);
						showStr(buffer, GetFileSize(flast, CrntFile), GetFileAttrib(flast, CrntFile), ConsoleSize.X);
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
					if (!CrntStr)
					{
						readBlockDown();
						CrntFile--;
						GetFileName(flast, buffer, CrntFile);
						SetColor(Cyan, White);
						showStr(buffer, GetFileSize(flast, CrntFile), GetFileAttrib(flast, CrntFile), ConsoleSize.X);
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
					GetFileName(flast, buffer, CrntFile);
					if ((buffer[0] == '.') && (buffer[1] == '.') && (buffer[2] == '\0')) showError("Путь назад невозможно удалить","");
					else
					{
						if (GetFileAttrib(flast, CrntFile) & _A_SUBDIR) deleteFolder(buffer); //удаление папки
						else remove(buffer); //удаление файла
						RefreshFiles(&flast, &fCount);
						CrntStr = 0;
						CrntFile = 0;
					}
				}
				SelectStr(CrntStr);
			}
			if ((key == 13) && (!GetFileSize(flast, CrntFile)))
			{
				GetFileName(flast, buffer, CrntFile);
				_chdir(buffer);
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
				CrntFile = 0;
			}
			if (key == 8)
			{
				_chdir("..");
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
				CrntFile = 0;
			}
			if (key == 'c')
			{
				if (source != NULL) fclose(source);
				pathCopy[0] = '\0';
				GetFileName(flast, fCopy, CrntFile);
				if (GetFileAttrib(flast, CrntFile) & _A_SUBDIR)
				{
					_chdir(fCopy);
					_getcwd(pathCopy, MAX_PATH);
					_chdir("..");
				}
				else 
				{
					source = fopen(fCopy, "r+b");
					if (source != NULL)  addLog("Файл добавлен в буфер","INFO");
					else
					{
						addLog("Ошибка открытия файла", "ERROR");
						showError("Файл не может быть скопирован", "");
					}
				}
			}
			if (key == 'v')
			{
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
				}
				if (pathCopy[0]) FolderCopy(pathCopy, fCopy);
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
				CrntFile = 0;
				memcpy(fCopy, buffer, 260);
			}
			if (key == 'e')
			{
				showError("Вы нажали не на ту клавишу", "О БОЖЕ!!!");
			}
			if (key == 'h')
			{
				GetFileName(flast, buffer, CrntFile);
				runHEX(buffer);
			}
			if (key == 't')
			{
				newFolder();
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
				CrntFile = 0;
				}
			SetColor(Blue, White);
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("Программа выключена","INFO");
		delete[] logFile;
		return 0;
	}
