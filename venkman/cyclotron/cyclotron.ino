// Simple cyclotron lighting animation. Designed for an Arduino Nano.
#include <Arduino_FreeRTOS.h>
#include <timers.h>

int cyclotronLedPin[] = {3, 5, 6, 9};
int cyclotronLedBrightness[] = {0, 0, 0, 0};
int cyclotronLedState[]= {1,0,0,0}; // 0 = off, 1 = turning on, 2 = on, 3 = turning off
const int CYCLOTRON_LED_ON_TIME = 1000;  // ms
const float CYCLOTRON_LED_WARM_UP_TIME = 100.0; //ms
const float CYCLOTRON_LED_COOL_DOWN_TIME = 200.0; //ms
const float UPDATE_INTERVAL = 10.0; // Update every 10ms
TimerHandle_t updateTimer;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i ++) {
    pinMode(cyclotronLedPin[i], OUTPUT);
  }

  updateTimer = xTimerCreate("UpdateTimer", pdMS_TO_TICKS(UPDATE_INTERVAL), pdTRUE, (void*)0, updateTimerCallback);
  xTimerStart(updateTimer, 0);  
  vTaskStartScheduler();  
}

void loop() {}

void updateTimerCallback(TimerHandle_t xTimer) {
  for (int i = 0; i < 4; i ++) {
    updateCyclotronLedState(i);
    updateCyclotronLedBrightness(i);
  }
}

int getFadeDeltaForCyclotronLed(int index) {
  int state = cyclotronLedState[index];
  int currentBrightness = cyclotronLedBrightness[index];
  
  if (state == 0 && currentBrightness == 0) return 0;
  if (state == 1 && currentBrightness < 255) return 255 * UPDATE_INTERVAL / CYCLOTRON_LED_WARM_UP_TIME;
  if (state == 1 && currentBrightness >= 255) return 0;
  if (state == 0 && currentBrightness > 0) return -255 * UPDATE_INTERVAL / CYCLOTRON_LED_COOL_DOWN_TIME;
  
  return 0;
}

void updateCyclotronLedBrightness(int index) {
  int brightness = cyclotronLedBrightness[index] + getFadeDeltaForCyclotronLed(index);
  Serial.println(brightness); // load bearing

  cyclotronLedBrightness[index] = constrain(brightness, 0, 255);

  analogWrite(cyclotronLedPin[index], cyclotronLedBrightness[index]);
}

void updateCyclotronLedState(int index) {
  int ledThatShouldBeOn = (millis() / CYCLOTRON_LED_ON_TIME) % 4;
  bool shouldThisLedBeOn = ledThatShouldBeOn == index;
  
  if (cyclotronLedState[index] == 0 && shouldThisLedBeOn) cyclotronLedState[index] = 1;
  if (cyclotronLedState[index] == 1 && !shouldThisLedBeOn) cyclotronLedState[index] = 0;
}

