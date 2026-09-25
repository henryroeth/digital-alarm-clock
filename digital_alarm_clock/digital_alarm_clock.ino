#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

#include "Globals.h"
#include "Buttons.h"
#include "DateTime.h"
#include "Display.h"
#include "Alarm.h"
#include "UI.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

const uint8_t LED_PIN    = 3;
const uint8_t MENU_PIN   = 4;
const uint8_t UP_PIN     = 5;
const uint8_t DOWN_PIN   = 6;
const uint8_t SELECT_PIN = 7;
const uint8_t BUZZER_PIN = 8;

const int DEF_YEAR  = 2026;
const int DEF_MONTH = 1;
const int DEF_DAY   = 1;
const int DEF_HOUR  = 12;
const int DEF_MIN   = 0;
const int DEF_ALARM_HOUR = 12;
const int DEF_ALARM_MIN  = 1;

unsigned long epochSeconds = 0;
unsigned long lastTickMs   = 0;

bool alarmEnabled = false;
int  alarmHour    = DEF_ALARM_HOUR;
int  alarmMinute  = DEF_ALARM_MIN;
bool alarmRinging = false;
bool alarmFiredThisMinute = false;
unsigned long alarmStartMs = 0;
unsigned long lastBlinkMs  = 0;
bool ledOn = false;

const unsigned long ALARM_TIMEOUT_MS = 120000UL;
const unsigned long BLINK_INTERVAL   = 100;

UiState uiState = STATE_CLOCK;

const int MENU_COUNT = 5;
int menuIndex  = 0;
int menuScroll = 0;

int editField = 0;
int editHour, editMinute;
int editYear, editMonth, editDay;

bool needsRedraw = true;
int  lastShownMinute = -1;

const char* dayNames[]   = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
const char* monthNames[] = {"Jan","Feb","Mar","Apr","May","Jun",
                            "Jul","Aug","Sep","Oct","Nov","Dec"};

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(MENU_PIN,   INPUT_PULLUP);
  pinMode(UP_PIN,     INPUT_PULLUP);
  pinMode(DOWN_PIN,   INPUT_PULLUP);
  pinMode(SELECT_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  epochSeconds = makeEpoch(DEF_YEAR, DEF_MONTH, DEF_DAY, DEF_HOUR, DEF_MIN, 0);
  lastTickMs = millis();

  lcdLine(0, centered("Clock ready"));
  lcdLine(1, centered("MENU to set time"));
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();
  while (now - lastTickMs >= 1000UL) {
    epochSeconds++;
    lastTickMs += 1000UL;
  }

  pollButtons();

  if (alarmRinging && anyButtonPressed()) {
    stopAlarm();
  } else {
    switch (uiState) {
      case STATE_CLOCK:     handleClockInput();         break;
      case STATE_MENU:      handleMenuInput();          break;
      case STATE_SET_TIME:  handleTimeEditInput(false); break;
      case STATE_SET_DATE:  handleDateEditInput();      break;
      case STATE_SET_ALARM: handleTimeEditInput(true);  break;
    }
  }

  serviceAlarm();

  struct tm* t = nowParts();
  if (uiState == STATE_CLOCK && t->tm_min != lastShownMinute) {
    lastShownMinute = t->tm_min;
    needsRedraw = true;
  }

  if (needsRedraw) {
    redraw();
    needsRedraw = false;
  }
}
