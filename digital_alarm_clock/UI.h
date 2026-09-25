#pragma once

void handleClockInput();
void handleMenuInput();
void handleTimeEditInput(bool isAlarm);
void handleDateEditInput();

void loadTimeForEdit();
void saveEditedTime();
void loadDateForEdit();
void saveEditedDate();
void adjustTimeField(int delta);
void adjustDateField(int delta);