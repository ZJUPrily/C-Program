#include "Events.h"
#include "Init.h"
#include "MenuFunction.h"
#include "Menu.h"
#include "StringFunction.h"
#include "TextEditor.h"
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;								//光标在一行字符串中的位置
extern int MouseInBox;
extern int isDrawCondition;
extern int NowListPos;									//当前result数组下标
extern int IsEng(char c);
extern void SignalJudge(double x, double y);
extern void AddText(char AddStr);
extern char now_result[MAXSTRINGLENTH];
extern void myDrawText();								//StringFunction的输出函数
extern double mx, my;					//鼠标回调返回当前输入字符位置的光标
extern int cnOrEng[MAXSTRINGLENTH];										//记录第几个数是中还是英文
extern int lineStr[MAXSTRINGLENTH];										//记录每行的字符数
extern int nowChar;										//记录当前字符的中英或者换行
extern int lineCount;										//记录行数
extern int maxPos;			 								//记录下标的最大值，用于移动光标时的判断
extern int totalChar;										//总字符数，F5刷新
extern char lineString[MAXSTRINGLENTH];									//暂时储存当前行字符,可能会在自动换行有用
double choice[SHORTERSTRINGLENTH][2];//选择的画笔位置
static char find[SHORTERSTRINGLENTH];//记录查找输入的字符串
static char Lin[3];//临时存储中文字符的通用字符串
static int ch_num ;
extern int upAccess;
extern void DrawGB(double x, double y);
extern void KeyboardEventProcess(int key, int event);
extern void MouseEventProcess(int x, int y, int button, int event);
extern void CharEventProcess(char ch);
extern int f_line[MAXSTRINGLENTH];
static char *backgroundcolor[6] = { "white","BackRed","BackYellow","BackBlue","BackGreen","BackPurple" };
static int BackgroundColorChoice = 0;
void DoBuildNewText()
{
	int i = 0, j = 0;
	for (i = 0; i < 1000; i++)
	{
		result[i] = '\0';
		lineString[i] = '\0';
		f_line[i] = 0;
		lineStr[i] = 0;
		now_result[i] = '\0';
		cnOrEng[i] = 0;
	}
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 2; j++)
		{
			choice[i][j] = 0;
		}
	}
	mx = TXTFIRSTX, my = TXTFIRSTY;
	nowChar = 1;
	lineCount = 1;
	maxPos = 0;
	totalChar = 0;
	upAccess = 0;
	NowListPos = 0;


	refreshF5();

	Display();
	DrawTextEditor();
	myDrawText();
}

void addTime()
{	
	char nowTime[100], rightChar, rightStr[2];
	double chWidth;
	char nextChar;
	int i = 0,maxNum;
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep); //取得当地时间
	maxNum = sprintf(nowTime,"%d/%d/%d %02d:%02d:%02d", (p->tm_year + 1900), (p->tm_mon + 1),p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	int k = 0;
	while (i< maxNum)
	{
		AddText(nowTime[i]);
		strcpy(result, now_result);
		totalChar++;
		lineStr[lineCount]++;
		maxPos++;
		i++;
		strcpy(result, now_result);
		rightChar = result[NowListPos];
		rightStr[0] = result[NowListPos];
		rightStr[1] = '\0';
		chWidth = TextStringWidth(rightStr);
		mx += chWidth;							//光标向右移动，当前下标右移
		NowListPos++;
	}
	Display();
	DrawTextEditor();
	myDrawText();
	return ;
}
void ShowStringFind()
{
	//预修正，可供二次更改
	strcpy(find, "");
	strcpy(Lin, "");
	ch_num = 0;
	memset(choice, 0, sizeof(choice));
	
	//part1:输出一个框
	//textbox(GenUIID(0), 1, 2, 2, 1, memo, sizeof(memo));

	//InitConsole();printf("%d", f_line[0]);

	SetPenColor("Black");
	drawRectangle(2, 2, 6, 2, 1);//左上2，4；右下8，2
	SetPenColor("White");
	MovePen(2.5, 3.5);
	DrawTextString("请输入需要查找的字符：");
	//两个小框，确定用
	drawRectangle(5, 2.5, 0.6, 0.3, 0); MovePen(5.15, 2.6); DrawTextString("查找");
	drawRectangle(6, 2.5, 0.6, 0.3, 0); MovePen(6.15, 2.6); DrawTextString("取消");

	MovePen(2.5, 3.5 - GetFontHeight() - 0.2);//这是目前画笔位置
	//startTimer(2, 500);
	DrawLine(4, 0);

	cancelMouseEvent();
	cancelCharEvent();
	cancelKeyboardEvent();

	registerCharEvent(f_CharEventProcess);
	registerMouseEvent(f_MouseEventProcess);


}

