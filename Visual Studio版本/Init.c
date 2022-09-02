#include "Init.h"
#include "Menu.h"
#include "TextEditor.h"
#include "MenuFunction.h"
#include "extgraph.h"
#include "graphics.h"

extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
extern char now_result[MAXSTRINGLENTH];

void Display()
{
	// 清屏
	DisplayClear();
	// 绘制和处理菜单
	ShowBackgroundcolor();
	ShowMenu();
	SetPenColor("black");
	DrawTextEditor();
	DrawCondition(isDrawCondition);
}
void Init()
{
	MyString = NULL;
	InitGraphics();
	DefineColor("BackRed", 1, 0.75, 0.75);
	DefineColor("BackYellow", 1, 1, 0.75);
	DefineColor("BackBlue", 0.75, 0.75, 1);
	DefineColor("BackGreen", 0.75, 1, 0.75);
	DefineColor("BackPurple", 1, 0.75, 1);
	usePredefinedMenuColors(4);
	SetWindowTitle("C大程大作业");
	SetFont("微软雅黑");
	SetPointSize(15);
	InitGUI();
	DrawTextEditor();
	strcpy(result, "");
	DrawCondition(isDrawCondition);
	SignalPosition = 0;
	MouseInBox = 0;
	isDrawCondition = 1;
	ShowMenu();
	DrawCondition(isDrawCondition);
}
