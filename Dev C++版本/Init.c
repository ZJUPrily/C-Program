#include "Init.h"
#include "Menu.h"
#include "TextEditor.h"
#include "MenuFunction.h"
#include "extgraph.h"
#include "graphics.h"

extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//�����һ���ַ����е�λ��
extern int MouseInBox;
extern int isDrawCondition;
extern char now_result[MAXSTRINGLENTH];

void Display()
{
	// ����
	DisplayClear();
	// ���ƺʹ���˵�
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
	SetWindowTitle("C��̴���ҵ");
	SetFont("΢���ź�");
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
