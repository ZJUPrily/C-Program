#ifndef LINESTRING_H
#define LINESTRING_H
struct LineString
{
	double positionX;//xλ��
	double positionY;//yλ��
	char str[100];
	struct LineString* next;
};
#endif
