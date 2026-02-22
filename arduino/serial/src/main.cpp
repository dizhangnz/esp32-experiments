#include <Arduino.h>

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  Serial.println("setup started");
  pinMode(LED_PIN, OUTPUT);
  Serial.println("setup completed");
}

void loop() {
  Serial.println("turn on the LED");
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  delay(1000); // Wait for 1 second
  Serial.println("turn off the LED");
  digitalWrite(LED_PIN, LOW); // Turn the LED off
  delay(1000); // Wait for 1 second
}