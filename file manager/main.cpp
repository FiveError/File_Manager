


	
#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <conio.h>


using namespace std;

char path[100] = "";

struct files{
	_finddata_t file;
	files *next;
}flist;

void add(_finddata_t a, files ***b)
{
	files *adding = new files;
	files *last = **b;
	adding->file = a;
	adding->next = NULL;
	if (last)                //Список в правильном порядке
	{
		while (last->next) last = last->next;
		last->next = adding;
	}
	else
		**b = adding;
	//adding->next = *b;     //Список в обратном порядке
	//*b = adding;
}

void show(files *first)
{
	files *showing = first;
	while ((showing))
	{
		if (showing->file.attrib & _A_SUBDIR)
			printf("%15s\t%s\n", "<DIR>", showing->file.name);
		else
			printf("%15d\t%s\n", showing->file.size, showing->file.name);

		showing = showing->next;
	}
}
	
void searchFiles(char path[100], files **flast) // принимает два параметра: путь и указатель на начало списка
{
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst(path, &myfile); 
	if (p != -1)
	{
		add(myfile, &flast);
	}
		while (_findnext(p, &myfile) != -1)
		{
			add(myfile, &flast);
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
	int main() {
		int choice = 1;
		files *flast = flist.next;
		while (choice)
		{
			cout << "1 - Adres" << endl;
			cout << "2- Clear" << endl;
			cin >> choice;
			switch (choice)
			{
			case 1:
				cout << "Adres" << endl;
				cin >> path;
				searchFiles(path, &flast);
				show(flast);
				break; 
			case 2:
				system("cls");
				deleteAll(&flast);
				break;
			default: 
				break;
			}
		}
		
		system("PAUSE");
		return 0;
	}
