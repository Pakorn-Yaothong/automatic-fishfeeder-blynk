# Automatic Fish Feeder (ESP8266 + Blynk + LINE API)

An IoT-based **Automatic Fish Feeder** project using **ESP8266 (NodeMCU)**, controlled and monitored via **Blynk mobile app**.  
Supports both **manual feeding** and **automatic interval feeding** with real-time feedback through the app and physical LEDs.  
Automatically sends a **LINE message** after each successful feeding operation.

## Features

- Manual feeding via Blynk app button (V1)
- Automatic feeding based on interval timer (V2 - in hours)
- Real-time status LEDs (V3 "Feeding", V4 "Idle")
- Servo motor rotates to dispense food (0 → 180 → 0)
- LINE notification sent after feeding
- Simple implementation using `millis()` timing (no RTC required)
- Physical LED indicators (D4, D5 pins)

## Limitations

- No real-time clock (RTC); interval-based, not time-of-day based
- No food level detection
- No feeding history logging

## Author

Pakorn Yaothong  
[GitHub: Pakorn-Yaothong](https://github.com/Pakorn-Yaothong)
