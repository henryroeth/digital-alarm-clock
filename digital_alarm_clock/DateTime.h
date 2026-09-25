#pragma once
#include <time.h>

long daysFromCivil(int y, int m, int d);
unsigned long makeEpoch(int y, int mo, int d, int h, int mi, int s);
struct tm* nowParts();
bool isLeap(int y);
int daysInMonth(int y, int m);