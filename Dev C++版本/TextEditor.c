#include "TextEditor.h"
extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
extern int totalChar;
extern int lineCount;
extern int lineFeedAuto;
extern int lineStr[MAXSTRINGLENTH];



void DrawTextEditor()
{
	//add new
	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = GetWindowHeight();
	double h = fH * 1.5; // 控件高度
	double w = TextStringWidth("殷思慜"); // 控件宽度
	double wlist = TextStringWidth("蓝田学业发展中心")*1.2;
	double xindent = GetWindowHeight() / 20; // 缩进
	SetPenColor("black");
	//
	MovePen(0.1, 0.5);
	DrawLine(0, GetWindowHeight() - 0.9);
	DrawLine(GetWindowWidth() - 0.2, 0);
	DrawLine(0, -GetWindowHeight() + 0.9);
	DrawLine(-GetWindowWidth() + 0.2, 0);
}
void SetFontStyle()
{
	char* FontStyle[] = { "等线","宋体","黑体","楷体","仿宋","隶书","Arial","Times New Roman","Contlas" };
	static int select = 0;
	SetFont(FontStyle[select]);
	select++;
	if (select == 6) select = 0;
	Display();
}
void SetMenuStyle()
{
	static int select = 4;
	select++;
	if (select == 7) select = 0;
	usePredefinedMenuColors(select);
	Display();
}
void SetFontSize(int add)
{
	static int fontsize = 15;
	if (add) fontsize++;
	else if (fontsize > 0) fontsize--;
	SetPointSize(fontsize);
	Display();
}