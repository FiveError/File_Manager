


	
#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>

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
void SetBufferSize(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD BufferSize = { x,y };
	SetConsoleScreenBufferSize(hConsole, BufferSize);
}
void SetColor(ConsoleColor a, ConsoleColor b)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((a << 4) | b));
}
void SelectStr(int a)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 0, a };
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
void show(files *first)	
{
	int i = 23; //высота консоли
	files *showing = first;
	if (showing) {
		SetColor(Cyan, White);
		printf("%s\t%X\n", showing->file.name, showing->file.attrib);
		showing=showing->next;
	}
	SetColor(Blue, White);
	while ((showing && i--))
	{
		printf("%s\t%X\n", showing->file.name, showing->file.attrib);
		showing = showing->next;
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
		if ((pointer->file.attrib == _A_SUBDIR) || (pointer->file.attrib == 17) 
			|| (pointer->file.attrib == 22)) addFiles(pointer->file, &sortFiles);
		pointer = pointer->next;
	}
	pointer = *flast;
	while (pointer)
	{
		if ((pointer->file.attrib != _A_SUBDIR) && (pointer->file.attrib != 17)
			&& (pointer->file.attrib != 22)) addFiles(pointer->file, &sortFiles);
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
	if (p != -1)
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
	memcpy(buffer, flast->file.name, 80);
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
void RefreshFiles(files **flast, unsigned int *fCount)
{
	deleteAll(flast);
	searchFiles(flast, fCount);
	system("cls");
	show(*flast);
}
void addLog(char *message)
{
	int i;
	for (i = 0; message[i]; ++i);
	fwrite(message, i+1, sizeof(char), fLog);
	fwrite("\n", 1, sizeof(char), fLog);
}

	int main() 
	{
		fLog = fopen("logfile.log", "w");
		addLog("Программа запущена");
		SetBufferSize(80, 25);
		DisableCursor();
		_chdir("C:\\");
		FILE *source = NULL, *dist;
		unsigned int fCount;
		char buffer[80], fCopy[260];
		setlocale(LC_ALL, "rus");
		system("color 1f");
		int choice = 1, CrntStr = 0, key;
		files *flast = flist.next;
		searchFiles(&flast, &fCount);
		show(flast);
		do
		{
			SelectStr(CrntStr);
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				if ((key == 80) && (CrntStr < (fCount-1)))
				{
					GetFileName(flast, buffer, CrntStr);
					SetColor(Blue, White);
					printf("%s", buffer);
					CrntStr++;
					SelectStr(CrntStr);
					GetFileName(flast, buffer, CrntStr);
					SetColor(Cyan, White);
					printf("%s", buffer);
				}
				if ((key == 72) && (CrntStr)) {
					GetFileName(flast, buffer, CrntStr);
					SetColor(Blue, White);
					printf("%s", buffer);
					CrntStr--;
					SelectStr(CrntStr);
					GetFileName(flast, buffer, CrntStr);
					SetColor(Cyan, White);
					printf("%s", buffer);
				}
				if (key == 83)
				{
					GetFileName(flast, buffer, CrntStr);
					remove(buffer);
					RefreshFiles(&flast, &fCount);
					CrntStr = 0;
				}
				SelectStr(CrntStr);
			}
			if ((key == 13) && ((GetFileAttrib(flast, CrntStr) == _A_SUBDIR) 
				|| (GetFileAttrib(flast, CrntStr) == 17) || (GetFileAttrib(flast, CrntStr) == 22)))
			//if ((key == 13) && (GetFileAttrib(flast, CrntStr) != 32))
			{
				GetFileName(flast, buffer, CrntStr);
				_chdir(buffer);
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
			}
			if (key == 8)
			{
				_chdir("..");
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
			}
			if (key == 'c')
			{
				if (source != NULL) fclose(source);
				GetFileName(flast, fCopy, CrntStr);
				source = fopen(fCopy, "r+b");
				if (source != NULL)  addLog("Файл добавлен в буфер");
				else addLog("Ошибка открытия файла");
			}
			if ((key == 'v') && (source != NULL))
			{
				dist = fopen(fCopy, "r+b");
				if (dist != NULL)
				{
					fclose(dist);
				}
				dist = fopen(fCopy, "w+b");
				FileCopy(source, dist);
				fclose(dist);
				RefreshFiles(&flast, &fCount);
				CrntStr = 0;
			}
			

			SetColor(Blue, White);
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("Программа выключена");
		fclose(fLog);
		return 0;
	}
