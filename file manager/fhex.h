#pragma once
#include <Windows.h>
#include <direct.h>
#include <io.h>  
void selectHEX(int CrntStr, int CrntStl, bool select);
void readBlockDownHEX();
void readBlockUpHEX();
void runHEX(char * FileName, _fsize_t FileSize);
