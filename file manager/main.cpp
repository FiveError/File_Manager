


	
#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include <ctime>
using namespace std;
FILE *fLog;
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
void SetBufferSize(COORD BufferSize)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT src = { 0, 0, BufferSize.X-1, BufferSize.Y-1 };
	SetConsoleScreenBufferSize(hConsole, BufferSize);
	SetConsoleWindowInfo(hConsole, TRUE, &src);
}
void SetColor(ConsoleColor a, ConsoleColor b)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((a << 4) | b));
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
	/*files *last = *b;
	adding->next = NULL;
	if (last)                //Список в правильном порядке
	{
		while (last->next) last = last->next;
		last->next = adding;
	}
	else
		**b = adding; */
	adding->next = *b;     //Список в обратном порядке
	*b = adding;
	if ((adding->next)) (*b)->next->prev = adding;
}
void showStr(char *FileName, _fsize_t FileSize, int x)
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
		//printf("%s", FileName);
		for (int i = 0; FileName[i]; i++)
			printf("%c", FileName[i]);
		for (int i = 0; i < (5 * x / 6 - 1 - FileNameLen); i++)
			printf(" ");
	}
	//setlocale(LC_ALL, "C");
	printf("%c", 166);  // 179 166
	//setlocale(LC_ALL, "rus");
	if (!FileSize)
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
	setlocale(LC_ALL, "C");
	printf("%c", 179);
	setlocale(LC_ALL, "rus");
	for (int i = 0; i < (x / 6); i++)
		printf(" ");
}
void show(files *first, COORD ConsoleSize, int a, bool b)	
{
	files *showing = first;
	for (int i = 0; i < a; i++) showing = showing->next;
	int i = ConsoleSize.Y - 5;
	if (!b)
	{
		SetColor(Cyan, White);
		SelectStr(ConsoleSize.Y - 5 - i);
		showStr(showing->file.name, showing->file.size, ConsoleSize.X);
		showing = showing->next;
		i--;
		SetColor(Blue, White);
		while ((showing && i))
		{
			SelectStr(ConsoleSize.Y - 5 - i);
			showStr(showing->file.name, showing->file.size, ConsoleSize.X);
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
			showStr(showing->file.name, showing->file.size, ConsoleSize.X);
			showing = showing->next;
			i--;
		}
		SetColor(Cyan, White);
		SelectStr(ConsoleSize.Y - 5 - i);
		showStr(showing->file.name, showing->file.size, ConsoleSize.X);
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
/*void SortAlphWRONG!!!(files *flast)
{
	files *pointer = flast;
	files *sortFiles = slist.next;
	while (pointer->next) pointer = pointer->next;
	while (pointer->prev)
	{
		if (pointer->file.attrib == _A_SUBDIR)
			addFiles(pointer->file, &sortFiles);
		pointer = pointer->prev;
	}
	while (pointer->next) pointer = pointer->next;
	while (pointer->prev)
	{
		if (pointer->file.attrib != _A_SUBDIR)
			addFiles(pointer->file, &sortFiles);
		pointer = pointer->prev;
	}
	flast = sortFiles;
	deleteAll(&sortFiles);
}*/
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
int GetFileAttrib(files *flast, int CrntStr)
{
	int a;
	for (; CrntStr > 0; --CrntStr) flast = flast->next;
	a = flast->file.attrib;
	return a;
}
/*bool CheckFile(files *flast, char *fCheck, unsigned int fCount)
{
	for (int i = 0; i < fCount; ++i)
	{

		flast = flast->next;
	}

	
}*/
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
void RefreshFiles(files **flast, unsigned int *fCount, COORD ConsoleSize)
{
	deleteAll(flast);
	searchFiles(flast, fCount);
	show(*flast, ConsoleSize, 0, FALSE);
}
void addLog(char *message, char  *typemessage)
{
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
}
void ConsoleFrame(COORD ConsoleSize)
{
	SetColor(Blue, White);
	ConsoleSize.X -= 2;
	printf("%c", 201);
	for (int i = 0; i < (5 * ConsoleSize.X / 6-1); ++i)
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
/*void RefreshStr(int CrntStr)
{
	PCHAR_INFO buffer;
	PSMALL_RECT lpReadRegion;
	COORD dwBufferSize{ 78,1 };
	COORD dwBufferCoord{ 1, CrntStr + 2 };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ReadConsoleOutput(hConsole, buffer, dwBufferSize, dwBufferCoord, lpReadRegion);
	SetColor(Blue, White);
	WriteConsoleOutput(hConsole, buffer, dwBufferSize, dwBufferCoord, lpReadRegion);
}*/

	int main() 
	{
		//COORD ConsoleSize = { 122,40 };

		SetConsoleTitle(L"File Manager");
		COORD ConsoleSize = { 80,25 };
		fLog = fopen("logfile.log", "w");
		addLog("Программа запущена","INFO");
		SetBufferSize(ConsoleSize);
		DisableCursor();
		_chdir("C:\\");
		FILE *source = NULL, *dist;
		unsigned int fCount;
		char buffer[260], fCopy[260];
		ConsoleFrame(ConsoleSize);
		setlocale(LC_ALL, "rus");
		int choice = 1, CrntStr = 0, key, CrntFile = 0;
		files *flast = flist.next;
		searchFiles(&flast, &fCount);
		show(flast, ConsoleSize, 0,FALSE);
		do
		{
			SelectStr(CrntStr);
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				if ((key == 80) && (CrntFile < (fCount-1)))
				{
					if (CrntStr+1 == (ConsoleSize.Y - 5))
					{
						show(flast, ConsoleSize, CrntFile - ConsoleSize.Y + 7, TRUE);
						CrntFile++;
					}
					else 
					{
						SetColor(Blue, White);
						GetFileName(flast, buffer, CrntFile);
						showStr(buffer, GetFileSize(flast, CrntFile), ConsoleSize.X);
						//RefreshStr(CrntStr);
						CrntStr++;
						CrntFile++;
						SelectStr(CrntStr);
						SetColor(Cyan, White);
						GetFileName(flast, buffer, CrntFile);
						showStr(buffer, GetFileSize(flast, CrntFile), ConsoleSize.X);
					}
				}
				if ((key == 72) && (CrntFile)) {
					if (!CrntStr)
					{
						show(flast, ConsoleSize, CrntFile-1, FALSE);
						CrntFile--;
					}
					else
					{
						SetColor(Blue, White);
						GetFileName(flast, buffer, CrntFile);
						showStr(buffer, GetFileSize(flast, CrntFile), ConsoleSize.X);
						CrntStr--;
						CrntFile--;
						SelectStr(CrntStr);
						SetColor(Cyan, White);
						GetFileName(flast, buffer, CrntFile);
						showStr(buffer, GetFileSize(flast, CrntFile), ConsoleSize.X);
					}
				}
				if (key == 83)
				{
					GetFileName(flast, buffer, CrntFile);
					remove(buffer);
					RefreshFiles(&flast, &fCount, ConsoleSize);
					CrntStr = 0;
					CrntFile = 0;
				}
				SelectStr(CrntStr);
			}
			if ((key == 13) && (!GetFileSize(flast, CrntFile)))
			{
				GetFileName(flast, buffer, CrntFile);
				_chdir(buffer);
				RefreshFiles(&flast, &fCount, ConsoleSize);
				CrntStr = 0;
				CrntFile = 0;
			}
			if (key == 8)
			{
				_chdir("..");
				RefreshFiles(&flast, &fCount, ConsoleSize);
				CrntStr = 0;
				CrntFile = 0;
			}
			if (key == 'c')
			{
				if (source != NULL) fclose(source);
				GetFileName(flast, fCopy, CrntFile);
				source = fopen(fCopy, "r+b");
				if (source != NULL)  addLog("Файл добавлен в буфер","INFO");
				else addLog("Ошибка открытия файла","ERROR");
			}
			if ((key == 'v') && (source != NULL))
			{
				GetFileName(flast, fCopy, CrntFile);
				dist = fopen(fCopy, "r+b");
				if (dist != NULL)
				{
					fclose(dist);
				}
				GetFileName(flast, fCopy, CrntFile);
				dist = fopen(fCopy, "w+b");
				FileCopy(source, dist);
				fclose(dist);
				RefreshFiles(&flast, &fCount, ConsoleSize);
				CrntStr = 0;
				CrntFile = 0;
			}
			

			SetColor(Blue, White);
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("Программа выключена","INFO");
		fclose(fLog);
		return 0;
	}
