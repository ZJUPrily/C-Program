#include "StringFunction.h"
#include "Init.h"
#include "Menu.h"

//˵����txtĬ�ϱ��������ans1,��2���ֽڱ�ʾ��Ӣ����asc2,������gbk
int NowListPos = 0;//ָʾĿǰ����Ӧ�����ֵ�result�±꣬��result[NowList]
char StrLin[3];//��ʱ�洢��Ӣ���ַ��Ա����
extern char result[MAXSTRINGLENTH];
extern int maxPos;
extern int totalChar;
char now_result[MAXSTRINGLENTH];//Ŀǰ��ҳ���ַ���
//static int f_count, f_linecnt=0;//ͳ��һ�е��ַ���������
int f_line[MAXSTRINGLENTH];//�洢f_count������
extern int lineStr[MAXSTRINGLENTH];										//��¼ÿ�е��ַ���
extern int nowChar ;										//��¼��ǰ�ַ�����Ӣ���߻���
extern int lineCount;										//��¼����

int IsEng(char c)
{
	return (c < 128 && c > 0);
}

void myDrawText() //��λ��Ϊ0.1���ַ���0.12��ʼ����
{
	char str[1000] = { 0 };//�洢��������
	char *f_result = now_result;//now_result��ָ��
	char *f_str = str;//��������str��ָ��
	double f_y = GetWindowHeight() - GetFontHeight() * 2 - 0.1 - 0.07;//��ʼy���꣬�ַ����½�
	int f_count = 0, f_linecnt = 0;//ͳ��һ�е��ַ���������

	MovePen(0.12, f_y);

	while (*f_result != 0 && f_y > 0.5) {
		*f_str = *f_result;
		f_count++;
		if (IsEndLine(str)) {//�Ƿ���Ҫ�Զ�����
			if (!IsEng(*f_str)) {
				/*��⵽����*/
				StrLin[0] = *f_str;
				*f_str = 0;
				f_count--;

				DrawTextString(str);
				f_y -= GetFontHeight();
				MovePen(0.12, f_y);//����
				/*str����*/
				for (f_str = str; *f_str != 0; f_str++)
					*f_str = 0;
				f_str = str;
				*f_str = StrLin[0];
				f_str++;
				/*ͳ��һ���м����ַ�*/
				f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;

				f_result++;
			}
			else {//Ӣ������
				DrawTextString(str);
				f_y -= GetFontHeight();
				MovePen(0.12, f_y);
				/*str����*/
				for (f_str = str; *f_str != 0; f_str++)
					*f_str = 0;
				f_str = str;
				/*ͳ��һ���м����ַ�*/
				f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;

				f_result++;
			}
		}
		else if (*f_str == '\r' || *f_str == '\n') {//�ֶ����У�ʵ�ֹ���ͬӢ��
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
	DrawTextString(str);//������һ��
	f_line[f_linecnt] = f_count; f_linecnt++; f_count = 0;
	//InitConsole(); printf("%d", f_line[0]);
}


int IsEndLine(char* str)//����Ϊ��Ļ���-0.2��totΪ����Ŀǰ���ַ��� 
{
	double f_width;
	f_width = GetWindowWidth() - 0.2 - 0.1;//���һ�����ݸ���ʵ���������

	if (TextStringWidth(str) > f_width)
		return 1;
	else
		return 0;
}

void SignalJudge(double x, double y)//x,yΪ���ص�����λ�ã���ת��ΪӢ��
{
	int line = (int)y / GetFontHeight();//���λ��λ�ڵڼ���
	int i, pos;//һ�е�λ��
	int poslin, odd = 0;//������λ���������ַ�
	char str[1000] = { 0 };
	if (f_line[line] == 0) {//�����ҳ���·��հ״�������������һ�����
		MovePen(0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
		return;
	}
	/*ȷ�����λ�ö�Ӧ���ַ����±�*/
	for (i = 0; i < line; i++)//���i=line
		NowListPos += f_line[i];
	/*��Ϊֻ�ܵõ�һ�е��ַ����ܿ�ȣ���������ۼӵķ�����ֻ��Ϊ��textstringwidth*/
	for (pos = 0; pos < f_line[i]; pos++, NowListPos++) {
		str[pos] = now_result[NowListPos];//�����ַ���ת��Ϊ���ַ���
		if (TextStringWidth(str) + 0.12 > x || str[pos] == '\r' || str[pos] == '\n') {
			if (IsEng(str[pos])) {
				/*���λ����Ӣ���ַ�*/
				str[pos] = 0;//������ַ�Ĩȥ����Ϊ��λ�����½�
				MovePen(TextStringWidth(str) + 0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
				return;
			}
			else {
				/*���������ַ�����Ҫ��Ϊ�����ֵ������벿�ֻ��Ǻ�벿��*/
				for (poslin = pos; !IsEng(str[poslin]); poslin--) {
					odd++;
				}

				if (odd % 2 == 1) {//����벿��
					str[pos] = 0;
					MovePen(TextStringWidth(str) + 0.12, GetWindowHeight() - GetFontHeight()*1.5 - 0.1 - 0.02 - line * GetFontHeight());
					return;
				}
				else {
					str[pos] = 0; str[--pos] = 0; NowListPos--;//���ְ벿��
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
	//�ٸ����ӣ�now=a,now+1=b,now+2(i)=c,now+3=d
	lin1 = now_result[NowListPos];//�洢Ŀǰλ��,a
	lin2 = now_result[NowListPos + 1];//Ŀǰ��һ��λ�ã�Ϊ���ƶ�����,b
	for (i = NowListPos + 2; now_result[i - 2] != 0; i++) {//�ж����������Ͻ���
		lin = lin1;//lin=a���ڶ���lin=b
		lin2 = now_result[i];//lin2=c,�ڶ���lin2=d
		lin1 = now_result[i - 1];//lin1=b���ڶ���lin1=c
		now_result[i - 1] = lin;//now+1=a,��ԭ��now��a,�ڶ���now+2��b
	}
	now_result[NowListPos] = AddStr;//����ԭ��ָ����λ���ַ�
	//Ӣ��ֱ�����
	if (IsEng(AddStr)) {
		Display();
		myDrawText();
	}
	else {
		//�����Ȼ�һ���������
		if (mybool == 0)
			mybool = 1;
		else {
			Display();
			myDrawText();
		}
	}
}

void DeleteText(int BorD)//BΪ0��DΪ1,B�˸�,Dɾ�����˸��൱����ǰһ���ɾ��
{
	int i;
	/*������������Ļ����ϣ����NowListPos�Ѿ�����Ϊ���µ�ַ��*/
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
				now_result[i] = now_result[i + 1];//�����ǰ��һ��
		}
		else
		{
			NowListPos -= 2;
			maxPos -= 2;
			totalChar--;
			lineStr[lineCount] --;
			for (i = NowListPos; now_result[i] != 0; i++) 
				now_result[i] = now_result[i + 2];//�����ǰ������
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
				now_result[i] = now_result[i + 1];//�����ǰ��һ��
		}
		else
		{
			maxPos -= 2;
			totalChar--;
			for (i = NowListPos; now_result[i] != 0; i++)
				now_result[i] = now_result[i + 2];//�����ǰ������
		}
	}
	
}
