#include <Servo.h>
#include <SoftwareSerial.h>

const int trigPin = 12;  // Ultrasonic sensor trigger pin
const int echoPin = 13; // Ultrasonic sensor echo pin
const int moisturePin = A0; // Analog pin for soil moisture sensor
const int gasPin = A1; // Analog pin for gas sensor
const int irSensorPin = 3; // Digital pin for IR sensor

const int buzzerPin = 2;  // Digital pin for the buzzer
const int servoPin = 9;   // Digital pin for the servo motor

Servo servo;

const float Vcc = 5.0;  // Voltage supplied to the sensors

SoftwareSerial bluetooth(4, 5); // RX, TX pins for Bluetooth communication

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  bluetooth.begin(9600); // Initialize Bluetooth communication
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  servo.attach(servoPin);
}

void loop() {
  // Ultrasonic Sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Soil Moisture Sensor
  int moistureValue = analogRead(moisturePin);
  int moisturePercentage = map(moistureValue, 0, 1023, 0, 100);

  // Gas Sensor
  int gasValue = analogRead(gasPin);
  float voltage = gasValue * (Vcc / 1024.0);
  float ppm = (voltage - 0.2) * 50.0;

  // IR Sensor (Bin Full)
  int irSensorValue = digitalRead(irSensorPin);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  Serial.print("Gas Concentration (PPM): ");
  Serial.println(ppm);

  // Check if IR sensor detects an object (bin is full)
  if (irSensorValue == HIGH) {
    Serial.println("IR sensor detected an object. Activating buzzer.");
    digitalWrite(buzzerPin, LOW);
    bluetooth.println("Bin is full. Please empty it.");
  } else {
    digitalWrite(buzzerPin, HIGH);
  }

  // Actions based on sensor readings
  if (distance < 10) {
    if (moisturePercentage > 50) {
      servo.write(90);
    } else {
      servo.write(180);
    }

    if (ppm > 60) {
      bluetooth.println("Gas concentration is high. Ventilate the area.");
    }
  } else {
    servo.write(0);
  }

  delay(1000);
}