void f_CharEventProcess(char ch)
{
	//static int ch_num = 0;//记录字符串下标
	static int bol;
	SetPenColor("White");
	if (IsEng(ch)) {
		MovePen(2.5 + TextStringWidth(find), 3.5 - GetFontHeight() - 0.2);
		Lin[0] = ch;
		DrawTextString(Lin);
		find[ch_num++] = ch;
	}
	else if (bol == 0) {
		Lin[0] = ch;
		bol = 1;
		find[ch_num++] = ch;
	}
	else {
		MovePen(2.5 + TextStringWidth(find), 3.5 - GetFontHeight() - 0.2);
		Lin[1] = ch;
		bol = 0;
		DrawTextString(Lin);
		find[ch_num++] = ch;
	}
}

void f_MouseEventProcess(int x, int y, int button, int event)
{

	double mx = ScaleXInches(x);
	double my = ScaleYInches(y);
	//鼠标划过框使其突出显示
	if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(6, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(6.15, 2.6); DrawTextString("取消");
	}
	else if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(5, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(5.15, 2.6); DrawTextString("查找");
	}

	switch (event) {
	case BUTTON_DOWN://执行查询或不查询
		if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
			SetEraseMode(TRUE);
			drawRectangle(0.09, 0.49, GetWindowWidth() - 0.18, GetWindowHeight() - 0.88, 1);
			SetEraseMode(FALSE);
			//cancelTimer(2);
			HighLight();
			//return ;
		}
		else if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
			SetEraseMode(TRUE);
			drawRectangle(0.09, 0.49, GetWindowWidth() - 0.18, GetWindowHeight() - 0.88, 1);
			SetEraseMode(FALSE);
			//cancelTimer(2);

			SetPenColor("Blue");
			//Display();
			myDrawText();
			//return;
			cancelMouseEvent();
			cancelCharEvent();
			registerCharEvent(CharEventProcess);
			registerMouseEvent(MouseEventProcess);
			registerKeyboardEvent(KeyboardEventProcess);
		}break;
	case BUTTON_UP:break;
		//case MOUSEMOVE:break;
	default:break;
	}
}
void HighLight()
{
	char memo[1000] = { 0 };
	long int start = 0;
	char *str = now_result;
	int i = 0, j = 0, cnt = 0, count = 0;
	int debug;
	double wid;
	double f_y = GetWindowHeight() - GetFontHeight() * 2 - 0.1 - 0.07;//初始y坐标，字符左下角
	NowListPos = 0;


	while ((start = FindString(find, str, start)) != -1) {//返回找到位置与开始位置的距离，否则返回-1
		NowListPos += start;
		//InitConsole(); printf("%d+ %d ", start, NowListPos);
		cnt = 0;
		for (i = 0; i < 100; i++) {
			cnt += f_line[i];
			if (cnt > NowListPos) break;
		}

		cnt = cnt - f_line[i];//标记为本行开头的下标
		debug = i;

		//行处理，读取一行宽度
		j = 0;
		for (i = cnt; i < NowListPos; i++) {
			memo[j] = now_result[i];
			j++;
		}
		if (memo[0] == 0) wid = 0;
		else wid = TextStringWidth(memo);
		choice[count][0] = 0.12 + wid; choice[count++][1] = f_y - GetFontHeight()*debug;//存储选中的画笔位置
		MovePen(0.12 + wid, f_y - GetFontHeight()*cnt);
		SetPenColor("Violet");
		DrawLine(0.1, 0);

		str += start + strlen(find);
		NowListPos += strlen(find);
	}
	SetPenColor("Blue");
	//Display();
	myDrawText();

	cancelMouseEvent();
	cancelCharEvent();
	registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
}


void ShowStringCompare()//共用charevent,mouseevent单独
{
	//预修正，可供二次更改
	strcpy(find, "");
	strcpy(Lin, "");
	ch_num = 0;
	//InitConsole();printf("%d", f_line[0]);

	SetPenColor("Black");
	drawRectangle(2, 2, 6, 2, 1);//左上2，4；右下8，2
	SetPenColor("White");
	MovePen(2.5, 3.5);
	DrawTextString("请输入需要替换的字符，中间用空格区分：");
	//两个小框，确定用
	drawRectangle(5, 2.5, 0.6, 0.3, 0); MovePen(5.15, 2.6); DrawTextString("替换");
	drawRectangle(6, 2.5, 0.6, 0.3, 0); MovePen(6.15, 2.6); DrawTextString("取消");

	MovePen(2.5, 3.5 - GetFontHeight() - 0.2);//这是目前画笔位置
	DrawLine(4, 0);

	cancelMouseEvent();
	cancelCharEvent();
	cancelKeyboardEvent();

	registerCharEvent(f_CharEventProcess);
	registerMouseEvent(f2_MouseEventProcess);
}

