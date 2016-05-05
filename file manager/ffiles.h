#pragma once
#include "Globals.h"
extern files flist, slist;
void addFiles(_finddata_t a, files ** b);
void saveClearStr();
void showStr(char * FileName, _fsize_t FileSize, unsigned int attrib, int x, int y, bool newlist);
void show(files * first);
void show(files * first, int *CrntStr);
void showLastStr(files *fCrnt, int CrntStr);
void deleteElement(files ** fCrnt);
void deleteAll(files ** flast);
void sortAlph(files ** flast);
void sortAlph(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void searchFiles(files ** flast);
void searchFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void FileCopy(FILE * source, FILE * dist);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void addLog(char * message, char * typemessage, char * extramessage = "" );
void getLogPath(const char * argv[]);
void ExistFile(char(*str)[260]);
void newFolder();
void deleteFolder(char * path);
void FolderCopy(char * path, char * fCopy);
bool addFolder(files **flast, char * FileName);

void CountFileFolder(char * FolderPath, unsigned int * countFile, unsigned int * countFolder);

