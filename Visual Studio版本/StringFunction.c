#include "StringFunction.h"
#include "Init.h"
#include "Menu.h"

//说明：txt默认编码规则是ans1,即2个字节表示，英文用asc2,中文用gbk
int NowListPos = 0;//指示目前光标对应的文字的result下标，即result[NowList]
char StrLin[3];//临时存储非英文字符以便输出
extern char result[MAXSTRINGLENTH];
extern int maxPos;
extern int totalChar;
char now_result[MAXSTRINGLENTH];//目前该页的字符串
//static int f_count, f_linecnt=0;//统计一行的字符数与行数
int f_line[MAXSTRINGLENTH];//存储f_count的数组
extern int lineStr[MAXSTRINGLENTH];										//记录每行的字符数
extern int nowChar ;										//记录当前字符的中英或者换行
extern int lineCount;										//记录行数

int IsEng(char c)
{
	return (c < 128 && c > 0);
}

void myDrawText() //框位置为0.1，字符从0.12开始绘制
{
	char str[1000] = { 0 };//存储单行数据
	char *f_result = now_result;//now_result的指针
	char *f_str = str;//单行数据str的指针
	double f_y = GetWindowHeight() - GetFontHeight() * 2 - 0.1 - 0.07;//初始y坐标，字符左下角
	int f_count = 0, f_linecnt = 0;//统计一行的字符数与行数

	MovePen(0.12, f_y);

	while (*f_result != 0 && f_y > 0.5) {
		*f_str = *f_result;
		f_count++;
		if (IsEndLine(str)) {//是否需要自动换行
			if (!IsEng(*f_str)) {
				/*检测到中文*/
				StrLin[0] = *f_str;
				*f_str = 0;
				f_count--;

				DrawTextString(str);
				f_y -= GetFontHeight();
				MovePen(0.12, f_y);//换行
				/*str置零*/
				for (f_str = str; *f_str != 0; f_str++)
					*f_str = 0;
				f_str = str;
				*f_str = StrLin[0];
				f_str++;
				/*统计一行有几个字符*/
				f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;

				f_result++;
			}
			else {//英文输入
				DrawTextString(str);
				f_y -= GetFontHeight();
				MovePen(0.12, f_y);
				/*str置零*/
				for (f_str = str; *f_str != 0; f_str++)
					*f_str = 0;
				f_str = str;
				/*统计一行有几个字符*/
				f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;

				f_result++;
			}
		}
		else if (*f_str == '\r' || *f_str == '\n') {//手动换行，实现过程同英文
			DrawTextString(str);
			f_y -= GetFontHeight();
			MovePen(0.12, f_y);

			for (f_str = str; *f_str != 0; f_str++)
				*f_str = 0;
			f_str = str;
			f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;
			f_result++;
		}
		else {
			f_str++; f_result++;
		}
	}
	*f_str = 0;
	DrawTextString(str);//输出最后一行
	f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;
	//InitConsole(); printf("%d", f_line[0]);
}


int IsEndLine(char* str)//框宽度为屏幕宽度-0.2，tot为该行目前的字符数 
{
	double f_width;
	f_width = GetWindowWidth() - 0.2 - 0.1;//最后一个数据根据实际情况调整

	if (TextStringWidth(str) > f_width)
		return 1;
	else
		return 0;
}

void SignalJudge(double x, double y)//x,y为鼠标回调传入位置，已转化为英寸
{
	int line = (int)y / GetFontHeight();//点击位置位于第几行
	int i, pos;//一行的位置
	int poslin, odd = 0;//处理点击位置是中文字符
	char str[1000] = { 0 };
	if (f_line[line] == 0) {//点击在页面下方空白处，画笔置最下一行左边
		MovePen(0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
		return;
	}
	/*确定点击位置对应的字符串下标*/
	for (i = 0; i < line; i++)//最后i=line
		NowListPos += f_line[i];
	/*因为只能得到一行的字符的总宽度，采用逐个累加的方法，只是为了textstringwidth*/
	for (pos = 0; pos < f_line[i]; pos++, NowListPos++) {
		str[pos] = now_result[NowListPos];//将总字符串转化为行字符串
		if (TextStringWidth(str) + 0.12 > x || str[pos] == '\r' || str[pos] == '\n') {
			if (IsEng(str[pos])) {
				/*点击位置是英文字符*/
				str[pos] = 0;//将这个字符抹去，因为定位到左下角
				MovePen(TextStringWidth(str) + 0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
				return;
			}
			else {
				/*处理中文字符，主要是为了区分点击在左半部分还是后半部分*/
				for (poslin = pos; !IsEng(str[poslin]); poslin--) {
					odd++;
				}

				if (odd % 2 == 1) {//在左半部分
					str[pos] = 0;
					MovePen(TextStringWidth(str) + 0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
					return;
				}
				else {
					str[pos] = 0; str[--pos] = 0; NowListPos--;//在又半部分
					MovePen(TextStringWidth(str) + 0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
					return;
				}
			}
		}
		else {
			//do nothing
		}
	}
}

void AddText(char AddStr)
{
	int i;
	char lin1, lin2, lin;
	int odd = 0;
	static int mybool = 0;
	//举个例子，now=a,now+1=b,now+2(i)=c,now+3=d
	lin1 = now_result[NowListPos];//存储目前位置,a
	lin2 = now_result[NowListPos + 1];//目前下一个位置，为了移动数组,b
	for (i = NowListPos + 2; now_result[i - 2] != 0; i++) {//判断条件不是严谨的
		lin = lin1;//lin=a，第二次lin=b
		lin2 = now_result[i];//lin2=c,第二次lin2=d
		lin1 = now_result[i - 1];//lin1=b，第二次lin1=c
		now_result[i - 1] = lin;//now+1=a,而原来now是a,第二次now+2是b
	}
	now_result[NowListPos] = AddStr;//更改原来指定的位置字符
	//英文直接输出
	if (IsEng(AddStr)) {
		Display();
		myDrawText();
	}
	else {
		//中文先缓一个，再输出
		if (mybool == 0)
			mybool = 1;
		else {
			Display();
			myDrawText();
		}
	}
}

void DeleteText(int BorD)//B为0，D为1,B退格,D删除，退格相当于往前一格的删除
{
	int i;
	/*建立在鼠标点击的基础上，因此NowListPos已经更改为最新地址了*/
	if (BorD == 1)
	{
		if (now_result[NowListPos - 1] == '\n' || now_result[NowListPos - 1] == '\r'|| NowListPos == 0)
			return;
		if (IsEng(now_result[NowListPos - 1]))
		{
			NowListPos--;
			maxPos--;
			totalChar--;
			lineStr[lineCount] --;
			for (i = NowListPos; now_result[i] != 0; i++)
				now_result[i] = now_result[i + 1];//后面的前移一格
		}
		else
		{
			NowListPos -= 2;
			maxPos -= 2;
			totalChar--;
			lineStr[lineCount] --;
			for (i = NowListPos; now_result[i] != 0; i++) 
				now_result[i] = now_result[i + 2];//后面的前移两格
		}
	}
	else if (BorD == 0)
	{
		if (now_result[NowListPos] == 0)
			return;
		if (IsEng(now_result[NowListPos])) 
		{
			maxPos--;
			totalChar--;
			for (i = NowListPos; now_result[i] != 0; i++)
				now_result[i] = now_result[i + 1];//后面的前移一格
		}
		else
		{
			maxPos -= 2;
			totalChar--;
			for (i = NowListPos; now_result[i] != 0; i++)
				now_result[i] = now_result[i + 2];//后面的前移两格
		}
	}
	
}
