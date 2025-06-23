#include <LiquidCrystal.h>

// LCD pin configuration: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int lm35Pin = A0;         // Temperature sensor pin
const int pulsePin = A1;        // Pulse sensor pin
const int ledPin = 13;          // Alert LED

int bpm = 0;
float tempC = 0.0;
unsigned long lastBeatTime = 0;
int pulseThreshold = 550;       // Adjust based on your sensor
bool beatDetected = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Health Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  readTemperature();
  readPulse();

  lcd.setCursor(0, 0);
  lcd.print("BPM  : ");
  lcd.print(bpm);

  lcd.setCursor(0, 1);
  lcd.print("Temp : ");
  lcd.print(tempC, 2);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C");

  // Alert if abnormal
  if (bpm > 100 || tempC > 38.0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000);
}

void readTemperature() {
  int raw = analogRead(lm35Pin);
  tempC = (raw * 5.0 * 100.0) / 1024.0;
}

void readPulse() {
  int signal = analogRead(pulsePin);

  if (signal > pulseThreshold && !beatDetected) {
    beatDetected = true;
    unsigned long currentTime = millis();
    bpm = 60000 / (currentTime - lastBeatTime);
    lastBeatTime = currentTime;
  }

  if (signal < pulseThreshold) {
    beatDetected = false;
  }
}
