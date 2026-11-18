#include <KiwisIoT.h>

const char* ssid = "wifi name";
const char* pass = "password";
const char* topic = "dash_1791206712742";

#define HEART_PIN A0

KiwisIoT kiwisiot(ssid, pass, topic);

int threshold = 550;
bool beatDetected = false;

unsigned long lastBeatTime = 0;
int bpm = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  kiwisiot.begin();

  Serial.println();
  Serial.println("Heart Rate Sensor Started");
}

void loop() {
  kiwisiot.run();

  int sensorValue = analogRead(HEART_PIN);

  // Detect heartbeat
  if (sensorValue > threshold && !beatDetected) {
    beatDetected = true;

    unsigned long currentTime = millis();

    if (lastBeatTime > 0) {
      unsigned long beatInterval = currentTime - lastBeatTime;

      // Calculate BPM
      bpm = 60000 / beatInterval;

      // Accept normal measurable range
      if (bpm >= 40 && bpm <= 200) {
        Serial.print("Heart Rate: ");
        Serial.print(bpm);
        Serial.println(" BPM");

        // Send BPM to KiwisIoT Channel 1
        kiwisiot.send("1", bpm);
      }
    }

    lastBeatTime = currentTime;
  }

  // Wait until signal drops before detecting next beat
  if (sensorValue < threshold) {
    beatDetected = false;
  }

  delay(10);
}