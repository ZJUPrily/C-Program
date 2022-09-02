#include "Menu.h"
#include "TextEditor.h"
#include "Init.h"
#include "MenuFunction.h"
#include "SaveAndOpen.h"
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
extern HWND hwnd;//主窗口指针，在保存的时候有用
extern void addTime();
extern void DoBuildNewText();
static int isPlaySound = 0;

void ShowMenu()
{
	double fH = GetFontHeight();
	double x = 0; 
	double y = GetWindowHeight();
	double h = fH * 1.5; // 控件高度
	static double w = 0.5; // 控件宽度
	static double wlist = 1.35;
	int    selection;

	// 文件菜单
	static char * menuListFile[] = { "文件",
		"新建   | Ctrl-A",
		"打开   | Ctrl-B", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"保存   | Ctrl-C",
		"另存为 | Ctrl-D",
		"退出   | Ctrl-E",
	};
	int isexit;
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	switch (selection)
	{
	case 1:
		Init();
		DoBuildNewText();
		break;
	case 2:
		DoFileOpenSave(hwnd, FALSE);
		break;
	case 3:
		DoFileSave(hwnd);
		break;
	case 4:
		DoFileOpenSave(hwnd, TRUE);
		break;
	case 5:
		isexit=MessageBox(hwnd, "你真的要退出吗", "提示", MB_OKCANCEL);
		if (isexit == 1)
		{
			exit(0);
		}
	}
	
	//编辑菜单
	static char * menuListEdit[] = { "编辑",
		"刷新 | Ctrl-F",
		"查找 | Ctrl-G",
		"替换 | Ctrl-H",
		"日期 | Ctrl-I",
	};
	selection = menuList(GenUIID(0), x+0.5, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
	switch (selection)
	{
	case 1:
		refreshF5();
		break;
	case 2:
		ShowStringFind();
		break;
	case 3:
		ShowStringCompare();
		break;
	case 4:
		addTime();
		break;
	}

	//格式菜单
	static char * menuListStyle[] = { "格式",
		"字体   | Ctrl-J",
		"加粗/斜体| Ctrl-K",
		"字号+  | Ctrl-L",
		"字号-  | Ctrl-M",
		"C代码   | Ctrl-N", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"菜单色 | Ctrl-O",
		"背景色 | Ctrl-P",
	};
	selection = menuList(GenUIID(2), x + 2 * w, y - h, w, wlist, h, menuListStyle, sizeof(menuListStyle) / sizeof(menuListStyle[0]));
	switch (selection)
	{
	case 1:
		SetFontStyle();
		break;
	case 2:
		SetBoldOrItalic();
		break;
	case 3:
		SetFontSize(1);
		wlist += 0.1;
		break;
	case 4:
		SetFontSize(0);
		wlist -= 0.1;
		break;
	case 5:
		RunC();
		break;
	case 6:
		SetMenuStyle();
		break;
	case 7:
		ChangeBackgroundColor();
		break;
	}

	//查看菜单
	static char * menuListLook[] = { "查看",
		"状态栏 | Ctrl-Q",
		"关于   | Ctrl-R", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"背景音乐| Ctrl-S"
	};
	selection = menuList(GenUIID(0), x + 0.5 * 3, y - h, w, wlist, h, menuListLook, sizeof(menuListLook) / sizeof(menuListLook[0]));
	switch (selection)
	{
	case 1:
		isDrawCondition = !isDrawCondition;
		break;
	case 2:
		MessageBox(NULL, 
			"C大程大作业：记事本\n版本：1.0.1\n版权所有\n按F5刷新\n使用方式见大程序报告"
			, "关于", MB_OK);
		break;
	case 3:
		isPlaySound = !isPlaySound;
		if (isPlaySound)
		{
			PlaySound(TEXT("三国杀登陆音乐.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else
		{
			PlaySound(NULL, NULL, SND_FILENAME);
		}
		break;
	}

}