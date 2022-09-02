#include "Events.h"
#include "Init.h"
#include "MenuFunction.h"
#include "Menu.h"
#include "StringFunction.h"
#include "TextEditor.h"
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;								//�����һ���ַ����е�λ��
extern int MouseInBox;
extern int isDrawCondition;
extern int NowListPos;									//��ǰresult�����±�
extern int IsEng(char c);
extern void SignalJudge(double x, double y);
extern void AddText(char AddStr);
extern char now_result[MAXSTRINGLENTH];
extern void myDrawText();								//StringFunction���������
extern double mx, my;					//���ص����ص�ǰ�����ַ�λ�õĹ��
extern int cnOrEng[MAXSTRINGLENTH];										//��¼�ڼ��������л���Ӣ��
extern int lineStr[MAXSTRINGLENTH];										//��¼ÿ�е��ַ���
extern int nowChar;										//��¼��ǰ�ַ�����Ӣ���߻���
extern int lineCount;										//��¼����
extern int maxPos;			 								//��¼�±�����ֵ�������ƶ����ʱ���ж�
extern int totalChar;										//���ַ�����F5ˢ��
extern char lineString[MAXSTRINGLENTH];									//��ʱ���浱ǰ���ַ�,���ܻ����Զ���������
double choice[SHORTERSTRINGLENTH][2];//ѡ��Ļ���λ��
static char find[SHORTERSTRINGLENTH];//��¼����������ַ���
static char Lin[3];//��ʱ�洢�����ַ���ͨ���ַ���
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
	p = localtime(&timep); //ȡ�õ���ʱ��
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
		mx += chWidth;							//��������ƶ�����ǰ�±�����
		NowListPos++;
	}
	Display();
	DrawTextEditor();
	myDrawText();
	return ;
}
void ShowStringFind()
{
	//Ԥ�������ɹ����θ���
	strcpy(find, "");
	strcpy(Lin, "");
	ch_num = 0;
	memset(choice, 0, sizeof(choice));
	
	//part1:���һ����
	//textbox(GenUIID(0), 1, 2, 2, 1, memo, sizeof(memo));

	//InitConsole();printf("%d", f_line[0]);

	SetPenColor("Black");
	drawRectangle(2, 2, 6, 2, 1);//����2��4������8��2
	SetPenColor("White");
	MovePen(2.5, 3.5);
	DrawTextString("��������Ҫ���ҵ��ַ���");
	//����С��ȷ����
	drawRectangle(5, 2.5, 0.6, 0.3, 0); MovePen(5.15, 2.6); DrawTextString("����");
	drawRectangle(6, 2.5, 0.6, 0.3, 0); MovePen(6.15, 2.6); DrawTextString("ȡ��");

	MovePen(2.5, 3.5 - GetFontHeight() - 0.2);//����Ŀǰ����λ��
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
	//static int ch_num = 0;//��¼�ַ����±�
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
	//��껮����ʹ��ͻ����ʾ
	if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(6, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(6.15, 2.6); DrawTextString("ȡ��");
	}
	else if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(5, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(5.15, 2.6); DrawTextString("����");
	}

	switch (event) {
	case BUTTON_DOWN://ִ�в�ѯ�򲻲�ѯ
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
	double f_y = GetWindowHeight() - GetFontHeight() * 2 - 0.1 - 0.07;//��ʼy���꣬�ַ����½�
	NowListPos = 0;


	while ((start = FindString(find, str, start)) != -1) {//�����ҵ�λ���뿪ʼλ�õľ��룬���򷵻�-1
		NowListPos += start;
		//InitConsole(); printf("%d+ %d ", start, NowListPos);
		cnt = 0;
		for (i = 0; i < 100; i++) {
			cnt += f_line[i];
			if (cnt > NowListPos) break;
		}

		cnt = cnt - f_line[i];//���Ϊ���п�ͷ���±�
		debug = i;

		//�д�����ȡһ�п��
		j = 0;
		for (i = cnt; i < NowListPos; i++) {
			memo[j] = now_result[i];
			j++;
		}
		if (memo[0] == 0) wid = 0;
		else wid = TextStringWidth(memo);
		choice[count][0] = 0.12 + wid; choice[count++][1] = f_y - GetFontHeight()*debug;//�洢ѡ�еĻ���λ��
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


void ShowStringCompare()//����charevent,mouseevent����
{
	//Ԥ�������ɹ����θ���
	strcpy(find, "");
	strcpy(Lin, "");
	ch_num = 0;
	//InitConsole();printf("%d", f_line[0]);

	SetPenColor("Black");
	drawRectangle(2, 2, 6, 2, 1);//����2��4������8��2
	SetPenColor("White");
	MovePen(2.5, 3.5);
	DrawTextString("��������Ҫ�滻���ַ����м��ÿո����֣�");
	//����С��ȷ����
	drawRectangle(5, 2.5, 0.6, 0.3, 0); MovePen(5.15, 2.6); DrawTextString("�滻");
	drawRectangle(6, 2.5, 0.6, 0.3, 0); MovePen(6.15, 2.6); DrawTextString("ȡ��");

	MovePen(2.5, 3.5 - GetFontHeight() - 0.2);//����Ŀǰ����λ��
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
	//��껮����ʹ��ͻ����ʾ
	if (mx > 6 && mx<6.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(6, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(6.15, 2.6); DrawTextString("ȡ��");
	}
	else if (mx > 5 && mx<5.6&&my>2.5&&my < 2.8) {
		SetPenColor("White"); drawRectangle(5, 2.5, 0.6, 0.3, 1); SetPenColor("Black"); MovePen(5.15, 2.6); DrawTextString("�滻");
	}

	switch (event) {
	case BUTTON_DOWN://ִ�в�ѯ�򲻲�ѯ
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

	//��find���
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
	//memo��¼���滻�� ��findΪԭʼ

	NowListPos = 0;
	while ((start = FindString(find, str, start)) != -1) {//�����ҵ�λ���뿪ʼλ�õľ��룬���򷵻�-1
		NowListPos += start;

		while (count++ < strlen(find))
			DeleteText(0);

		count = strlen(memo);
		while (count > 0) {
			count--; AddText(memo[count]);
		}//��Ҳ��֪������ʲô��������Ҫ�Ӻ���ǰadd

		NowListPos += strlen(memo) - 1;
		str += start + strlen(memo) - 1;
	}
	SetPenColor("Blue");
	//Display();
	NowListPos = 0;
	myDrawText();

	//����Ĳ�Ҫ����
	cancelMouseEvent();
	cancelCharEvent();
	registerCharEvent(CharEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
}

void RunC() //��ȡnow_result�������ʵĸ��ģ�ֻʹ��һ����ɫ
{
	//InitConsole(); printf("%s", now_result);
	Display();
	char p = now_result[0];
	NowListPos = 0;
	int i = 0, j = 0, signal = 0;

	while (p != 0) {//��⵽���Ķ���Ҫ���
		switch (p) {
			/*case '#': {
				while (p != '\r'&&p != '\n'&&p != 0) NowListPos++;//ֱ��p��\r
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
	for (i = 0; i < MAXNUM; i++) lineStr[i] = 0;			//����lineStr
	for (i = 0; i < MAXNUM; i++) cnOrEng[i] = 0;			//����cnOrEng
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
		else if (IsEng(result[i]) == 0)					//�������ģ���ִ������ch�Ķ��룬����ֻ��Ҫ��ִ��һ�Σ�cnOnlyDoOnce���������ִֻ��һ��
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
	/*���׷��*/
	mx = TXTFIRSTX, my = TXTFIRSTY; 					//λ������
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
	sprintf(ConditionString, "��%d�У���%d��          ��������%d", lineCount, lineStr[lineCount], totalChar);
	MovePen(GetWindowWidth() / 3, 0.2);
	DrawTextString(ConditionString);
	SetEraseMode(0);
}
