#include <Arduino.h>

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  Serial.println("setup started");
  pinMode(LED_PIN, OUTPUT);
  Serial.println("setup completed");
}

void loop() {
  Serial.println("increasing the LED brightness");
  for (int i = 0; i < 256; i++) {
    analogWrite(LED_PIN, i); // Gradually increase brightness
    delay(10); // Wait for 10 milliseconds
  }
  Serial.println("decreasing the LED brightness");
  for (int i = 255; i >= 0; i--) {
    analogWrite(LED_PIN, i); // Gradually decrease brightness
    delay(10); // Wait for 10 milliseconds
  }
}