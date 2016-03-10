


	
#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
using namespace std;
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
	files *showing = first;
	if (showing) {
		SetColor(Cyan, White);
		printf("%s\t%d\n", showing->file.name, showing->file.attrib);
		showing=showing->next;
	}
	SetColor(Blue, White);
	while ((showing))
	{
		printf("%s\t%d\n", showing->file.name, showing->file.attrib);
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
void GetFileName(files *flast, char *buffer, int k)
{
	for (; k > 0; --k) flast=flast->next;
	memcpy(buffer, flast->file.name, 80);
}
int GetFileAttrib(files *flast, int k)
{
	int a;
	for (; k > 0; --k) flast = flast->next;
	a = flast->file.attrib;
	return a;
}

	int main() 
	{
		DisableCursor();
		_chdir("C:\\");
		unsigned int fCount;
		char buffer[80];
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

				SelectStr(CrntStr);
			}
			if ((key == 13) && ((GetFileAttrib(flast, CrntStr) == _A_SUBDIR) 
				|| (GetFileAttrib(flast, CrntStr) == 17) || (GetFileAttrib(flast, CrntStr) == 22)))
			//if ((key == 13) && (GetFileAttrib(flast, CrntStr) != 32))
			{
				GetFileName(flast, buffer, CrntStr);
				_chdir(buffer);
				deleteAll(&flast);
				searchFiles(&flast, &fCount);
				system("cls");
				show(flast);
				CrntStr = 0;
			}
			if (key == 8)
			{
				_chdir("..");
				deleteAll(&flast);
				searchFiles(&flast, &fCount);
				system("cls");
				show(flast);
				CrntStr = 0;
			}
			SetColor(Blue, White);
		} while (key != 27);
		return 0;
	}
