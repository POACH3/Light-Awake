/*
  Light-based alarm clock with a motion activated nightlight.
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
int alarmHour = 5;            // the hour that alarm lighting activates
int alarmMinute = 30;         // the minute that alarm lighting activates
int timerMinutes = 20;        // the number of minutes the alarm light stays on
bool alarmTriggered = false;

int nightlightBeginHour = 22; // the hour that motion lighting can be activated
bool motion = false;


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

/*
Turn on alarm lighting.
*/
void sunrise() {
  SoftPWMSetPercent(whitePin, 50);
  SoftPWMSetPercent(redPin, 0);
  SoftPWMSetPercent(greenPin, 0);
  SoftPWMSetPercent(bluePin, 40);
  delay(5000);

  SoftPWMSetPercent(whitePin, 100);
  SoftPWMSetPercent(bluePin, 100);
}

/*
Turn off alarm lighting.
*/
void sunset() {
  SoftPWMSetPercent(whitePin, 0);
  SoftPWMSetPercent(redPin, 0);
  SoftPWMSetPercent(greenPin, 0);
  SoftPWMSetPercent(bluePin, 0);
}

/*
Turn on night time lighting.
*/
void nightLight() {
  SoftPWMSetPercent(redPin, 40);
}

/*
Returns the time the number of seconds given from now.
*/
DateTime setTimer(int seconds) {
  uint32_t unixTime = now.unixtime();
  uint32_t unixAlarmTime = unixTime + seconds;
  return DateTime(unixAlarmTime);
}