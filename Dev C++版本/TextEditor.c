#include "TextEditor.h"
extern struct Linestring* MyString;
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;//�����һ���ַ����е�λ��
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
	double h = fH * 1.5; // �ؼ��߶�
	double w = TextStringWidth("��˼�O"); // �ؼ����
	double wlist = TextStringWidth("����ѧҵ��չ����")*1.2;
	double xindent = GetWindowHeight() / 20; // ����
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
	char* FontStyle[] = { "����","����","����","����","����","����","Arial","Times New Roman","Contlas" };
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