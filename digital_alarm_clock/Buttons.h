#pragma once
#include <Arduino.h>

struct Button {
  uint8_t pin;
  bool    stable;
  bool    lastReading;
  unsigned long lastChangeMs;
  unsigned long lastRepeatMs;
  bool    allowRepeat;
};

enum { BTN_MENU = 0, BTN_UP = 1, BTN_DOWN = 2, BTN_SELECT = 3 };

extern Button buttons[4];
extern bool pressedEvent[4];

void pollButtons();
bool anyButtonPressed();