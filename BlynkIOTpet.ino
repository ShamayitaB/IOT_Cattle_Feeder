#define BLYNK_TEMPLATE_ID           "TMPL3jWaQKSKO"
#define BLYNK_TEMPLATE_NAME         "NodeMCU"
#define BLYNK_AUTH_TOKEN            "sampletoken"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char ssid[] = "WIFINAME"; // Your WiFi SSID
char pass[] = "verSE88&"; // Your WiFi password

#define LED D0

#define IR_SENSOR_PIN D2
#define SERVO_PIN D5

#define ONE_WIRE_BUS D1  // Pin for DS18B20 data
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Servo servo;
BlynkTimer timer;

BLYNK_CONNECTED() {
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void sendTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  Serial.print("Temperature (C): ");
  Serial.println(temperatureC);

  Blynk.virtualWrite(V0, temperatureC);
}

void rotateServo() {
  Blynk.virtualWrite(V5, "Feeding!");
  servo.attach(SERVO_PIN);
  servo.write(135);
}

void waitmsg() {
  Blynk.virtualWrite(V5, "Waiting...");
}

void returnServo() {
  servo.write(0);
}

void checkIRStatus() {
  static unsigned long lastDetectionTime = 0; // Store the time of the last detection
  unsigned long currentTime = millis(); // Get the current time
  
  if (digitalRead(IR_SENSOR_PIN) == LOW) {
    lastDetectionTime = currentTime;
  } else {
    if (currentTime - lastDetectionTime >= 4000) {
      waitmsg();
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initial DS18B20 value:");
  sensors.begin();
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(10000L, sendTemperature);
  timer.setInterval(1000L, checkIRStatus); 
  pinMode(IR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();

  static bool obstacleDetected = false;
  bool currentDetection = digitalRead(IR_SENSOR_PIN);

  if (currentDetection != obstacleDetected) {
    obstacleDetected = currentDetection;
    if (obstacleDetected) {
      rotateServo();
      digitalWrite(LED, LOW);
    } else {
      returnServo();
      digitalWrite(LED, HIGH);
    }
  }
}