void f2_MouseEventProcess(int x, int y, int button, int event)
{
	double mx = ScaleXInches(x);
	double my = ScaleYInches(y);
	//鼠标划过框使其突出显示
	if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(6, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(6.15, 2.6); DrawTextString("取消");
	}
	else if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(5, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(5.15, 2.6); DrawTextString("替换");
	}

	switch (event) {
	case BUTTON_DOWN://执行查询或不查询
		if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
			SetEraseMode(TRUE);
			drawRectangle(0.09, 0.49, GetWindowWidth() - 0.18, GetWindowHeight() - 0.88, 1);
			SetEraseMode(FALSE);
			//cancelTimer(2);
			Replace();
			//return ;
		}
		else if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
			SetEraseMode(TRUE);
			drawRectangle(0.09, 0.49, GetWindowWidth() - 0.18, GetWindowHeight() - 0.88, 1);
			SetEraseMode(FALSE);
			//cancelTimer(2);

			SetPenColor("Blue");
			//Display();
			myDrawText();
			//return;
			cancelMouseEvent();
			cancelCharEvent();
			registerCharEvent(CharEventProcess);
			registerMouseEvent(MouseEventProcess);
			registerKeyboardEvent(KeyboardEventProcess);
		}break;
	case BUTTON_UP:break;
		//case MOUSEMOVE:break;
	default:break;
	}
}

void Replace()
{
	char memo[100] = { 0 };
	long int start = 0;
	char* str = now_result;
	int i = 0, j = 0, cnt = 0, count = 0;

	//将find拆分
	for (i = 0; i < 100; i++) {
		if (find[i] == ' ') {
			find[i] = 0;
			break;
		}
	}
	for (j = ++i; j < 100; j++, cnt++) {
		memo[cnt] = find[j];
		find[j] = 0;
	}
	//InitConsole(); printf("%s1%s", find,memo);
	//memo记录了替换后 ，find为原始

	NowListPos = 0;
	while ((start = FindString(find, str, start)) != -1) {//返回找到位置与开始位置的距离，否则返回-1
		NowListPos += start;

		while (count++ < strlen(find))
			DeleteText(0);

		count = strlen(memo);
		while (count > 0) {
			count--; AddText(memo[count]);
		}//我也不知道出了什么错，但就是要从后往前add

		NowListPos += strlen(memo) - 1;
		str += start + strlen(memo) - 1;
	}
	SetPenColor("Blue");
	//Display();
	NowListPos = 0;
	myDrawText();

	//下面的不要更改
	cancelMouseEvent();
	cancelCharEvent();
	registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
}

void RunC() //读取now_result并做合适的更改，只使用一种颜色
{
	//InitConsole(); printf("%s", now_result);
	Display();
	char p = now_result[0];
	NowListPos = 0;
	int i = 0, j = 0, signal = 0;

	while (p != 0) {//检测到中文都不要输出
		switch (p) {
			/*case '#': {
				while (p != '\r'&&p != '\n'&&p != 0) NowListPos++;//直到p打到\r
				}*/
		case ';': {
			NowListPos++;  if (now_result[NowListPos] == '\r') break;
			while (j++ <= i) AddText(' ');
			AddText('\r');  j = 0; break;
		}
		case '{': {
			i += 2;
			NowListPos++; if (now_result[NowListPos] == '\r') break;
			while (j++ <= i) AddText(' '); AddText('\r'); j = 0;
			break;
		}
		case '}': {
			i -= 2;
			NowListPos++; if (now_result[NowListPos] == '\r') break;
			while (j++ <= i) AddText(' '); AddText('\r'); j = 0;
			break;
		}
		case '+':case '-':case '*': case '/': {
			AddText(' '); NowListPos++;
			NowListPos++;
			AddText(' '); NowListPos++; break;
		}
		default: break;
		}
		p = now_result[++NowListPos];
	}

	NowListPos = 0;
	Display();
	myDrawText();
}

void refreshF5()
{
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
}


void ShowBackgroundcolor()
{
	SetPenColor(backgroundcolor[BackgroundColorChoice]);
	MovePen(0, 0);
	StartFilledRegion(1);
	DrawLine(0, GetWindowHeight());
	DrawLine(GetWindowWidth(), 0);
	DrawLine(0, -GetWindowHeight());
	DrawLine(-GetWindowWidth(), 0);
	EndFilledRegion();
}
void ChangeBackgroundColor()
{
	BackgroundColorChoice++;
	if (BackgroundColorChoice > 5) BackgroundColorChoice = 0;
	ShowBackgroundcolor();
}
void SetBoldOrItalic()
{
	static int BoldOrItalic = 0;
	BoldOrItalic++;
	if(BoldOrItalic==3) BoldOrItalic = 0;
	SetStyle(BoldOrItalic);
}
void DrawCondition(int isDrawCondition)
{
	if (isDrawCondition) SetPenColor("black");
	else SetPenColor(backgroundcolor[BackgroundColorChoice]);
	MovePen(0.1, 0.1);
	DrawLine(0, 0.3);
	DrawLine(GetWindowWidth() - 0.2, 0);
	DrawLine(0, -0.3);
	DrawLine(-GetWindowWidth() + 0.2, 0);
	char ConditionString[100];
	sprintf(ConditionString, "第%d行，第%d列          总字数：%d", lineCount, lineStr[lineCount], totalChar);
	MovePen(GetWindowWidth() / 3, 0.2);
	DrawTextString(ConditionString);
	SetEraseMode(0);
}
