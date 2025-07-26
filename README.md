# IOT_Cattle_Feeder

This project is an IoT-enabled smart feeder using NodeMCU ESP8266. It automatically dispenses food when a pet is detected using an IR sensor. It also monitors the remaining food weight using a load cell (HX711) and tracks ambient temperature with a DS18B20 sensor. The system is integrated with the Blynk platform for remote monitoring and feedback.

## Features

- Real-time temperature and weight monitoring
- Automatic food dispensing when motion is detected
- Load cell integration for measuring remaining food
- Remote interface using the Blynk mobile app
- Visual LED feedback for system activity

## Hardware Components

| Component              | Description                          |
|------------------------|--------------------------------------|
| NodeMCU ESP8266        | Wi-Fi microcontroller                |
| IR Obstacle Sensor     | Detects pet presence                 |
| SG90 Servo Motor       | Controls food dispensing mechanism   |
| DS18B20 Temperature Sensor | Measures ambient temperature     |
| HX711 Load Cell        | Tracks food weight                   |
| LED                    | Optional visual indicator            |
| Breadboard, jumper wires, power supply |

## Pin Configuration

| Module          | NodeMCU Pin |
|-----------------|-------------|
| IR Sensor       | D2          |
| Servo Motor     | D5          |
| DS18B20         | D1          |
| HX711 (DOUT)    | D6          |
| HX711 (SCK)     | D7          |
| LED             | D0          |

## Blynk Setup

1. Create a new project in the Blynk IoT app.
2. Add the following widgets and connect them to the corresponding virtual pins:
   - V0: Temperature Display
   - V1: Weight Display
   - V3: Image Button (optional)
   - V5: Status Display
3. Use the `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN` from the web dashboard in your code.

## HX711 Load Cell Calibration

Before using the load cell:
- Upload a simple calibration sketch to the NodeMCU
- Place a known weight on the scale and adjust the `CALIBRATION_FACTOR` in the code until the readings match the actual weight

## Folder Structure

