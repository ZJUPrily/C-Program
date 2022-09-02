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
int SignalPosition;//光标在一行字符串中的位置
int MouseInBox;
int isDrawCondition;
extern int lineStr[SHORTERSTRINGLENTH];
void Main()
{
	Init();
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	registerCharEvent(CharEventProcess);       //字符
	registerTimerEvent(TimeEventProcess); //定时器
	startTimer(1, 500);
	startTimer(3, 100);
	//startTimer(4, 1);
	//SetPenColor("red");
	//SetPenSize(0.2);
	//MovePen(0.2, 0.2);
	//DrawLine(0.1, 0.1);
	//Display(); // 刷新显示	
	//for (int i = 0; i < 100; i++)
	//	lineStr[i] = 0;

}