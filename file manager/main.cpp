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
#include "fhex.h"
#pragma pack(1)
                               
using namespace std;
 char *logFile, *frameFile, *clearStr;
 COORD ConsoleSize = { 122,40 };

 void SelectStr(int a)
 {
 	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { 1, a + 2 };
 	SetConsoleCursorPosition(hConsole, position);
 }
void savePath(char *FileName, char CrntPath[260])
{
	if ((FileName[0] == '.') && (FileName[1] == '.') && (FileName[2] == '\0'))
	{
		for (int i = 0; i < MAX_PATH; ++i) CrntPath[i] = '\0';
		_getcwd(CrntPath, MAX_PATH);
		if (CrntPath[3]) sprintf(CrntPath, "%s\\", CrntPath);
	}
	else sprintf(CrntPath, "%s%s\\", CrntPath, FileName);
}
void showPath(char CrntPath[260])
{
	if (CrntPath[ConsoleSize.X])
	{
		SetCursorPosition(0, ConsoleSize.Y - 2);
		SetColor(Black, White);
		for (int i = 0; i < ConsoleSize.X - 2; ++i) printf("%c", CrntPath[i]);
		printf("..");
	}
	else
	{
		SetCursorPosition(0, ConsoleSize.Y - 2);
		SetColor(Black, White);
		for (int i = 0; i < ConsoleSize.X; ++i) printf(" ");
		SetCursorPosition(0, ConsoleSize.Y - 2);
		printf("%s", CrntPath);
	}
}
bool exitFM()
{
	short top = ConsoleSize.Y / 2 - 3;
	short bottom = ConsoleSize.Y / 2 + 2;
	short left = ConsoleSize.X / 2 - 31;
	short right = ConsoleSize.X / 2 + 31;
	CHAR_INFO *chiBuffer = new CHAR_INFO[7 * 62];
	showWindow(&chiBuffer, top, left, bottom, right, Green);
	SetCursorPosition(left + 20, top + 1);
	printf("Вы хотите выйти?");
	SetCursorPosition(left + 8, top + 3);
	SetColor(White, Green);
	printf("  Да  ");
	SetCursorPosition(left + 48, top + 3);
	SetColor(Green, White);
	printf("  Нет  ");
	bool yes = TRUE;
	unsigned char key;
	do
	{
		key = _getch();
		if (key == 224) 
		{
			key = _getch();
			switch (key)
			{
			case 75: yes = TRUE;
				break;
			case 77: yes = FALSE;
				break;
			default:
				break;
			}
		}
		if (key == 27)
		{
			yes = false;
			break;
		}
		if (yes)
		{
			SetCursorPosition(left + 8, top + 3);
			SetColor(White, Green);
			printf("  Да  ");
			SetCursorPosition(left + 48, top + 3);
			SetColor(Green, White);
			printf("  Нет  ");
		}
		else
		{
			SetCursorPosition(left + 8, top + 3);
			SetColor(Green, White);
			printf("  Да  ");
			SetCursorPosition(left + 48, top + 3);
			SetColor(White, Green);
			printf("  Нет  ");
		}
	} while (key != 13);
	hideWindow(chiBuffer, top, left, bottom, right);
	delete[] chiBuffer;
	return yes;

}
void preOptions(const char *argv[])
{
	getLogPath(argv);
	getFramePath(argv);
	SetBufferSize();
	saveClearStr();
	if (!fopen(frameFile, "r"))
	{
		ConsoleFrame();
		saveConsoleToFile(frameFile);
	}
	
	SetConsoleTitle(L"File Manager");
	addLog("Программа запущена", "INFO");
	EnableCursor(false);
	_chdir("C:\\");
	setlocale(LC_ALL, "rus");
}

