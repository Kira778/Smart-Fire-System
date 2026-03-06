#include <Servo.h>

const int FLAME_PIN = A0;       // Flame sensor analog pin
const int SCAN_SERVO_PIN = 9;   // Servo with flame sensor
const int FAN_SERVO_PIN = 10;   // Servo with pump
const int RELAY_PIN = 8;        // Relay pin

Servo scanServo;
Servo fanServo;

const int PAN_MIN = 20;    // left limit
const int PAN_MAX = 160;   // right limit

int flameThreshold = 500;  // adjust after testing (0–1023)

int scanAngle = PAN_MIN;   // current angle of scan servo
int scanDirection = 1;     // +1 = right, -1 = left
bool flameDetected = false;

void setup() {
  Serial.begin(115200);

  scanServo.attach(SCAN_SERVO_PIN);
  fanServo.attach(FAN_SERVO_PIN);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Fan OFF initially

  // Start positions
  scanServo.write(scanAngle);
  fanServo.write((PAN_MIN + PAN_MAX) / 2);

  Serial.println("🔥 Flame Tracking System Ready (UNO R4 Minima - Fan ON only on Flame)");
}

void loop() {
  int reading = analogRead(FLAME_PIN);

  // -----------------------
  // If flame detected
  // -----------------------
  if (reading > flameThreshold) {
    flameDetected = true;
    Serial.print("🔥 Flame detected at ");
    Serial.print(scanAngle);
    Serial.print("° | Value: ");
    Serial.println(reading);

    // Lock fan towards flame
    fanServo.write(scanAngle);

    // Fan ON
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Fan ON");

    // Stop scanner (hold position)
    delay(100);
  }
  else {
    // -----------------------
    // If no flame
    // -----------------------
    flameDetected = false;

    // Fan OFF
    digitalWrite(RELAY_PIN, LOW);

    // Resume scanning motion
    scanServo.write(scanAngle);
    delay(30); // scanning speed

    scanAngle += scanDirection;
    if (scanAngle >= PAN_MAX || scanAngle <= PAN_MIN) {
      scanDirection = -scanDirection; // reverse
    }
  }
}


