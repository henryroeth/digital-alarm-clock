#include <Arduino.h>
#include "UI.h"
#include "Globals.h"
#include "Buttons.h"
#include "DateTime.h"

void loadTimeForEdit() {
  struct tm* t = nowParts();
  editHour   = t->tm_hour;
  editMinute = t->tm_min;
  editField  = 0;
}

void saveEditedTime() {
  struct tm* t = nowParts();
  epochSeconds = makeEpoch(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                           editHour, editMinute, 0);
  lastTickMs = millis();
}

void loadDateForEdit() {
  struct tm* t = nowParts();
  editYear  = t->tm_year + 1900;
  editMonth = t->tm_mon + 1;
  editDay   = t->tm_mday;
  editField = 0;
}

void saveEditedDate() {
  struct tm* t = nowParts();
  int h = t->tm_hour, mi = t->tm_min, s = t->tm_sec;
  if (editDay > daysInMonth(editYear, editMonth)) {
    editDay = daysInMonth(editYear, editMonth);
  }
  epochSeconds = makeEpoch(editYear, editMonth, editDay, h, mi, s);
  lastTickMs = millis();
}

void adjustTimeField(int delta) {
  if (editField == 0) {
    editHour = (editHour + delta + 24) % 24;
  } else if (editField == 1) {
    editMinute = (editMinute + delta + 60) % 60;
  } else {
    editHour = (editHour + 12) % 24;
  }
}

void adjustDateField(int delta) {
  if (editField == 0) {
    editMonth += delta;
    if (editMonth < 1)  editMonth = 12;
    if (editMonth > 12) editMonth = 1;
  } else if (editField == 1) {
    int maxDay = daysInMonth(editYear, editMonth);
    editDay += delta;
    if (editDay < 1)      editDay = maxDay;
    if (editDay > maxDay) editDay = 1;
  } else {
    editYear += delta;
    if (editYear < 2000) editYear = 2000;
    if (editYear > 2099) editYear = 2099;
  }

  if (editDay > daysInMonth(editYear, editMonth)) {
    editDay = daysInMonth(editYear, editMonth);
  }
}

void handleClockInput() {
  if (pressedEvent[BTN_MENU]) {
    uiState    = STATE_MENU;
    menuIndex  = 0;
    menuScroll = 0;
    needsRedraw = true;
  }
}

void handleMenuInput() {
  if (pressedEvent[BTN_UP]) {
    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
    needsRedraw = true;
  }
  if (pressedEvent[BTN_DOWN]) {
    menuIndex = (menuIndex + 1) % MENU_COUNT;
    needsRedraw = true;
  }
  if (pressedEvent[BTN_MENU]) {
    uiState = STATE_CLOCK;
    needsRedraw = true;
  }
  if (pressedEvent[BTN_SELECT]) {
    switch (menuIndex) {
      case 0:
        loadTimeForEdit();
        uiState = STATE_SET_TIME;
        break;
      case 1:
        loadDateForEdit();
        uiState = STATE_SET_DATE;
        break;
      case 2:
        editHour   = alarmHour;
        editMinute = alarmMinute;
        editField  = 0;
        uiState = STATE_SET_ALARM;
        break;
      case 3:
        alarmEnabled = !alarmEnabled;
        break;
      case 4:
        uiState = STATE_CLOCK;
        break;
    }
    needsRedraw = true;
  }
}

void handleTimeEditInput(bool isAlarm) {
  if (pressedEvent[BTN_UP])   { adjustTimeField(+1); needsRedraw = true; }
  if (pressedEvent[BTN_DOWN]) { adjustTimeField(-1); needsRedraw = true; }

  if (pressedEvent[BTN_SELECT]) {
    editField = (editField + 1) % 3;
    needsRedraw = true;
  }

  if (pressedEvent[BTN_MENU]) {
    if (isAlarm) {
      alarmHour    = editHour;
      alarmMinute  = editMinute;
      alarmEnabled = true;
      alarmFiredThisMinute = false;
    } else {
      saveEditedTime();
    }
    uiState = STATE_MENU;
    needsRedraw = true;
  }
}

void handleDateEditInput() {
  if (pressedEvent[BTN_UP])   { adjustDateField(+1); needsRedraw = true; }
  if (pressedEvent[BTN_DOWN]) { adjustDateField(-1); needsRedraw = true; }

  if (pressedEvent[BTN_SELECT]) {
    editField = (editField + 1) % 3;
    needsRedraw = true;
  }

  if (pressedEvent[BTN_MENU]) {
    saveEditedDate();
    uiState = STATE_MENU;
    needsRedraw = true;
  }
}