/*
使用说明：支持中英文输入，点击界面显示光标，光标自动追踪，左右键可以移动光标，支持回车换行等功能
尚且存在的问题：
1、上下移动，无法准确定位到字符
2、光标无法定位到输入处
3、Delete键存在bug
*/


#include "Events.h"
#include "Init.h"
#include "Menu.h"
#include "time.h"
#include "TextEditor.h"
#include "StringFunction.h"
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;								//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
extern int NowListPos;									//当前result数组下标
extern int IsEng(char c);
extern void SignalJudge(double x, double y); 
void AddText(char AddStr);
extern char now_result[MAXSTRINGLENTH];
extern void myDrawText();								//StringFunction的输出函数
double mx = TXTFIRSTX, my = TXTFIRSTY;					//鼠标回调返回当前输入字符位置的光标
int cnOrEng[MAXSTRINGLENTH];										//记录第几个数是中还是英文
int lineStr[MAXSTRINGLENTH];										//记录每行的字符数
int nowChar = 1;										//记录当前字符的中英或者换行
int lineCount = 1;										//记录行数
static bool isDisplayGB = TRUE;							//光标闪烁函数
int maxPos = 0;			 								//记录下标的最大值，用于移动光标时的判断
int totalChar = 0;										//总字符数，F5刷新
static bool cnOnlyDoOnce = TRUE;						//暂时没用
char lineString[MAXSTRINGLENTH];									//暂时储存当前行字符,可能会在自动换行有用
int upAccess = 0;
int IsCtrlPressed = 0;
extern double choice[SHORTERSTRINGLENTH][2];


// 用户的键盘事件响应函数

