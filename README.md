# ESP weather station

> "ESP weather station" — Smartwatch and Station on ESP8266 with time synchronization via NTP and Temperature in outside.

---

## Content

- [Description](#description)
- [Requirements](#requirements)
- [Installation](#installation)
- [Connecting_pins](#Connecting_pins)


---

## Description

Smartwatch on ESP and weather station. Weather gets throught yandex ID.


---

## Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- ESP8266 (NodeMCU, Wemos D1 Mini и др.)
- Libs are included in rar file 
- LCD Display 1602 with I2C

---

## Installation

- Install source code
- Open .ino file in arduino IDE 
- ESP core is 3.1.2
- Add libs to IDE 
- Flash

## Connecting_pins

- Display pins VCC - VIN or VBUS, GND - GND, SCL - D1 (GPIO5 - ESP8266), SDA -  D2 (GPIO4 - ESP8266).
- Power to ESP 8266 trught VIN (+) and GND (-).
- Strongly recomend to use PSU from phone or other 5 Volts and 2 Ampers.
