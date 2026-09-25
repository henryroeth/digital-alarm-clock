#include "Display.h"
#include "Globals.h"
#include "DateTime.h"

String two(int n) {
  return (n < 10) ? "0" + String(n) : String(n);
}

String fmt12(int h24, int m) {
  String ampm = (h24 >= 12) ? "PM" : "AM";
  int h12 = h24 % 12;
  if (h12 == 0) h12 = 12;
  return String(h12) + ":" + two(m) + " " + ampm;
}

void lcdLine(int row, String text) {
  while (text.length() < 20) text += ' ';
  if (text.length() > 20) text = text.substring(0, 20);
  lcd.setCursor(0, row);
  lcd.print(text);
}

String centered(String s) {
  int pad = (20 - (int)s.length()) / 2;
  if (pad < 0) pad = 0;
  String out = "";
  for (int i = 0; i < pad; i++) out += ' ';
  return out + s;
}

void drawClock() {
  struct tm* t = nowParts();

  lcdLine(0, centered(String(dayNames[t->tm_wday]) + ", " +
                      String(monthNames[t->tm_mon]) + " " +
                      String(t->tm_mday) + " " +
                      String(t->tm_year + 1900)));

  lcdLine(1, centered(fmt12(t->tm_hour, t->tm_min)));

  if (alarmRinging) {
    lcdLine(2, centered("** ALARM **"));
    lcdLine(3, centered("any button = stop"));
  } else if (alarmEnabled) {
    lcdLine(2, "Alarm: " + fmt12(alarmHour, alarmMinute) + "  ON");
    lcdLine(3, "MENU = settings");
  } else {
    lcdLine(2, "Alarm: OFF");
    lcdLine(3, "MENU = settings");
  }
}

void drawMenu() {
  String items[MENU_COUNT];
  items[0] = "Set Time";
  items[1] = "Set Date";
  items[2] = "Set Alarm";
  items[3] = String("Alarm: ") + (alarmEnabled ? "ON" : "OFF");
  items[4] = "Exit";

  if (menuIndex < menuScroll)     menuScroll = menuIndex;
  if (menuIndex > menuScroll + 3) menuScroll = menuIndex - 3;

  for (int row = 0; row < 4; row++) {
    int item = menuScroll + row;
    if (item >= MENU_COUNT) {
      lcdLine(row, "");
    } else {
      lcdLine(row, (item == menuIndex ? "> " : "  ") + items[item]);
    }
  }
}

void drawSetTime(const char* title) {
  lcdLine(0, centered(title));

  int h12 = editHour % 12;
  if (h12 == 0) h12 = 12;
  lcdLine(1, "      " + two(h12) + ":" + two(editMinute) + " " +
             ((editHour >= 12) ? "PM" : "AM"));

  String caret = "                    ";
  int col = (editField == 0) ? 6 : (editField == 1) ? 9 : 12;
  caret.setCharAt(col, '^');
  caret.setCharAt(col + 1, '^');
  lcdLine(2, caret);

  lcdLine(3, "SEL=next MENU=save");
}

void drawSetDate() {
  lcdLine(0, centered("Set Date"));
  lcdLine(1, "   " + String(monthNames[editMonth - 1]) + " " +
             two(editDay) + " " + String(editYear));

  String caret = "                    ";
  int col   = (editField == 0) ? 3 : (editField == 1) ? 7 : 10;
  int width = (editField == 0) ? 3 : (editField == 1) ? 2 : 4;
  for (int i = 0; i < width; i++) caret.setCharAt(col + i, '^');
  lcdLine(2, caret);

  lcdLine(3, "SEL=next MENU=save");
}

void redraw() {
  switch (uiState) {
    case STATE_CLOCK:     drawClock();              break;
    case STATE_MENU:      drawMenu();               break;
    case STATE_SET_TIME:  drawSetTime("Set Time");  break;
    case STATE_SET_DATE:  drawSetDate();            break;
    case STATE_SET_ALARM: drawSetTime("Set Alarm"); break;
  }
}