void KeyboardEventProcess(int key, int event)
{
	if (key == VK_CONTROL && event == KEY_DOWN)
		IsCtrlPressed = 1;
	if (key == VK_CONTROL && event == KEY_UP)
		IsCtrlPressed = 0;
	double chWidth = 0;									//字符宽度	
	double chHeight = GetFontHeight();					//字符高度
	uiGetKeyboard(key, event);							// GUI获取键盘（不用管他，我们不用imgui的输入框功能）
		/*光标定位*/
	switch (event)
	{
		int i;
		char rightStr[2]; char leftStr[2];				//用于左右移动的TextStringWidth参数
		char rightChar; char leftChar;					//用于储存左右字符
	case KEY_DOWN:
		switch (key)
		{
		case VK_LEFT:
			if (NowListPos == 0)
				break;									//到左上角，不能再向左移动
			leftChar = result[NowListPos - 1];
			leftStr[0] = result[NowListPos - 1];
			leftStr[1] = '\0';
			if (IsEng(leftChar) == 1)
			{
				chWidth = TextStringWidth(leftStr);
				mx -= chWidth;							//光标向左移动，当前下标左移
				Display();
				myDrawText();							//刷新显示，因为左右移动如果不刷新会在之前的位置留下痕迹
			}
			else if (IsEng(leftChar) != 1)
			{
				mx -= 2 * CNFONTWIDTH;							//光标向左移动，当前下标左移
				Display();
				myDrawText();							//刷新显示，因为左右移动如果不刷新会在之前的位置留下痕迹
			}
			break;
		case VK_RIGHT:
			if (NowListPos >= maxPos)
				break;
			rightChar = result[NowListPos];
			rightStr[0] = result[NowListPos];
			rightStr[1] = '\0';

			if (IsEng(rightChar) == 1)
			{
				chWidth = TextStringWidth(rightStr);
				mx += chWidth;							//光标向右移动，当前下标右移
				Display();
				myDrawText();
			}
			else if (IsEng(rightChar) != 1)
			{
				mx += 2 * CNFONTWIDTH;							//光标向右移动，当前下标右移
				Display();
				myDrawText();
			}
			break;
		case VK_BACK: if (NowListPos == 0)	break;				//退格，光标左移,到左上角直接退出
			if (result[NowListPos - 1] == '\n' || result[NowListPos - 1] == '\r')
			{
				break;
			}
			else
			{
				leftChar = result[NowListPos - 1];
				leftStr[0] = result[NowListPos - 1];
				leftStr[1] = '\0';
				if (IsEng(leftChar) == 1)
				{
					chWidth = TextStringWidth(leftStr);
					mx -= chWidth;							//光标向左移动，当前下标左移
					Display();
					myDrawText();							//刷新显示，因为左右移动如果不刷新会在之前的位置留下痕迹
				}
				else if (IsEng(leftChar) != 1)
				{
					mx -= 2 * CNFONTWIDTH;							//光标向左移动，当前下标左移
					Display();
					myDrawText();									//刷新显示，因为左右移动如果不刷新会在之前的位置留下痕迹
				}
			}

			break;
		case VK_UP:
			if (NowListPos == 0) break;
			i = NowListPos;
			leftChar = result[i - 1];
			leftStr[0] = result[i - 1];
			leftStr[1] = '\0';

			if (my == TXTFIRSTY)								//到顶，向左移动到最左上角
			{
				mx = TXTFIRSTX;
				upAccess = 1;
				break;
			}
			else if (leftChar == '\r' || leftChar == '\n')		//最左，上移
			{
				my += chHeight;
				break;
			}
			else												//不是首行的一行中间，左移到行首
			{
				while (leftChar != '\n'&& leftChar != '\r')
				{
					if (IsEng(leftChar) == 1)
					{
						leftStr[0] = result[i - 1];
						leftStr[1] = '\0';
						chWidth = TextStringWidth(leftStr);
						mx -= chWidth;							//光标向右移动，当前下标右移
						Display();
						myDrawText();
						i--;
						leftChar = result[i - 1];
					}
					else if (IsEng(leftChar) != 1)
					{
						mx -= 2 * CNFONTWIDTH;							//光标向右移动，当前下标右移
						Display();
						myDrawText();
						i -= 2;
						leftChar = result[i - 1];
					}

				}
				break;
			}
			break;
		case VK_DOWN:
			i = NowListPos;
			rightChar = result[i];
			rightStr[0] = result[i];
			rightStr[1] = '\0';
			if (rightChar == '\n' || rightChar == '\r' || rightChar == 0)
			{
				mx = TXTFIRSTX;
				my -= chHeight;
				break;
			}
			while (rightChar != 0 && rightChar != '\n'&& rightChar != '\r')			//验证1.是否已经到文末；2.验证我按下键，有没有下一行
			{

				if (IsEng(rightChar) == 1)
				{
					rightStr[0] = result[i];
					rightStr[1] = '\0';
					chWidth = TextStringWidth(rightStr);
					mx += chWidth;							//光标向右移动，当前下标右移
					Display();
					myDrawText();
					i++;
					rightChar = result[i];
				}
				else if (IsEng(rightChar) != 1)
				{
					mx += 2 * CNFONTWIDTH;							//光标向右移动，当前下标右移
					Display();
					myDrawText();
					i += 2;
					rightChar = result[i];
				}
			}
			break;

		}
	case KEY_UP:
		break;
	}



	/*字符输入点定位*/
	switch (event)
	{
		char rightChar; char leftChar;					//用于储存左右字符
	case KEY_DOWN:
		switch (key)
		{
		case VK_LEFT:
			if (NowListPos == 0)
				break;								//到左上角，不能再向左移动
			leftChar = result[NowListPos - 1];
			if (leftChar == '\n' || leftChar == '\r')
			{
				break;
			}
			else if (IsEng(leftChar) == 1)
			{
				NowListPos--;
				lineStr[lineCount] --;
			}
			else if (IsEng(leftChar) != 1)
			{
				NowListPos--;
				NowListPos--;
				lineStr[lineCount] --;
			}

			break;
		case VK_RIGHT:
			if (NowListPos >= maxPos)
				break;
			rightChar = result[NowListPos];
			if (rightChar == '\n' || rightChar == '\r')
			{
				break;
			}
			else if (IsEng(rightChar) == 1)
			{
				NowListPos++;
				lineStr[lineCount] ++;
			}
			else if (IsEng(rightChar) != 1)
			{
				NowListPos++;
				NowListPos++;
				lineStr[lineCount] ++;
			}
			break;
		case VK_BACK:
			SetEraseMode(1);
			myDrawText();
			DeleteText(1);				//删除元素，用到StringFunction函数的DeleteText函数
			strcpy(result, now_result);
			SetEraseMode(0);
			myDrawText();
			break;
		case VK_DELETE: DeleteText(0);				//删除元素，用到StringFunction函数的DeleteText函数
			strcpy(result, now_result);
			break;
		case VK_UP:if (NowListPos == 0) break;
			leftChar = result[NowListPos - 1];
			if (my == TXTFIRSTY && upAccess == 1)								//到顶，向左移动到最左上角
			{
				NowListPos = 0;
				lineStr[lineCount] = 0;
				upAccess = 0;
			}
			else if (leftChar == '\r' || leftChar == '\n')		//最左，上移
			{
				NowListPos--;
				leftChar = result[NowListPos - 1];
				lineCount--;
				lineStr[lineCount]--;
				while (leftChar != '\r' && leftChar != '\n'&& NowListPos != 0)
				{
					NowListPos--;
					leftChar = result[NowListPos - 1];
					lineStr[lineCount]--;
				}
			}

			else												//不是首行的一行中间，左移到行首
			{
				while (leftChar != '\n'&& leftChar != '\r')
				{
					NowListPos--;
					leftChar = result[NowListPos - 1];
					lineStr[lineCount] = 0;
				}
				break;
			}
			strcpy(now_result, result);
			break;
		case VK_DOWN:
			if (result[NowListPos] == 0)
			{
				result[NowListPos] = '\r';
				totalChar++;
				NowListPos++;
				lineStr[lineCount]++;
				strcpy(now_result, result);
				lineCount++;
			}
			else if (result[NowListPos] == '\r' || result[NowListPos] == '\n')
			{
				NowListPos++;
				lineStr[lineCount]++;
				lineCount++;

			}
			else
			{
				while (result[NowListPos] != 0 && result[NowListPos] != '\n'&& result[NowListPos] != '\r')			//验证1.是否已经到文末；2.验证我按下键，有没有下一行
				{
					NowListPos++;
					lineStr[lineCount]++;
				}
			}
			strcpy(now_result, result);
			break;

		}
	case KEY_UP:
		break;
	}


	/*
	快捷键功能：
	1、刷新显示
	2、重复
	3、查找
	4、替换
	*/
	switch (event)
	{
	case KEY_DOWN:
		switch (key)
		{
		case VK_F5:
			strcpy(now_result, result);
			int i;
			for (i = 0; i < MAXNUM; i++) lineStr[i] = 0;			//重置lineStr
			for (i = 0; i < MAXNUM; i++) cnOrEng[i] = 0;			//重置cnOrEng
			lineCount = 1;
			totalChar = 0;
			nowChar = 1;
			NowListPos = 0;
			for (i = 0; result[i] != 0; i++)
			{
				if (result[i] == '\b')
				{
					MouseInBox = 1;
					Display();
				}
				else if (result[i] == '\n' || result[i] == '\r')
				{

					cnOrEng[nowChar] = 2;
					totalChar++;
					lineStr[lineCount]++;
					//lineStr[lineCount] -= mistakeCN;
					//mistakeCN = 0;
					nowChar++;
					lineCount++;
					NowListPos++;
				}
				else if (IsEng(result[i]) == 1)
				{
					cnOrEng[nowChar] = 1;
					totalChar++;
					lineStr[lineCount]++;
					nowChar++;
					NowListPos++;
					//lineStr[lineCount][lineStrCount] = 1;
				}
				else if (IsEng(result[i]) == 0)					//对于中文，会执行两次ch的读入，但我只需要它执行一次，cnOnlyDoOnce帮助我完成只执行一次
				{
					//if (cnOnlyDoOnce)
					//{
					cnOrEng[nowChar] = 3;
					totalChar++;
					lineStr[lineCount]++;
					nowChar++;
					i++;
					NowListPos += 2;
					//mistakeCN++;
					//cnOnlyDoOnce = !cnOnlyDoOnce;
				//}
				//else cnOnlyDoOnce = !cnOnlyDoOnce;
				}
			}
			/*光标追踪*/
			mx = TXTFIRSTX, my = TXTFIRSTY; 					//位置重置
			double chHeight = GetFontHeight();
			for (i = 0; result[i] != 0; i++)
			{
				if (result[i] == '\b')
				{
					MouseInBox = 1;
					Display();
				}
				else if (result[i] == '\n' || result[i] == '\r')
				{
					mx = TXTFIRSTX;
					my -= chHeight;
				}
				else if (IsEng(result[i]) == 1)
				{
					char strNext[2];
					strNext[0] = result[i];
					strNext[1] = '\0';
					mx += TextStringWidth(strNext);
				}
				else if (IsEng(result[i]) == 0)
				{
					mx += CNFONTWIDTH;
				}
			}
			break;
		}
	case BUTTON_UP:
		break;
	}

	Display();
	DrawTextEditor();
	myDrawText();
}


// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	int i = 0;
	uiGetMouse(x, y, button, event);					//GUI获取鼠标（不用管他，我们的鼠标只用于菜单选择）
	switch (event)
	{
	case BUTTON_DOWN:MouseInBox = 1;					//鼠标点击显示光标
		void SignalJudge(x, y);							//调用SignalJudge移动光标
		break;				
	}
	//if(event!=MOUSEMOVE)
	{
		Display();
		myDrawText();
	}


	//保证查找标记不会消失
	while (choice[i][0] != 0) 
	{
		MovePen(choice[i][0], choice[i][1]);
		SetPenColor("Violet");
		DrawLine(0.15, 0);
		SetPenColor("Blue");
		i++;
	}

}
// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	if (IsCtrlPressed == 0)
	{
		uiGetChar(ch);
		MouseInBox = 0;
		/*
		说明：为了方便移动光标，我们创建了一个int型数组，数组下标代表输入的字符序号，
		每一个数组元素储存了当前字符的类型（中文3/英文1/换行回车2）
		lineStr数组用于删除字符，当删除时当前数组元素-1，当减为零时行数-1，光标向上移动
		*/
		/*字符读入与储存*/
		if (ch == '\b')
		{
			MouseInBox = 1;
			Display();
		}
		else if (IsEng(ch) == 1)							//输入英文
		{
			AddText(ch);						//字符回调‘ch’传至result数组中
			NowListPos++;
			maxPos++;
			totalChar++;
			MouseInBox = 1;
			Display();
			DrawTextEditor();
			myDrawText();
		}
		else if (IsEng(ch) != 1)							//输入中文
		{
			AddText(ch);									//字符回调‘ch’传至result数组中
			NowListPos++;
			maxPos++;
			MouseInBox = 1;
			Display();
			DrawTextEditor();
			myDrawText();
			if (cnOnlyDoOnce)
			{
				totalChar++;
				cnOnlyDoOnce != cnOnlyDoOnce;
			}
			else
				cnOnlyDoOnce != cnOnlyDoOnce;

		}
		strcpy(result, now_result);
		/*光标的自动追踪*/
		if (ch == '\b')
		{
			Display();
		}
		else if (IsEng(ch) == 1)
		{
			double chHeight = GetFontHeight();				//字符高度
			char strNext[2];
			strNext[0] = ch;
			strNext[1] = '\0';
			mx += TextStringWidth(strNext);
			if (ch == '\n' || ch == '\r')
			{
				mx = TXTFIRSTX;
				my -= chHeight;
			}
			MouseInBox = 1;
			Display();
			DrawTextEditor();
			myDrawText();
		}
		else if (IsEng(ch) != 1)
		{
			mx += CNFONTWIDTH;								//中文字符宽度
		}

		/*字符统计*/

		if (ch == '\b')
		{
			MouseInBox = 1;
			Display();
		}
		else if (ch == '\n' || ch == '\r')
		{
			cnOrEng[nowChar] = 2;

			lineStr[lineCount]++;
			//lineStr[lineCount] -= mistakeCN;
			//mistakeCN = 0;
			lineCount++;

		}
		else if (IsEng(ch) == 1)
		{
			cnOrEng[nowChar] = 1;

			lineStr[lineCount]++;


			//lineStr[lineCount][lineStrCount] = 1;
		}
		else if (IsEng(ch) == 0)					//对于中文，会执行两次ch的读入，但我只需要它执行一次，cnOnlyDoOnce帮助我完成只执行一次
		{
			if (cnOnlyDoOnce)
			{
				cnOrEng[nowChar] = 3;

				lineStr[lineCount]++;

				//mistakeCN++;
				cnOnlyDoOnce = !cnOnlyDoOnce;
			}
			else cnOnlyDoOnce = !cnOnlyDoOnce;
		}


	}
}


//用户的时间响应函数,闪烁光标
void TimeEventProcess(int timerID)
{
	if (MouseInBox)
	{
		switch (timerID)
	case 1:SetEraseMode(isDisplayGB);
		DrawGB(mx, my);										//在（mx，my）处画光标
		isDisplayGB = !isDisplayGB;							//实现闪烁
	}
	if (timerID == 2) {//用于查找函数调用
		SetEraseMode(isDisplayGB);
		DrawGB(2.5, 3.5 - GetFontHeight() - 0.2);
		isDisplayGB = !isDisplayGB;
	}
	if (timerID == 3)
	{
		//Display();
		//myDrawText();
	}
	if (timerID == 4)
	{
		ChangeBackgroundColor();
		Display();
		myDrawText();
	}
}


//光标绘制函数,x,y代表当前输入字符的右下角
void DrawGB(double x, double y)
{
	double fontHeight = GetFontHeight();
	MovePen(x, y);
	DrawLine(0, fontHeight);
}
