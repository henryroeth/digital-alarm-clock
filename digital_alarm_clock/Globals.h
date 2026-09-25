#pragma once
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- Pins ----------------
extern const uint8_t LED_PIN;
extern const uint8_t MENU_PIN;
extern const uint8_t UP_PIN;
extern const uint8_t DOWN_PIN;
extern const uint8_t SELECT_PIN;
extern const uint8_t BUZZER_PIN;

// ---------------- Power-on defaults ----------------
extern const int DEF_YEAR;
extern const int DEF_MONTH;
extern const int DEF_DAY;
extern const int DEF_HOUR;
extern const int DEF_MIN;
extern const int DEF_ALARM_HOUR;
extern const int DEF_ALARM_MIN;

// ---------------- LCD ----------------
extern LiquidCrystal_I2C lcd;

// ---------------- Timekeeping ----------------
extern unsigned long epochSeconds;
extern unsigned long lastTickMs;

// ---------------- Alarm state ----------------
extern bool alarmEnabled;
extern int  alarmHour;
extern int  alarmMinute;
extern bool alarmRinging;
extern bool alarmFiredThisMinute;
extern unsigned long alarmStartMs;
extern unsigned long lastBlinkMs;
extern bool ledOn;

extern const unsigned long ALARM_TIMEOUT_MS;
extern const unsigned long BLINK_INTERVAL;

// ---------------- UI state ----------------
enum UiState {
  STATE_CLOCK,
  STATE_MENU,
  STATE_SET_TIME,
  STATE_SET_DATE,
  STATE_SET_ALARM
};
extern UiState uiState;

extern const int MENU_COUNT;
extern int menuIndex;
extern int menuScroll;

extern int editField;
extern int editHour, editMinute;
extern int editYear, editMonth, editDay;

extern bool needsRedraw;
extern int  lastShownMinute;

extern const char* dayNames[];
extern const char* monthNames[];