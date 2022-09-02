#include "Menu.h"
#include "TextEditor.h"
#include "Init.h"
#include "MenuFunction.h"
#include "SaveAndOpen.h"
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//�����һ���ַ����е�λ��
extern int MouseInBox;
extern int isDrawCondition;
extern HWND hwnd;//������ָ�룬�ڱ����ʱ������
extern void addTime();
extern void DoBuildNewText();
static int isPlaySound = 0;

void ShowMenu()
{
	double fH = GetFontHeight();
	double x = 0; 
	double y = GetWindowHeight();
	double h = fH * 1.5; // �ؼ��߶�
	static double w = 0.5; // �ؼ����
	static double wlist = 1.35;
	int    selection;

	// �ļ��˵�
	static char * menuListFile[] = { "�ļ�",
		"�½�   | Ctrl-A",
		"��   | Ctrl-B", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"����   | Ctrl-C",
		"���Ϊ | Ctrl-D",
		"�˳�   | Ctrl-E",
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
		isexit=MessageBox(hwnd, "�����Ҫ�˳���", "��ʾ", MB_OKCANCEL);
		if (isexit == 1)
		{
			exit(0);
		}
	}
	
	//�༭�˵�
	static char * menuListEdit[] = { "�༭",
		"ˢ�� | Ctrl-F",
		"���� | Ctrl-G",
		"�滻 | Ctrl-H",
		"���� | Ctrl-I",
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

	//��ʽ�˵�
	static char * menuListStyle[] = { "��ʽ",
		"����   | Ctrl-J",
		"�Ӵ�/б��| Ctrl-K",
		"�ֺ�+  | Ctrl-L",
		"�ֺ�-  | Ctrl-M",
		"C����   | Ctrl-N", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"�˵�ɫ | Ctrl-O",
		"����ɫ | Ctrl-P",
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

	//�鿴�˵�
	static char * menuListLook[] = { "�鿴",
		"״̬�� | Ctrl-Q",
		"����   | Ctrl-R", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"��������| Ctrl-S"
	};
	selection = menuList(GenUIID(0), x + 0.5 * 3, y - h, w, wlist, h, menuListLook, sizeof(menuListLook) / sizeof(menuListLook[0]));
	switch (selection)
	{
	case 1:
		isDrawCondition = !isDrawCondition;
		break;
	case 2:
		MessageBox(NULL, 
			"C��̴���ҵ�����±�\n�汾��1.0.1\n��Ȩ����\n��F5ˢ��\nʹ�÷�ʽ������򱨸�"
			, "����", MB_OK);
		break;
	case 3:
		isPlaySound = !isPlaySound;
		if (isPlaySound)
		{
			PlaySound(TEXT("����ɱ��½����.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		else
		{
			PlaySound(NULL, NULL, SND_FILENAME);
		}
		break;
	}

}