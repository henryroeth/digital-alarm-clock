#include "Buttons.h"
#include "Globals.h"

const unsigned long DEBOUNCE_MS     = 25;
const unsigned long REPEAT_DELAY_MS = 500;
const unsigned long REPEAT_RATE_MS  = 140;

Button buttons[4] = {
  {MENU_PIN,   false, false, 0, 0, false},
  {UP_PIN,     false, false, 0, 0, true },
  {DOWN_PIN,   false, false, 0, 0, true },
  {SELECT_PIN, false, false, 0, 0, false}
};

bool pressedEvent[4] = {false, false, false, false};

void pollButtons() {
  unsigned long now = millis();

  for (int i = 0; i < 4; i++) {
    pressedEvent[i] = false;
    bool reading = (digitalRead(buttons[i].pin) == LOW);

    if (reading != buttons[i].lastReading) {
      buttons[i].lastReading  = reading;
      buttons[i].lastChangeMs = now;
    }

    if (now - buttons[i].lastChangeMs > DEBOUNCE_MS && reading != buttons[i].stable) {
      buttons[i].stable = reading;
      if (reading) {
        pressedEvent[i] = true;
        buttons[i].lastRepeatMs = now + REPEAT_DELAY_MS;
      }
    }

    if (buttons[i].allowRepeat && buttons[i].stable && now >= buttons[i].lastRepeatMs) {
      pressedEvent[i] = true;
      buttons[i].lastRepeatMs = now + REPEAT_RATE_MS;
    }
  }
}

bool anyButtonPressed() {
  for (int i = 0; i < 4; i++) {
    if (pressedEvent[i]) return true;
  }
  return false;
}