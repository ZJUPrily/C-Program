#include "graphics.h"
#include "imgui.h"
#include "Init.h"
#include "Events.h"
#include "Menu.h"
#include "MenuFunction.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
struct Linestring* MyString;
char result[MAXSTRINGLENTH];
int SignalPosition;//�����һ���ַ����е�λ��
int MouseInBox;
int isDrawCondition;
extern int lineStr[SHORTERSTRINGLENTH];
void Main()
{
	Init();
	registerKeyboardEvent(KeyboardEventProcess);// ����
	registerMouseEvent(MouseEventProcess);      // ���
	registerCharEvent(CharEventProcess);       //�ַ�
	registerTimerEvent(TimeEventProcess); //��ʱ��
	startTimer(1, 500);
	startTimer(3, 100);
	//startTimer(4, 1);
	//SetPenColor("red");
	//SetPenSize(0.2);
	//MovePen(0.2, 0.2);
	//DrawLine(0.1, 0.1);
	//Display(); // ˢ����ʾ	
	//for (int i = 0; i < 100; i++)
	//	lineStr[i] = 0;

}