int main(int argc, const char * argv[]) 
	{
		preOptions(argv);                             
		//ConsoleFrame();
		//saveConsoleToFile("ConsoleFrame.txt");
		bool Disk[26], error;
		unsigned int fCount;
		char buffer[260], fCopy[260], pathCopy[MAX_PATH], CrntPath[MAX_PATH] = "C:\\";
		pathCopy[0] = '\0';
		int CrntStr = 0, key, CrntFile = 0;
		FILE *source = NULL, *dist;
		files *flast = flist.next;
		searchFiles(&flast, &fCount);
		files *fCrnt = flast;
		show(flast, 0, FALSE);
		showPath(CrntPath);
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
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.X, ConsoleSize.Y - 4, FALSE);
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
						showStr(fCrnt->file.name, fCrnt->file.size, fCrnt->file.attrib, ConsoleSize.X, 2, FALSE);
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
				break;
			case 13:
				if (fCrnt->file.attrib & _A_SUBDIR)
				{
					if (!(fCrnt->file.attrib & _A_SYSTEM))
					{
						_chdir(fCrnt->file.name);
						savePath(fCrnt->file.name, CrntPath);
						addLog("Перешли в папку", "INFO", fCrnt->file.name);
						RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
						showPath(CrntPath);
					}
					else showError("У вас нет доступа к данной папке", "");
				}
				else
				{
					if (!(fCrnt->file.attrib & _A_SYSTEM))
					{
						runHEX(fCrnt->file.name, fCrnt->file.size);
						addLog(fCrnt->file.name, "INFO", "Открыт в HEX редакторе");
					}
					else showError("У вас нет доступа к данному файлу", "");
				}
				break;
			case 8:
				_chdir("..");
				savePath("..", CrntPath);
				RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
				showPath(CrntPath);
				break;
			case 0:
				key = _getch();
				switch (key)
				{
				case 59: 
					showError("Вы нажали не на ту клавишу", "О БОЖЕ!!!");
					addLog("Нажата клавиша ошибки", "ERROR");
					break;
				case 60:
					renameWindow(fCrnt->file.name);
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					break;
				case 61:
					if (source != NULL) fclose(source);
					pathCopy[0] = '\0';
					memcpy(fCopy, fCrnt->file.name, 260);
					if (fCrnt->file.attrib & _A_SUBDIR)
					{
						if (!(fCrnt->file.attrib & _A_SYSTEM))
						{
							_chdir(fCopy);
							_getcwd(pathCopy, MAX_PATH);
							_chdir("..");
						}
						else showError("У вас нет доступа к копированию данной папки", "");
					}
					else
					{
						source = fopen(fCopy, "r+b");
						if (source != NULL)  addLog(fCopy, "COPY", "Добавлен в буфер");
						else
						{
							addLog("Ошибка открытия файла", "ERROR");
							showError("Файл не может быть скопирован", "");
						}
					}
					break;
				case 62:
					if (source != NULL)
					{
						dist = fopen(fCopy, "rb");
						memcpy(buffer, fCopy, 260);
						if (dist != NULL)
						{
							fclose(dist);
							ExistFile(&fCopy);
						}
						dist = fopen(fCopy, "wb");
						FileCopy(source, dist);
						fclose(dist);
						memcpy(fCopy, buffer, 260);
						addLog(fCopy, "COPY", "Успешно скопирован");
					}
					if (pathCopy[0])
					{
						error = TRUE;
						for (int i = 0; pathCopy[i]; i++)
						{
							if (pathCopy[i] != CrntPath[i])
							{
								FolderCopy(pathCopy, fCopy);
								error = FALSE;
								break;
							}
						}
						if (error) showError("Нельзя копировать папку в дочернюю", "");

					}
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					addLog(fCopy, "COPY", "Успешно скопирован");
					break;
				case 63:
					newFolder();
					addLog("Созданна новая папка", "INFO");
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					break;
				case 64:
					listDisk(Disk);
					chooseDisk(Disk);
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					break;
				case 65:
					huffman(fCrnt->file.name);
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					break;
				case 66: 
					unhuffman(fCrnt->file.name);
					RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					break;
				case 67:
					if ((fCrnt->file.name[0] == '.') && (fCrnt->file.name[1] == '.') && (fCrnt->file.name[2] == '\0')) showError("Путь назад невозможно удалить", "");
					else
					{
						if (fCrnt->file.attrib & _A_SUBDIR)
						{
							deleteFolder(fCrnt->file.name);
							addLog(fCrnt->file.name, "INFO", "Удалена");
						}																			
						else
							if (remove(fCrnt->file.name) == -1) showError("Данный файл не может быть удален", "");
							else addLog(fCrnt->file.name, "INFO", "Удален");
							RefreshFiles(&flast, &fCount, &CrntStr, &CrntFile, &fCrnt);
					}
				default:
					break;
				}
				break;
			case 27:
				if (!exitFM()) key = 1;
			default:
				break;
			}
		} while (key != 27);
		if (source != NULL) fclose(source);
		addLog("Программа выключена","INFO");
		delete[] logFile;
		delete[] clearStr;
		delete[] frameFile;
		return 0;
	}
