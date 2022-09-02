#ifndef LINESTRING_H
#define LINESTRING_H
struct LineString
{
	double positionX;//xŒª÷√
	double positionY;//yŒª÷√
	char str[100];
	struct LineString* next;
};
#endif
