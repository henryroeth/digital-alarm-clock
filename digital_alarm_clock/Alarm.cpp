#include <Arduino.h>
#include "Alarm.h"
#include "Globals.h"
#include "DateTime.h"

const int NOTE_CS4 = 277;
const int NOTE_D4  = 294;
const int NOTE_E4  = 330;
const int NOTE_FS4 = 370;
const int NOTE_GS4 = 415;
const int NOTE_A4  = 440;
const int NOTE_B4  = 494;
const int NOTE_CS5 = 554;
const int NOTE_D5  = 587;
const int NOTE_E5  = 659;
const int REST     = 0;

const int melodyNotes[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4,
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4,
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4, REST
};
const int melodyDurations[] = {
  125, 125, 250, 250,
  125, 125, 250, 250,
  125, 125, 250, 250,
  500, 400
};
const int MELODY_LENGTH = sizeof(melodyNotes) / sizeof(melodyNotes[0]);

int melodyIndex = 0;
unsigned long melodyNoteStartMs = 0;

void serviceMelody(unsigned long now) {
  if (now - melodyNoteStartMs >= (unsigned long)melodyDurations[melodyIndex]) {
    melodyIndex = (melodyIndex + 1) % MELODY_LENGTH;
    melodyNoteStartMs = now;

    if (melodyNotes[melodyIndex] == REST) {
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melodyNotes[melodyIndex]);
    }
  }
}

void serviceAlarm() {
  struct tm* t = nowParts();
  bool atAlarmMinute = (t->tm_hour == alarmHour && t->tm_min == alarmMinute);

  if (alarmEnabled && !alarmRinging && atAlarmMinute && !alarmFiredThisMinute) {
    alarmRinging = true;
    alarmFiredThisMinute = true;
    alarmStartMs = millis();
    melodyIndex = 0;
    melodyNoteStartMs = millis();
    tone(BUZZER_PIN, melodyNotes[0]);
    uiState = STATE_CLOCK;
    needsRedraw = true;
  }

  if (!atAlarmMinute) {
    alarmFiredThisMinute = false;
  }

  if (alarmRinging) {
    unsigned long now = millis();

    if (now - lastBlinkMs >= BLINK_INTERVAL) {
      lastBlinkMs = now;
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }

    serviceMelody(now);

    if (now - alarmStartMs > ALARM_TIMEOUT_MS) {
      stopAlarm();
    }
  }
}

void stopAlarm() {
  alarmRinging = false;
  ledOn = false;
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  needsRedraw = true;
}