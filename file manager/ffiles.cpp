#include <stdio.h> 
#include <stdlib.h>
#include <io.h>  
#include <iostream>
#include <limits>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <direct.h>
#include "Globals.h"
#include <chrono>
#include <ctime>
#include <thread>
#include "fm.h"
#include "fwin.h"
#include "ffiles.h"
using namespace std;

files flist, slist;
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
void saveClearStr()
{
	clearStr = new char[ConsoleSize.X - 1];
	ConsoleSize.X -= 2;
	for (int i = 0; i < (5 * ConsoleSize.X / 6 - 1); i++)
		sprintf(clearStr + i, " ");
	sprintf(clearStr + 5 * ConsoleSize.X / 6 - 1, "%c", 166);      //179
	for (int i = 0; i < (ConsoleSize.X / 6); i++)
		sprintf(clearStr + 5 * ConsoleSize.X / 6 + i, " ");
	ConsoleSize.X += 2;
}
void showStr(char *FileName, _fsize_t FileSize, unsigned int attrib, int y, bool newlist)
{
	if ((!newlist) || (y == 2))
	{
		SetCursorPosition(1, y);
		printf("%s", clearStr);
	}
	SetCursorPosition(1, y);
	int FileNameLen;
	for (FileNameLen = 0; FileName[FileNameLen]; FileNameLen++);
	if (FileNameLen > 5 * (ConsoleSize.X-2) / 6 - 3) printf("%.*s..¦", 5 * (ConsoleSize.X - 2) / 6 - 3, FileName);
	else
	{
		printf("%s", FileName);
		SetCursorPosition(5 * ConsoleSize.X / 6, y);
	}
	if (attrib & _A_SUBDIR)
		printf("FOLDER");
	else
	{
		if (FileSize >> 23)
		{
			FileSize >>= 20;
			printf("%dMB", FileSize);
		}
		else
			if (FileSize >> 13)
			{
				FileSize >>= 10;
				printf("%dKB", FileSize);
			}
			else printf("%dB", FileSize);
	}
}
void show(files *first)
{
	if (!loadConsoleFrame(frameFile)) ConsoleFrame();
	files *showing = first;
	int i = ConsoleSize.Y - 5;
	SetColor(Cyan, White);
	showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.Y - 3 - i, TRUE);
	showing = showing->next;
	i--;
	SetColor(Blue, White);
	while ((showing && i))
	{
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.Y - 3 - i, TRUE);
		showing = showing->next;
		i--;
	}
}
void show(files * first, int *CrntStr)
{
	if (!loadConsoleFrame(frameFile)) ConsoleFrame();
	files *showing = first;
	int i = ConsoleSize.Y - 5;
	if (*CrntStr < i)
	{
		SetColor(Blue, White);
		while ((showing && i))
		{
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.Y - 3 - i, TRUE);
			showing = showing->next;
			i--;
		}
		readStringFromConsole(*CrntStr, Cyan, White);
	}
	else
	{
		for (; *CrntStr >= i; (*CrntStr)--)
			showing = showing->next;
		SetColor(Blue, White);
		while (i-1)
		{
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.Y - 3 - i, TRUE);
			showing = showing->next;
			i--;
		}
		SetColor(Cyan, White);
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.Y - 4, FALSE);
	}
}
void showLastStr(files * fCrnt, int CrntStr)
{
	SetColor(Blue, White);
	CrntStr++;
	while (CrntStr < ConsoleSize.Y - 5)
	{
		if (!fCrnt->next)
		{
			SetCursorPosition(1, ConsoleSize.Y - 4);
			printf("%s", clearStr);
			return;
		}
		fCrnt = fCrnt->next;
		CrntStr++;
	}
	showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.Y - 4, FALSE);
}
void deleteElement(files ** fCrnt)
{
	files *del = *fCrnt;
	if ((*fCrnt)->prev)
	(*fCrnt)->prev->next = (*fCrnt)->next;
	if ((*fCrnt)->next)
	(*fCrnt)->next->prev = (*fCrnt)->prev;
	if ((*fCrnt)->prev)
	*fCrnt = (*fCrnt)->prev;
	else  *fCrnt = (*fCrnt)->next;
	delete del;
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
	(*flast)->prev = NULL;
	pointer = flist.next;
	flist.next = slist.next;
	slist.next = pointer;
}
int sortAlph(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName)
{
	files *pointer = *flast;
	files *sortFiles = slist.next;
	int a = 0,alength = 0, clength = 0;
	for (; (*crntName)[clength]; clength++);
	for (; (*addName)[alength]; alength++);
	bool addBool = false;
	bool crntBool = false;
	while (pointer)
	{
		if (pointer->file.attrib & _A_SUBDIR)
		{
			addFiles(pointer->file, &sortFiles);
			if (!addBool)
			{
				addBool = TRUE;
				if (pointer->file.name[alength]) addBool = FALSE;
				else
					for (int i = 0; i <= alength; i++)
						if (pointer->file.name[i] != (*addName)[i])
						{
							addBool = FALSE;
							break;
						}
				if (addBool)
				{
					*addFile = sortFiles;
					while ((*addFile)->next) *addFile = (*addFile)->next;
				}
			}
			if (!crntBool)
			{
				crntBool = TRUE;
				if (pointer->file.name[clength]) crntBool = FALSE;
				else
					for (int i = 0; i <= clength; i++)
						if (pointer->file.name[i] != (*crntName)[i])
						{
							crntBool = FALSE;
							break;
						}
				if (crntBool)
				{
					*fCrnt = sortFiles;
					while ((*fCrnt)->next) *fCrnt = (*fCrnt)->next;
				}
			}
			if (crntBool && !addBool) a++;
			if (!crntBool && addBool) a--;
		}
		pointer = pointer->next;
	}
	pointer = *flast;
	while (pointer)
	{
		if (!(pointer->file.attrib & _A_SUBDIR))
		{
			addFiles(pointer->file, &sortFiles);
			if (!addBool)
			{
				addBool = TRUE;
				if (pointer->file.name[alength]) addBool = FALSE;
				else
					for (int i = 0; i <= alength; i++)
						if (pointer->file.name[i] != (*addName)[i])
						{
							addBool = FALSE;
							break;
						}
				if (addBool)
				{
					*addFile = sortFiles;
					while ((*addFile)->next) *addFile = (*addFile)->next;
				}
			}
			if (!crntBool)
			{
				crntBool = TRUE;
				if (pointer->file.name[clength]) crntBool = FALSE;
				else
					for (int i = 0; i <= clength; i++)
						if (pointer->file.name[i] != (*crntName)[i])
						{
							crntBool = FALSE;
							break;
						}
				if (crntBool)
				{
					*fCrnt = sortFiles;
					while ((*fCrnt)->next) *fCrnt = (*fCrnt)->next;
				}
			}
			if (crntBool && !addBool) a++;
			if (!crntBool && addBool) a--;
		}
		pointer = pointer->next;
	}
	deleteAll(flast);
	*flast = sortFiles;
	(*flast)->prev = NULL;
	pointer = flist.next;
	flist.next = slist.next;
	slist.next = pointer;
	return a;
}
void sortAlph(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath)
{
	files *pointer = *flast;
	files *sortFiles = slist.next;
	*CrntStr = 0;
	int length = 0;
	for (; (*prevPath)[length]; length++);
	bool b = false;
	while (pointer)
	{
		if (pointer->file.attrib & _A_SUBDIR)
		{
			addFiles(pointer->file, &sortFiles);
			if (!b)
			{
				b = TRUE;
				if (pointer->file.name[length]) b = FALSE;
				else
					for (int i = 0; i <= length; i++)
						if (pointer->file.name[i] != (*prevPath)[i])
						{
							b = FALSE;
							break;
						}						
				if (b)
				{
					*fCrnt = sortFiles;
					while ((*fCrnt)->next) *fCrnt = (*fCrnt)->next;
				}
				else (*CrntStr)++;
			}
		}
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
	(*flast)->prev = NULL;
	pointer = flist.next;
	flist.next = slist.next;
	slist.next = pointer;
}
void searchFiles(files **flast)
{
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst("*.*", &myfile);
	if ((p != -1) && (myfile.name[0] != '.') && (myfile.name[1] != '\0'))
		addFiles(myfile, flast);
	while (_findnext(p, &myfile) != -1)
		addFiles(myfile, flast);
	_findclose(p);
	sortAlph(flast);
}
int searchFiles(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName)
{
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst("*.*", &myfile);
	if ((p != -1) && (myfile.name[0] != '.') && (myfile.name[1] != '\0'))
		addFiles(myfile, flast);
	while (_findnext(p, &myfile) != -1)
		addFiles(myfile, flast);
	_findclose(p);
	int a = sortAlph(flast, fCrnt, addFile, crntName, addName);
	return a;
}
void searchFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath)
{
	struct _finddata_t myfile;  intptr_t p;
	p = _findfirst("*.*", &myfile);
	if ((p != -1) && (myfile.name[0] != '.') && (myfile.name[1] != '\0'))
		addFiles(myfile, flast);
	while (_findnext(p, &myfile) != -1)
		addFiles(myfile, flast);
	_findclose(p);
	sortAlph(flast, CrntStr, fCrnt, prevPath);
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
void RefreshFiles(files **flast, int *CrntStr, files **fCrnt)
{
	deleteAll(flast);
	searchFiles(flast);
	show(*flast);
	*CrntStr = 0;
	*fCrnt = *flast;
}
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath)
{
	deleteAll(flast);
	searchFiles(flast, CrntStr, fCrnt, prevPath);
	show(*flast, CrntStr);
	delete[] (*prevPath);
	*prevPath = NULL;
}
void addLog(char *message, char  *typemessage, char *extramessage )
{
	FILE *fLog;
	if (strcmp(message, "Программа запущена"))
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
	fwrite(timestring, i - 1, sizeof(char), fLog);
	fwrite(" ", 1, sizeof(char), fLog);
	for (i = 0; typemessage[i]; ++i);
	fwrite(typemessage, i + 1, sizeof(char), fLog);
	for (i = 0; message[i]; ++i);
	fwrite(message, i + 1, sizeof(char), fLog);
	fwrite(" ", 1, sizeof(char), fLog);
	for (i = 0; extramessage[i]; ++i);
	fwrite(extramessage, i + 1, sizeof(char), fLog);
	fwrite("\n", 1, sizeof(char), fLog);
	fclose(fLog);
}
void getLogPath(const char *argv[])
{
	int i;
	for (i = 0; argv[0][i]; ++i);
	i -= 4;
	logFile = new char[i];
	memcpy(logFile, argv[0], i - 12);
	memcpy(logFile + i - 12, "logFile.log", 12);
}
void ExistFile(char(*str)[260])
{
	int i;
	char *str1, *str2;
	for (i = 0; (*str)[i]; ++i);
	if (i > 256)
	{
		showError("Файл с таким именем уже существует", "и его имя слишком длинное");
		return;
	}
	int k = i;
	for (; ((k != -1) && (*str)[k] != '.'); --k);
	if (k == -1)
	{
		str1 = new char[i + 1];
		str2 = new char[1];
		memcpy(str1, *str, i);
		str1[i] = '\0';
		str2[0] = '\0';
	}
	else
	{
		str1 = new char[k + 2];
		str2 = new char[i - k + 2];
		memcpy(str1, *str, k);
		str1[k] = '\0';
		memcpy(str2, (*str) + k, i - k);
		str2[i - k] = '\0';
	}
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
void newFolder(files **flast, files ** fCrnt, int *CrntStr)
{
	int i = 1;
	char str1[] = "Новая папка";
	if (!_mkdir(str1))
	{
		renameWindow(str1);
		addElement(flast, fCrnt,str1 , CrntStr, NULL);
		return;
	}
	char str[25];
	while (1)
	{
		sprintf(str, "%s(%d)", str1, i);
		if (!_mkdir(str)) break;
		i++;
	}
	renameWindow(str);
	addElement(flast, fCrnt, str, CrntStr, NULL);

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
				if ((myfile.name[0] == '.') && (myfile.name[1] == '.') && (myfile.name[2] == '\0')) break;
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
bool FolderCopy(char *path, char *fCopy)
{
	bool makedir = true;
	if (_mkdir(fCopy) == -1) makedir = false;
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
			source = fopen(pathCopy, "rb");
			if (source != NULL)
			{
				dist = fopen(myfile.name, "wb");
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
	return makedir;
}

int addElement(files ** flast, files **fCrnt, char * FileName, int *CrntStr, files **newCurrent)
{
	files *current;
	char *crntName;
	int i, attrib;
	for (i = 0; (*fCrnt)->file.name[i]; i++);
	crntName = new char[i + 1];
	for (i = 0; (*fCrnt)->file.name[i]; i++) crntName[i] = (*fCrnt)->file.name[i];
	crntName[i] = '\0';
	deleteAll(flast);
	i = searchFiles(flast, fCrnt, &current, &crntName, &FileName);
	SetColor(Blue, White);
	if ((i>0) && (i + *CrntStr < ConsoleSize.Y - 5))
	{
		addBlockDown(i + *CrntStr + 2);
		showStr(FileName, current->file.size, current->file.attrib,  *CrntStr + 2 + i, FALSE);
	}
	if ((i<0) && (*CrntStr + 3 + i > 2	))
	{
		if (*CrntStr < ConsoleSize.Y - 6) {
			addBlockDown(*CrntStr + 3 + i);
			showStr(FileName, current->file.size, current->file.attrib, *CrntStr + 3 + i, FALSE);
			(*CrntStr)++;
		}
		else
		{
			addBlockUp(*CrntStr + 3 + i);
			showStr(FileName, current->file.size, current->file.attrib, *CrntStr + 2 + i, FALSE);
		}
	}
	if (newCurrent) *newCurrent = current;
	delete[] crntName;
	return i;
}
void afterRename(files ** flast, files ** fCrnt, int * CrntStr)
{
	char *newName;
	int length;
	for (length = 0; (*fCrnt)->file.name[length]; length++);
	newName = new char[length + 1];
	for (length = 0; (*fCrnt)->file.name[length]; length++) newName[length] = ((*fCrnt)->file.name[length]);
	newName[length] = '\0';
	*fCrnt = (*fCrnt)->prev;
	deleteBlockUp(*CrntStr);
	(*CrntStr)--;
	files *current;
	length = addElement(flast, fCrnt, newName, CrntStr, &current);
	if (!(((length > 0) && (length + *CrntStr < ConsoleSize.Y - 5)) || ((length < 0) && (*CrntStr + 3 + length > 2))))
	{
		show(current);
		*CrntStr = 0;
	}
	else
	{
		showLastStr(*fCrnt, (*CrntStr));
		*CrntStr += length;
	}
	*fCrnt = current;
	delete[] newName;
}
void CountFileFolder(char *FolderPath, unsigned int *countFile, unsigned int *countFolder, unsigned int *sizeFolder)
{
		int sucsess = _chdir(FolderPath);
		if (!sucsess)
		{
			_finddata_t myfile;  intptr_t p;
			p = _findfirst("*.*", &myfile);
			if (p > 0)
			{
				_findnext(p, &myfile);

				while (_findnext(p, &myfile) != -1)
				{
					if ((myfile.attrib & _A_SUBDIR)&& !(myfile.attrib & _A_SYSTEM))
					{
						(*countFolder)++;
						CountFileFolder(myfile.name, countFile, countFolder, sizeFolder);
					}
					else
					{
						(*countFile)++;
						*sizeFolder += myfile.size;
					}
				}
			}
			_findclose(p);
			_chdir("..");
		}
}
	
void FolderInfo(files * fCrnt)
{
	unsigned int countFile = 0, countFolder = 0, sizeFolder = 0;
	CHAR_INFO *chiBuffer = new CHAR_INFO[40 * 20];
	short top = ConsoleSize.Y / 2 - 10;
	short bottom = ConsoleSize.Y / 2 + 10;
	short left = ConsoleSize.X / 2 - 20;
	short right = ConsoleSize.X / 2 + 20;
	showWindow(&chiBuffer, top, left, bottom, right, Cyan);
	SetCursorPosition(left + 10, top + 1);
	printf("Свойства папки");
	SetCursorPosition(left + 5, top + 2);
	printf("Название папки");
	SetCursorPosition(left + 5, top + 3);
	if (strlen(fCrnt->file.name) < 34)
	{
		printf("%s", fCrnt->file.name);
	}
	else
	{
		for (int i = 0; i < 32; ++i)
			printf("%c", fCrnt->file.name[i]);
		printf("..");
	}
	SetCursorPosition(left + 5, top + 4);
	printf("Дата создания");
	SetCursorPosition(left + 5, top + 5);
	printf("%s", asctime((localtime(&fCrnt->file.time_create))));
	SetCursorPosition(left + 5, top + 6);
	printf("Дата изменения");
	SetCursorPosition(left + 5, top + 7);
	printf("%s", asctime((localtime(&fCrnt->file.time_write))));
	SetCursorPosition(left + 5, top + 8);
	printf("Папок");

	SetCursorPosition(left + 5, top + 10);
	printf("Файлов");

	SetCursorPosition(left + 5, top + 12);
	printf("Размер папки");


	std::thread countFF(CountFileFolder, fCrnt->file.name, &countFile, &countFolder, &sizeFolder);
	countFF.detach();
	unsigned int tmp = countFile;
	unsigned int tmp1 = countFolder;
	unsigned int tmp3 = sizeFolder;
	int key = 0;
	float tmp2 = 0;
	unsigned int kb = 0, mb = 0, gb = 0;
	do
	{
	
		SetCursorPosition(left + 5, top + 11);
		printf("%d", countFile);
		tmp = countFile;
		SetCursorPosition(left + 5, top + 9);
		printf("%d", countFolder);
		SetCursorPosition(left + 5, top + 13);
		printf("                           ");
		tmp1 = countFolder;
		if (sizeFolder / 1024)
		{
			kb += sizeFolder / 1024;
			sizeFolder %= 1024;
		}
		if (kb / 1024)
		{
			mb += kb / 1024;
			kb %= 1024;
		}
		if (mb / 1024)
		{
			gb += mb / 1024;
			mb %= 1024;
		}
		if (kb || mb || gb)
		{
			if (mb || gb)
			{
				if (gb)
				{
					SetCursorPosition(left + 5, top + 13);
					tmp2 = gb + (float)mb / 1024;
					printf("%.1fGB", tmp2);

				}
				else
				{
					SetCursorPosition(left + 5, top + 13);
					tmp2 = mb + (float)kb / 1024;
					printf("%.1fMB", tmp2);
				}
			}
			else
			{
				SetCursorPosition(left + 5, top + 13);
				tmp2 = kb + (float)sizeFolder / 1024;
				printf("%.1fKB", tmp2);
			}
		}
		else
		{
			SetCursorPosition(left + 5, top + 13);
			printf("%dB", sizeFolder);
		}
		tmp3 = sizeFolder;
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	} while ((tmp != countFile) || (tmp1 != countFolder) || (tmp3 != sizeFolder));
	SetCursorPosition(left + 5, top + 11);
	printf("%d", countFile);
	SetCursorPosition(left + 5, top + 9);
	printf("%d", countFolder);
	if (sizeFolder / 1024)
	{
		kb += sizeFolder / 1024;
		sizeFolder %= 1024;
	}
	if (kb / 1024)
	{
		mb += kb / 1024;
		kb %= 1024;
	}
	if (mb / 1024)
	{
		gb += mb / 1024;
		mb %= 1024;
	}
	if (kb || mb || gb)
	{
		if (mb || gb)
		{
			if (gb)
			{
				SetCursorPosition(left + 5, top + 13);
				tmp2 = gb + (float)mb / 1024;
				printf("%.1fGB", tmp2);

			}
			else
			{
				SetCursorPosition(left + 5, top + 13);
				tmp2 = mb + (float)kb / 1024;
				printf("%.1fMB", tmp2);
			}
		}
		else
		{
			SetCursorPosition(left + 5, top + 13);
			tmp2 = kb + (float)sizeFolder / 1024;
			printf("%.1fKB", tmp2);
		}
	}
	else
	{
		SetCursorPosition(left + 5, top + 13);
		printf("%dB", sizeFolder);
	}
	_getch();
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}
void FileInfo(files * fCrnt)
{
	_fsize_t size = fCrnt->file.size;
	CHAR_INFO *chiBuffer = new CHAR_INFO[40 * 20];
	short top = ConsoleSize.Y / 2 - 10;
	short bottom = ConsoleSize.Y / 2 + 10;
	short left = ConsoleSize.X / 2 - 20;
	short right = ConsoleSize.X / 2 + 20;
	showWindow(&chiBuffer, top, left, bottom, right, Cyan);
	SetCursorPosition(left + 10, top + 1);
	printf("Свойства файла");
	SetCursorPosition(left + 5, top + 2);
	printf("Название файла");
	SetCursorPosition(left + 5, top + 3);
	if (strlen(fCrnt->file.name) < 34)
	{
		printf("%s", fCrnt->file.name);
	}
	else
	{
		for (int i = 0; i < 32; ++i)
			printf("%c", fCrnt->file.name[i]);
		printf("..");
	}
	SetCursorPosition(left + 5, top + 4);
	printf("Дата создания");
	SetCursorPosition(left + 5, top + 5);
	printf("%s", asctime((localtime(&fCrnt->file.time_create))));
	SetCursorPosition(left + 5, top + 6);
	printf("Дата изменения");
	SetCursorPosition(left + 5, top + 7);
	printf("%s", asctime((localtime(&fCrnt->file.time_write))));
	SetCursorPosition(left + 5, top + 8);
	printf("Размер файла");
	unsigned int kb = 0, mb = 0, gb = 0;
	float tmp2 = 0;
	if (size / 1024)
	{
		kb += size / 1024;
	}
	if (kb / 1024)
	{
		mb += kb / 1024;
		kb %= 1024;
	}
	if (mb / 1024)
	{
		gb += mb / 1024;
		mb %= 1024;
	}
	if (kb || mb || gb)
	{
		if (mb || gb)
		{
			if (gb)
			{
				SetCursorPosition(left + 5, top + 9);
				tmp2 = gb + (float)mb / 1024;
				printf("%.1fGB", tmp2);

			}
			else
			{
				SetCursorPosition(left + 5, top + 9);
				tmp2 = mb + (float)kb / 1024;
				printf("%.1fMB", tmp2);
			}
		}
		else
		{
			SetCursorPosition(left + 5, top + 9);
			tmp2 = kb + (float)fCrnt->file.size / 1024;
			printf("%.1fKB", tmp2);
		}
	}
	else
	{
		SetCursorPosition(left + 5, top + 9);
		printf("%dB", fCrnt->file.size);
	}
	_getch();
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
}