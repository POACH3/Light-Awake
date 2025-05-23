/*
  Light-based alarm clock with a motion activated night light.
*/

#include <Wire.h>
#include <RTClib.h>
#include <SoftPWM.h>

RTC_DS3231 rtc;

const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;
const int whitePin = 5;

const int motionPin = 6;
//const int motionLed = 2;

DateTime now;
DateTime alarmTimer;
DateTime motionTimer;
int alarmHour = 5;
int alarmMinute = 30;
int nightlightBeginHour = 22;
bool alarmTriggered = false;
bool motion = false;
int timerMinutes = 20;

void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // set the time only once (comment out after initial use)
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  }

  SoftPWMBegin();

  SoftPWMSet(whitePin, 0);
  SoftPWMSet(redPin, 0);
  SoftPWMSet(greenPin, 0);
  SoftPWMSet(bluePin, 0);

  SoftPWMSetFadeTime(whitePin, 4000, 4000);
  SoftPWMSetFadeTime(redPin, 4000, 4000);
  SoftPWMSetFadeTime(greenPin, 4000, 4000);
  SoftPWMSetFadeTime(bluePin, 4000, 4000);

  pinMode(motionPin, INPUT);
}

void loop() {
  now = rtc.now();

  if (now.hour() == alarmHour && now.minute() == alarmMinute && !alarmTriggered) {
    alarmTriggered = true;
    sunrise();
    alarmTimer = setTimer(timerMinutes * 60);
  }

  if (now.hour() == alarmTimer.hour() && now.minute() == alarmTimer.minute()) {
    alarmTriggered = false;
    sunset();
  }

  int motionDetected = digitalRead(motionPin);

  if (motionDetected == HIGH) { // Motion detected
    Serial.println("Motion detected!");
    motion = true;
  } else {
    Serial.println("---");
    motion = false;
  }
  delay(100); // debounce the sensor

  if (motion && !alarmTriggered) {
    if ( (now.hour() > nightlightBeginHour) && (now.hour() < alarmHour) && (now.minute() < alarmMinute) ) {
      nightLight();
      motionTimer = setTimer(30);
    }
  }

  if (now == motionTimer && !alarmTriggered) {
    sunset();
  }
}

void sunrise() {
  SoftPWMSetPercent(whitePin, 50);
  SoftPWMSetPercent(redPin, 0);
  SoftPWMSetPercent(greenPin, 0);
  SoftPWMSetPercent(bluePin, 40);
  delay(5000);

  SoftPWMSetPercent(whitePin, 100);
  SoftPWMSetPercent(bluePin, 100);
}

void sunset() {
  SoftPWMSetPercent(whitePin, 0);
  SoftPWMSetPercent(redPin, 0);
  SoftPWMSetPercent(greenPin, 0);
  SoftPWMSetPercent(bluePin, 0);
}

void nightLight() {
  SoftPWMSetPercent(redPin, 40);
}

DateTime setTimer(int seconds) {
  uint32_t unixTime = now.unixtime();
  uint32_t unixAlarmTime = unixTime + seconds;
  return DateTime(unixAlarmTime);
}

void timeUp() {

}
