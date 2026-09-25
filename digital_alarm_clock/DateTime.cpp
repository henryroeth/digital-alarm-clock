#include "DateTime.h"
#include "Globals.h"

long daysFromCivil(int y, int m, int d) {
  y -= (m <= 2);
  long era = (y >= 0 ? y : y - 399) / 400;
  unsigned yoe = (unsigned)(y - era * 400);
  unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097L + (long)doe - 719468L;
}

unsigned long makeEpoch(int y, int mo, int d, int h, int mi, int s) {
  return (unsigned long)(daysFromCivil(y, mo, d) * 86400L) + h * 3600UL + mi * 60UL + s;
}

struct tm* nowParts() {
  time_t t = (time_t)epochSeconds;
  return gmtime(&t);
}

bool isLeap(int y) {
  return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int daysInMonth(int y, int m) {
  static const int dm[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (m == 2 && isLeap(y)) return 29;
  return dm[m - 1];
}