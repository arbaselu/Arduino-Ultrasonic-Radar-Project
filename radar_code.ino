#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
Servo myServo;  // Create a servo object to control a servo motor

// Define pins for the ultrasonic sensor
const int trigPin = 3;
const int echoPin = 5;
const int ledRed = 7;
const int ledGreen = 9;
const int buzzer = 8;

void setup() {
  Serial.begin(9600);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
  myServo.attach(13); // Attach the servo motor to pin 13
  myServo.write(90); // Start at 90 degrees
  delay(2000); // Give the servo time to reach the position
}

void loop() {
  int angles[] = {90, 180, 90, 0}; // The angles the servo motor will move through
  for (int i = 0; i < 4; i++) {
    int distance = measureDistance(); // Measure distance before changing the angle
    if (distance > 20) {
      myServo.write(angles[i]); // Change the angle if the distance is greater than 20 cm
      lcd.setCursor(4, 0); // Set the cursor to the first line
      lcd.print("No Enemy");
      
      Serial.println(angles[i]);
      digitalWrite(ledGreen, HIGH);
      delay(500); // Check again after each second
      digitalWrite(ledGreen, LOW);
      delay(500);
      lcd.clear();
    } else {
      while (distance <= 20) { // Wait until the obstacle is removed
        digitalWrite(buzzer, HIGH);
        digitalWrite(ledRed, HIGH);
        delay(500); // Check again after each second
        digitalWrite(ledRed, LOW);
        digitalWrite(buzzer, LOW);
        lcd.setCursor(1, 0); // Set the cursor to the first line
        lcd.print("Enemy Detected");
        delay(500);
        lcd.clear();
        
        distance = measureDistance(); // Measure the distance again
      }
      myServo.write(angles[i]); // Continue movement after the obstacle is removed
      Serial.print("Continuing to angle: ");
      Serial.println(angles[i]);
      delay(1000);
    }
  }
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Calculate the distance
  return distance;
}
