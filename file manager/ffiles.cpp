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
void showStr(char *FileName, _fsize_t FileSize, unsigned int attrib, int x, int y, bool newlist)
{
	if ((!newlist) || (y == 2))
	{
		SetCursorPosition(1, y);
		printf("%s", clearStr);
	}
	SetCursorPosition(1, y);
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
	}
	SetCursorPosition(5 * ConsoleSize.X / 6, y);

	if (attrib & _A_SUBDIR)
		printf("FOLDER");
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

	}
}
void show(files *first)
{
	if (!loadConsoleFrame(frameFile)) ConsoleFrame();
	files *showing = first;
	int i = ConsoleSize.Y - 5;
	SetColor(Cyan, White);
	showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X, ConsoleSize.Y - 3 - i, TRUE);
	showing = showing->next;
	i--;
	SetColor(Blue, White);
	while ((showing && i))
	{
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X, ConsoleSize.Y - 3 - i, TRUE);
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
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X, ConsoleSize.Y - 3 - i, TRUE);
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
			showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X, ConsoleSize.Y - 3 - i, TRUE);
			showing = showing->next;
			i--;
		}
		SetColor(Cyan, White);
		showStr(showing->file.name, showing->file.size, showing->file.attrib, ConsoleSize.X, ConsoleSize.Y - 4, FALSE);
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
	showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.X, ConsoleSize.Y - 4, FALSE);
}
void deleteElement(files ** fCrnt)
{
	files *del = *fCrnt;
	(*fCrnt)->prev->next = (*fCrnt)->next;
	(*fCrnt)->next->prev = (*fCrnt)->prev;
	*fCrnt = (*fCrnt)->prev;
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
	if (strcmp(message, "��������� ��������"))
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
	for (i = 0; (*str)[i]; ++i);
	if (i > 256)
	{
		showError("���� � ����� ������ ��� ����������", "� ��� ��� ������� �������");
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
void newFolder(files **flast, files * fCrnt, int *CrntStr)
{
	int i = 1;
	char str1[] = "����� �����";
	if (!_mkdir(str1))
	{
		renameWindow(str1);
		addFolder(flast, str1, fCrnt, CrntStr);
	}
	char str[25];
	while (1)
	{
		sprintf(str, "%s(%d)", str1, i);
		if (!_mkdir(str)) break;
		i++;
	}
	renameWindow(str);
	addFolder(flast, str, fCrnt, CrntStr);

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
			showError("� ��� �� ���� � �������� ������ �����", "");
			break;
		case EBUSY:
			showError("������ ����� �������", "�������� �� ��� ��������");
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
void FolderCopy(char *path, char *fCopy)
{
	if (_mkdir(fCopy) == -1)
	{
		showError("������ �������� �����", "");
		return;
	}
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
			else showError(myfile.name, "������ ���� �� ����� ���� ����������");
		}
		delete[] pathCopy;
	}
	_findclose(p);
	_chdir("..");
}

bool addFolder(files ** flast, char * FileName, files *fCrnt, int *CrntStr)
{
	files *current = *flast;
	int i = 0;
	bool b, c;
	while (1)
	{
		b = false;
		c = false;
		if ((unsigned)FileName[0] < (unsigned)current->file.name[0])
			break;
		if ((unsigned)FileName[0] > (unsigned)current->file.name[0])
		{
			current = current->next;
			continue;
		}
		for (int j = 1; j<=i; j++)
		{
			if ((unsigned)FileName[j] < (unsigned)current->file.name[j])
			{
				b = true;
				break;
			}
			if ((unsigned)FileName[j] > (unsigned)current->file.name[j])
			{
				current = current->next;
				c = true;
				break;
			}
		}
		if (b) break;
		if (c) continue;
		do
		{
			i++;
			if ((unsigned)FileName[i] < (unsigned)current->file.name[i])
			{
				b = true;
				break;
			}
			if ((unsigned)FileName[i] > (unsigned)current->file.name[i])
			{
				current = current->next;
				c = true;
				break;
			}
		} while (FileName[i]);
		if (c) continue;
		if (b) break;
		if ((unsigned)FileName[i] == (unsigned)current->file.name[i]) return 0;
	}
	files *add = new files;
	add->file.attrib = _A_SUBDIR;
	add->file.size = 0;
	for (i = 0; FileName[i]; i++)
		add->file.name[i] = FileName[i];
	add->file.name[i] = '\0';
	if (current->prev) 
	add->prev = current->prev;
	add->next = current;
	if (current->prev) current->prev->next = add;
	current->prev = add;
	files *list = *flast;
	while ((list != current) && (list != fCrnt)) 
		list = list->next;
	i = 0;
	list = list->next;
	while ((list != current) && (list != fCrnt)) {
		list = list->next;
		i++;
	}
	SetColor(Blue, White);
	if ((list == current) && (i + *CrntStr < ConsoleSize.Y - 5))
	{
		addBlockDown(i + *CrntStr + 2);
		showStr(FileName, 0, _A_SUBDIR, ConsoleSize.X, *CrntStr + 2 + i, FALSE);
	}
	if ((list == fCrnt) && (*CrntStr + 1 - i > 2	))
	{
		addBlockDown(*CrntStr + 1 - i);
		showStr(FileName, 0, _A_SUBDIR, ConsoleSize.X, *CrntStr + 1 - i, FALSE);
		(*CrntStr)++;
	}
	return 1;
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
	printf("�������� �����");
	SetCursorPosition(left + 5, top + 2);
	printf("�������� �����");
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
	printf("���� ��������");
	SetCursorPosition(left + 5, top + 5);
	printf("%s", asctime((localtime(&fCrnt->file.time_create))));
	SetCursorPosition(left + 5, top + 6);
	printf("���� ���������");
	SetCursorPosition(left + 5, top + 7);
	printf("%s", asctime((localtime(&fCrnt->file.time_write))));
	SetCursorPosition(left + 5, top + 8);
	printf("�����");

	SetCursorPosition(left + 5, top + 10);
	printf("������");

	SetCursorPosition(left + 5, top + 12);
	printf("������ �����");


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
	printf("�������� �����");
	SetCursorPosition(left + 5, top + 2);
	printf("�������� �����");
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
	printf("���� ��������");
	SetCursorPosition(left + 5, top + 5);
	printf("%s", asctime((localtime(&fCrnt->file.time_create))));
	SetCursorPosition(left + 5, top + 6);
	printf("���� ���������");
	SetCursorPosition(left + 5, top + 7);
	printf("%s", asctime((localtime(&fCrnt->file.time_write))));
	SetCursorPosition(left + 5, top + 8);
	printf("������ �����");
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