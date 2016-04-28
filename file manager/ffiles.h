#pragma once
#include "Globals.h"
extern files flist, slist;
void addFiles(_finddata_t a, files ** b);
void add(_finddata_t a, files ** b);
void saveClearStr();
void showStr(char * FileName, _fsize_t FileSize, unsigned int attrib, int x, int y, bool newlist);
void show(files * first, int a, bool b);
void deleteAll(files ** flast);
void sortAlph(files ** flast);
void searchFiles(files ** flast, unsigned int * a);
void FileCopy(FILE * source, FILE * dist);
void RefreshFiles(files ** flast, unsigned int * fCount, int * CrntStr, int * CrntFile, files ** fCrnt);
void addLog(char * message, char * typemessage, char * extramessage = "" );
void getLogPath(const char * argv[]);
void ExistFile(char(*str)[260]);
void newFolder();
void deleteFolder(char * path);
void FolderCopy(char * path, char * fCopy);
