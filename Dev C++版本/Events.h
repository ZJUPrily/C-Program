#ifndef EVENTS_H
#define EVENTS_H

#define TXTFIRSTX 0.12
#define TXTFIRSTY 6.39
#define CNFONTWIDTH 0.0782
#define MAXNUM   1000
void DrawGB(double x, double y);//�����ƺ���
void KeyboardEventProcess(int key, int event);
void MouseEventProcess(int x, int y, int button, int event);
void CharEventProcess(char ch);
void TimeEventProcess(int timerID);
void DrawGB(double x, double y);
#endif

