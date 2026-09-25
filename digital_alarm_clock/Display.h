#pragma once
#include <Arduino.h>

String two(int n);
String fmt12(int h24, int m);
void lcdLine(int row, String text);
String centered(String s);

void drawClock();
void drawMenu();
void drawSetTime(const char* title);
void drawSetDate();
void redraw();