


	
#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;
char path[100] = "c:\\*.*";
struct files{
	_finddata_t file;
	files *prev;
	files *next;
}flist;
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
		printf("%1s\n", showing->file.name);
		showing=showing->next;
	}
	SetColor(Blue, White);
	while ((showing))
	{
		printf("%1s\n", showing->file.name);
		showing = showing->next;
	}
}	
void searchFiles(char path[100], files **flast) // принимает два параметра: путь и указатель на начало списка
{
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst(path, &myfile); 
	if (p != -1)
	{
		add(myfile, flast);
	}
		while (_findnext(p, &myfile) != -1)
		{
			add(myfile, flast);
		} 
		_findclose(p);
	
}
void deleteAll(files **flast)
{
	files *t = *flast;
	files *del;
	if (flast == NULL) return;
	while (t)
	{
		del = t;
		t= t->next;
		delete del;
	}
	*flast = NULL;
}
void GetFileName(files *flast, char *buffer, int k)
{
	for (; k > 0; --k) flast=flast->next;
	memcpy(buffer, flast->file.name, 80);
}

	int main() 
	{     
		char buffer[80];
		setlocale(LC_ALL, "rus");
		system("color 1f");
		int choice = 1, CrntStr = 0, key;
		files *flast = flist.next;
		searchFiles(path, &flast);
		show(flast);
		SelectStr(CrntStr);
		do
		{
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				if (key == 80)
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
				if (key == 72) {
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
		} while (key != 27);
		return 0;
	}
