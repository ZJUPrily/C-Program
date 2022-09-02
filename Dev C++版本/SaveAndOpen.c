#include "SaveAndOpen.h"
#include "resource.h"
#include "Menu.h"
#include "MenuFunction.h"
extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
//加载文件
BOOL LoadFile(HWND hEdit, LPSTR pszFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF) {
			LPSTR pszFileText;
			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL) {
				DWORD dwRead;
				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL)) {
					pszFileText[dwFileSize] = 0; // Null terminator
					if (strcpy(result, pszFileText))
						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	refreshF5();
	return bSuccess;
}

//保存result[1000]中的字符串至txt文件
BOOL SaveFile(HWND hEdit, LPSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;
		dwTextLength = strlen(result);
		if (dwTextLength > 0)
		{
			LPSTR pszText;
			pszText = result;
			if (pszText != NULL)
			{
				DWORD dwWritten;
				if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}
//bsave是true表示另存为，false表示打开
BOOL DoFileOpenSave(HWND hwnd, BOOL bSave)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = " Text Files (*.txt)\0*.txt\0 C Source Files (*.c)\0*.c\0 C++ Source Files (*.cpp)\0*.cpp\0 Python Source Files (*.py)\0*.py\0 HTML Files (*.html)\0*.html\0 CSS Files (*.css)\0*.css\0 All Files (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";

	if (bSave)
	{
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		if (GetSaveFileName(&ofn))
		{
			if (!SaveFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))
			{
				MessageBox(hwnd, "Save file failed.", "Error", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	else
	{
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		if (GetOpenFileName(&ofn))
		{
			if (!LoadFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))
			{
				MessageBox(hwnd, "Load of file failed.", "Error", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	refreshF5();
	return TRUE;
}
//普通保存
BOOL DoFileSave(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];

	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&ofn))
	{
		if (!SaveFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))
		{
			MessageBox(hwnd, "Save file failed.", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	return TRUE;
}