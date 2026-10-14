#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// LEDs
const int greenLED = 3;
const int yellowLED = 4;
const int redLED = 5;

// Buzzer
const int buzzer = 6;

long duration;
float distance;

void setup() {

  // Pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Rear Safety");
  lcd.setCursor(0, 1);
  lcd.print("System Starting");

  delay(2000);

  lcd.clear();
}

void loop() {

  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH, 30000);

  // Calculate distance in cm
  if (duration == 0) {
    distance = 999;
  }
  else {
    distance = duration * 0.0343 / 2;
  }

  // Turn everything OFF first
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  noTone(buzzer);

  // LCD distance
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");

  if (distance < 999) {
    lcd.print(distance, 0);
    lcd.print(" cm");
  }
  else {
    lcd.print("No Vehicle");
  }

  // SAFE
  if (distance > 150) {

    digitalWrite(greenLED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("STATUS: SAFE");

  }

  // CAUTION
  else if (distance > 100) {

    digitalWrite(yellowLED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("CAUTION");

    // Slow beep
    tone(buzzer, 1000);
    delay(150);
    noTone(buzzer);
    delay(850);
  }

  // WARNING
  else if (distance > 50) {

    digitalWrite(redLED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("WARNING!");

    // Rapid beep
    tone(buzzer, 1500);
    delay(200);
    noTone(buzzer);
    delay(200);
  }

  // DANGER
  else {

    digitalWrite(redLED, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("DANGER!");

    // Continuous buzzer
    tone(buzzer, 2000);
  }

  delay(100);
}