#pragma once
#include <Windows.h>
#include <direct.h>
#include <io.h>  
#include <stdio.h> 
#include <stdlib.h>
unsigned int char2int_(char chr);
void selectHEX(int CrntStr, int CrntStl, bool select);
void readBlockDownHEX();
void readBlockUpHEX();
void showStrHEX(FILE * fHex, unsigned int adress, int CrntStr, int CharCount);
void showAllHEX(FILE * fHex, int adress);
void runHEX(char * FileName, _fsize_t FileSize);
void preOption();
void searchHEX(FILE * HexFile);