# Light Awake

This is both an alarm that simulates a sunrise and a night light. Less of a passion project and more of a necessity, this project was born out of my despise of being jolted awake by obnoxious audio alarms and tripping in the dark in the middle of the night.

![Status](https://img.shields.io/badge/status-beta-brightgreen)
![Arduino Nano](https://img.shields.io/badge/Arduino-Nano-00979D?logo=Arduino)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
<!--![Tests](https://img.shields.io/badge/tests-%20none-lightgrey)-->

---

## Features and Status

- wake-up: simulates a sunrise (ideal during times of the year with reduced daylight hours)
- night light: LEDs provide soft red lighting upon motion activation

No unit testing has been done, but it functionally works in the real world.

---

## Hardware:
- Microcontroller: Arduino Nano
- RTC module: DS3231
- Motion sensor: HC-SR501 PIR
- LED strip: 5050 24V RGBW double row
- MOSFETs: IRLZ44N 47A 55V
- Buck converter: 5A 4-38V to 1.25-36V

---

## Usage

1. Set the clock time by uncommenting line 41 in the code.
2. Set parameters in the code (lines 22-28):
   - alarm time (hour and minute) start
   - alarm time duration (number of minutes of alarm lighting)
   - night light time (hour and minute) start
   - night light duration (number of seconds until lighting fades away)

NOTE: The night light will be triggered only during hours from the night light start time until the alarm time.

---

## Project Info
**Author:** T. Stratton  
**Start Date:** 10-JAN-2025  
**License:** MIT License – see [LICENSE](./LICENSE)  
**Language:** Arduino  
**Status:** Beta (manually tested and working reliably)  