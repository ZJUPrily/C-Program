/*
ʹ��˵����֧����Ӣ�����룬���������ʾ��꣬����Զ�׷�٣����Ҽ������ƶ���֧꣬�ֻس����еȹ���
���Ҵ��ڵ����⣺
1�������ƶ����޷�׼ȷ��λ���ַ�
2������޷���λ�����봦
3��Delete������bug
*/


#include "Events.h"
#include "Init.h"
#include "Menu.h"
#include "time.h"
#include "TextEditor.h"
#include "StringFunction.h"
extern char result[MAXSTRINGLENTH];
extern int SignalPosition;								//�����һ���ַ����е�λ��
extern int MouseInBox;
extern int isDrawCondition;
extern int NowListPos;									//��ǰresult�����±�
extern int IsEng(char c);
extern void SignalJudge(double x, double y); 
void AddText(char AddStr);
extern char now_result[MAXSTRINGLENTH];
extern void myDrawText();								//StringFunction���������
double mx = TXTFIRSTX, my = TXTFIRSTY;					//���ص����ص�ǰ�����ַ�λ�õĹ��
int cnOrEng[MAXSTRINGLENTH];										//��¼�ڼ��������л���Ӣ��
int lineStr[MAXSTRINGLENTH];										//��¼ÿ�е��ַ���
int nowChar = 1;										//��¼��ǰ�ַ�����Ӣ���߻���
int lineCount = 1;										//��¼����
static bool isDisplayGB = TRUE;							//�����˸����
int maxPos = 0;			 								//��¼�±�����ֵ�������ƶ����ʱ���ж�
int totalChar = 0;										//���ַ�����F5ˢ��
static bool cnOnlyDoOnce = TRUE;						//��ʱû��
char lineString[MAXSTRINGLENTH];									//��ʱ���浱ǰ���ַ�,���ܻ����Զ���������
int upAccess = 0;
int IsCtrlPressed = 0;
extern double choice[SHORTERSTRINGLENTH][2];


// �û��ļ����¼���Ӧ����

