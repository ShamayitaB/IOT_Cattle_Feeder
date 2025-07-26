// -----------------------------------------------------------
// Smart Feeder Project using NodeMCU, Blynk, IR, Servo, and DS18B20
// Author: ShamayitaB
// Description: This sketch controls a servo motor for feeding based on IR sensor detection,
//              sends temperature data from a DS18B20 sensor, and reads weight from a real HX711 load cell.
// Dependencies: Blynk, ESP8266WiFi, Servo, OneWire, DallasTemperature, HX711
// -----------------------------------------------------------

// Replace with your own Blynk Template ID and Auth Token
#define BLYNK_TEMPLATE_ID           "YourTemplateID"
#define BLYNK_TEMPLATE_NAME         "NodeMCU"
#define BLYNK_AUTH_TOKEN            "YourAuthToken"

// Enable Serial Monitor for debugging
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>

// WiFi Credentials (Replace with your own in secrets.h or input manually if private repo)
char ssid[] = "YourSSID";
char pass[] = "YourPassword";

// Pin Definitions
#define LED_PIN            D0
#define IR_SENSOR_PIN      D2
#define SERVO_PIN          D5
#define ONE_WIRE_BUS       D1   // DS18B20 data pin
#define HX711_DOUT         D6
#define HX711_SCK          D7

// Blynk Virtual Pins
#define TEMP_VPIN          V0
#define WEIGHT_VPIN        V1
#define STATUS_VPIN        V5
#define IMAGE_WIDGET_VPIN  V3

// Temperature Sensor
DallasTemperature sensors(new OneWire(ONE_WIRE_BUS));
#define TEMPERATURE_PRECISION 12

// Servo Motor
Servo servo;

// Load Cell
HX711 scale;
#define CALIBRATION_FACTOR -7050.0 // Set this based on calibration

// Blynk Timer
BlynkTimer timer;

// Blynk Setup
BLYNK_CONNECTED() {
  Blynk.setProperty(IMAGE_WIDGET_VPIN, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(IMAGE_WIDGET_VPIN, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(IMAGE_WIDGET_VPIN, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// Send temperature to Blynk
void sendTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature (C): ");
  Serial.println(temperatureC);
  Blynk.virtualWrite(TEMP_VPIN, temperatureC);
}

// Send real weight to Blynk
void sendWeight() {
  float weight = scale.get_units();
  Serial.print("Weight: ");
  Serial.println(weight);
  Blynk.virtualWrite(WEIGHT_VPIN, weight);
}

// Servo control functions
void rotateServo() {
  Blynk.virtualWrite(STATUS_VPIN, "Feeding!");
  servo.attach(SERVO_PIN);
  servo.write(135);  // Rotate to feed position
}

void returnServo() {
  servo.write(0);    // Return to default
}

void showWaitingMessage() {
  Blynk.virtualWrite(STATUS_VPIN, "Waiting...");
}

void setup() {
  Serial.begin(9600);
  sensors.begin();

  scale.begin(HX711_DOUT, HX711_SCK);
  scale.set_scale(CALIBRATION_FACTOR);
  scale.tare();  // Reset the scale to 0

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(IR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  timer.setInterval(10000L, sendTemperature);
  timer.setInterval(3000L, sendWeight);
}

void loop() {
  Blynk.run();
  timer.run();

  static bool previousDetection = false;
  bool currentDetection = digitalRead(IR_SENSOR_PIN);

  if (currentDetection != previousDetection) {
    previousDetection = currentDetection;
    if (currentDetection == LOW) {
      rotateServo();
      digitalWrite(LED_PIN, LOW);
    } else {
      returnServo();
      digitalWrite(LED_PIN, HIGH);
      showWaitingMessage();
    }
  }
}
