# ECSE3038_lab6

## Overview
This project uses an ESP32 to interact with a remote API, simulating a temperature sensor and a LED controlled via API calls. The device:
- Retrieves the light state via a GET request (`/api/light`) and toggles the LED accordingly.
- Sends the current temperature via a PUT request (`/api/temp`).

## Purpose
This lab assignment demonstrates how to use an ESP32 to consume APIs by:
- Establishing WiFi connections
- Making HTTP GET and PUT requests
- Parsing JSON responses
- Controlling hardware (an LED) based on API responses