void KeyboardEventProcess(int key, int event)
{
	if (key == VK_CONTROL && event == KEY_DOWN)
		IsCtrlPressed = 1;
	if (key == VK_CONTROL && event == KEY_UP)
		IsCtrlPressed = 0;
	double chWidth = 0;									//�ַ����	
	double chHeight = GetFontHeight();					//�ַ��߶�
	uiGetKeyboard(key, event);							// GUI��ȡ���̣����ù��������ǲ���imgui��������ܣ�
		/*��궨λ*/
	switch (event)
	{
		int i;
		char rightStr[2]; char leftStr[2];				//���������ƶ���TextStringWidth����
		char rightChar; char leftChar;					//���ڴ��������ַ�
	case KEY_DOWN:
		switch (key)
		{
		case VK_LEFT:
			if (NowListPos == 0)
				break;									//�����Ͻǣ������������ƶ�
			leftChar = result[NowListPos - 1];
			leftStr[0] = result[NowListPos - 1];
			leftStr[1] = '\0';
			if (IsEng(leftChar) == 1)
			{
				chWidth = TextStringWidth(leftStr);
				mx -= chWidth;							//��������ƶ�����ǰ�±�����
				Display();
				myDrawText();							//ˢ����ʾ����Ϊ�����ƶ������ˢ�»���֮ǰ��λ�����ºۼ�
			}
			else if (IsEng(leftChar) != 1)
			{
				mx -= 2 * CNFONTWIDTH;							//��������ƶ�����ǰ�±�����
				Display();
				myDrawText();							//ˢ����ʾ����Ϊ�����ƶ������ˢ�»���֮ǰ��λ�����ºۼ�
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
				mx += chWidth;							//��������ƶ�����ǰ�±�����
				Display();
				myDrawText();
			}
			else if (IsEng(rightChar) != 1)
			{
				mx += 2 * CNFONTWIDTH;							//��������ƶ�����ǰ�±�����
				Display();
				myDrawText();
			}
			break;
		case VK_BACK: if (NowListPos == 0)	break;				//�˸񣬹������,�����Ͻ�ֱ���˳�
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
					mx -= chWidth;							//��������ƶ�����ǰ�±�����
					Display();
					myDrawText();							//ˢ����ʾ����Ϊ�����ƶ������ˢ�»���֮ǰ��λ�����ºۼ�
				}
				else if (IsEng(leftChar) != 1)
				{
					mx -= 2 * CNFONTWIDTH;							//��������ƶ�����ǰ�±�����
					Display();
					myDrawText();									//ˢ����ʾ����Ϊ�����ƶ������ˢ�»���֮ǰ��λ�����ºۼ�
				}
			}

			break;
		case VK_UP:
			if (NowListPos == 0) break;
			i = NowListPos;
			leftChar = result[i - 1];
			leftStr[0] = result[i - 1];
			leftStr[1] = '\0';

			if (my == TXTFIRSTY)								//�����������ƶ��������Ͻ�
			{
				mx = TXTFIRSTX;
				upAccess = 1;
				break;
			}
			else if (leftChar == '\r' || leftChar == '\n')		//��������
			{
				my += chHeight;
				break;
			}
			else												//�������е�һ���м䣬���Ƶ�����
			{
				while (leftChar != '\n'&& leftChar != '\r')
				{
					if (IsEng(leftChar) == 1)
					{
						leftStr[0] = result[i - 1];
						leftStr[1] = '\0';
						chWidth = TextStringWidth(leftStr);
						mx -= chWidth;							//��������ƶ�����ǰ�±�����
						Display();
						myDrawText();
						i--;
						leftChar = result[i - 1];
					}
					else if (IsEng(leftChar) != 1)
					{
						mx -= 2 * CNFONTWIDTH;							//��������ƶ�����ǰ�±�����
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
			while (rightChar != 0 && rightChar != '\n'&& rightChar != '\r')			//��֤1.�Ƿ��Ѿ�����ĩ��2.��֤�Ұ��¼�����û����һ��
			{

				if (IsEng(rightChar) == 1)
				{
					rightStr[0] = result[i];
					rightStr[1] = '\0';
					chWidth = TextStringWidth(rightStr);
					mx += chWidth;							//��������ƶ�����ǰ�±�����
					Display();
					myDrawText();
					i++;
					rightChar = result[i];
				}
				else if (IsEng(rightChar) != 1)
				{
					mx += 2 * CNFONTWIDTH;							//��������ƶ�����ǰ�±�����
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



	/*�ַ�����㶨λ*/
	switch (event)
	{
		char rightChar; char leftChar;					//���ڴ��������ַ�
	case KEY_DOWN:
		switch (key)
		{
		case VK_LEFT:
			if (NowListPos == 0)
				break;								//�����Ͻǣ������������ƶ�
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
			DeleteText(1);				//ɾ��Ԫ�أ��õ�StringFunction������DeleteText����
			strcpy(result, now_result);
			SetEraseMode(0);
			myDrawText();
			break;
		case VK_DELETE: DeleteText(0);				//ɾ��Ԫ�أ��õ�StringFunction������DeleteText����
			strcpy(result, now_result);
			break;
		case VK_UP:if (NowListPos == 0) break;
			leftChar = result[NowListPos - 1];
			if (my == TXTFIRSTY && upAccess == 1)								//�����������ƶ��������Ͻ�
			{
				NowListPos = 0;
				lineStr[lineCount] = 0;
				upAccess = 0;
			}
			else if (leftChar == '\r' || leftChar == '\n')		//��������
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

			else												//�������е�һ���м䣬���Ƶ�����
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
				while (result[NowListPos] != 0 && result[NowListPos] != '\n'&& result[NowListPos] != '\r')			//��֤1.�Ƿ��Ѿ�����ĩ��2.��֤�Ұ��¼�����û����һ��
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
	��ݼ����ܣ�
	1��ˢ����ʾ
	2���ظ�
	3������
	4���滻
	*/
	switch (event)
	{
	case KEY_DOWN:
		switch (key)
		{
		case VK_F5:
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
			break;
		}
	case BUTTON_UP:
		break;
	}

	Display();
	DrawTextEditor();
	myDrawText();
}


// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	int i = 0;
	uiGetMouse(x, y, button, event);					//GUI��ȡ��꣨���ù��������ǵ����ֻ���ڲ˵�ѡ��
	switch (event)
	{
	case BUTTON_DOWN:MouseInBox = 1;					//�������ʾ���
		void SignalJudge(x, y);							//����SignalJudge�ƶ����
		break;				
	}
	//if(event!=MOUSEMOVE)
	{
		Display();
		myDrawText();
	}


	//��֤���ұ�ǲ�����ʧ
	while (choice[i][0] != 0) 
	{
		MovePen(choice[i][0], choice[i][1]);
		SetPenColor("Violet");
		DrawLine(0.15, 0);
		SetPenColor("Blue");
		i++;
	}

}
// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	if (IsCtrlPressed == 0)
	{
		uiGetChar(ch);
		MouseInBox = 0;
		/*
		˵����Ϊ�˷����ƶ���꣬���Ǵ�����һ��int�����飬�����±����������ַ���ţ�
		ÿһ������Ԫ�ش����˵�ǰ�ַ������ͣ�����3/Ӣ��1/���лس�2��
		lineStr��������ɾ���ַ�����ɾ��ʱ��ǰ����Ԫ��-1������Ϊ��ʱ����-1����������ƶ�
		*/
		/*�ַ������봢��*/
		if (ch == '\b')
		{
			MouseInBox = 1;
			Display();
		}
		else if (IsEng(ch) == 1)							//����Ӣ��
		{
			AddText(ch);						//�ַ��ص���ch������result������
			NowListPos++;
			maxPos++;
			totalChar++;
			MouseInBox = 1;
			Display();
			DrawTextEditor();
			myDrawText();
		}
		else if (IsEng(ch) != 1)							//��������
		{
			AddText(ch);									//�ַ��ص���ch������result������
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
		/*�����Զ�׷��*/
		if (ch == '\b')
		{
			Display();
		}
		else if (IsEng(ch) == 1)
		{
			double chHeight = GetFontHeight();				//�ַ��߶�
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
			mx += CNFONTWIDTH;								//�����ַ����
		}

		/*�ַ�ͳ��*/

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
		else if (IsEng(ch) == 0)					//�������ģ���ִ������ch�Ķ��룬����ֻ��Ҫ��ִ��һ�Σ�cnOnlyDoOnce���������ִֻ��һ��
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


//�û���ʱ����Ӧ����,��˸���
void TimeEventProcess(int timerID)
{
	if (MouseInBox)
	{
		switch (timerID)
	case 1:SetEraseMode(isDisplayGB);
		DrawGB(mx, my);										//�ڣ�mx��my���������
		isDisplayGB = !isDisplayGB;							//ʵ����˸
	}
	if (timerID == 2) {//���ڲ��Һ�������
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


//�����ƺ���,x,y����ǰ�����ַ������½�
void DrawGB(double x, double y)
{
	double fontHeight = GetFontHeight();
	MovePen(x, y);
	DrawLine(0, fontHeight);
}
