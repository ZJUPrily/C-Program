#ifndef SAVEANDOPEN_H
#define SAVEANDOPEN_H
#include "windows.h"
BOOL DoFileOpenSave(HWND hwnd, BOOL bSave);
BOOL DoFileSave(HWND hwnd);
BOOL SaveFile(HWND hEdit, LPSTR pszFileName);
BOOL LoadFile(HWND hEdit, LPSTR pszFileName);
#endif

