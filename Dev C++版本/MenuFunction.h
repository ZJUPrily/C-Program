#ifndef MENUFUNCTION_H
#define MENUFUNCTION_H
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"
#include "extgraph.h"
#include "gcalloc.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <time.h>
#include "imgui.h"
#include "extgraph.h"
#include "graphics.h"
/*��������˲˵�������Ĳ��ң��滻��cģʽ����*/
void ShowStringFind();
void f_CharEventProcess(char ch);
void f_MouseEventProcess(int x, int y, int button, int event);
void HighLight();
void ShowStringCompare();//����charevent,mouseevent����
void f2_MouseEventProcess(int x, int y, int button, int event);
void Replace();
void RunC(); //��ȡnow_result�������ʵĸ��ģ�ֻʹ��һ����ɫ
void refreshF5();
void ShowBackgroundcolor();
void ChangeBackgroundColor();
void SetBoldOrItalic();
void DrawCondition(int isDrawCondition);
void addTime();
void DoBuildNewText();
#endif

