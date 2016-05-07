#pragma once
#include "Globals.h"
extern files flist, slist;
void addFiles(_finddata_t a, files ** b);
void saveClearStr();
void showStr(char * FileName, _fsize_t FileSize, unsigned int attrib, int y, bool newlist);
void show(files * first);
void show(files * first, int *CrntStr);
void showLastStr(files *fCrnt, int CrntStr);
void deleteElement(files ** fCrnt);
void deleteAll(files ** flast);
void sortAlph(files ** flast);
int sortAlph(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName);
void sortAlph(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void searchFiles(files ** flast);
int searchFiles(files ** flast, files ** fCrnt, files ** addFile, char **crntName, char ** addName);
void searchFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void FileCopy(FILE * source, FILE * dist);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt);
void RefreshFiles(files ** flast, int * CrntStr, files ** fCrnt, char ** prevPath);
void addLog(char * message, char * typemessage, char * extramessage = "" );
void getLogPath(const char * argv[]);
void ExistFile(char(*str)[260]);
void newFolder(files **flast, files ** fCrnt, int *CrntStr);
void deleteFolder(char * path);
void FolderCopy(char * path, char * fCopy);
void addFolder(files ** flast, files **fCrnt, char * crntName, char * FileName, int *CrntStr);

void CountFileFolder(char * FolderPath, unsigned int * countFile, unsigned int * countFolder, unsigned int *sizeFolder);

void FolderInfo(files * fCrnt);

void FileInfo(files * fCrnt);

