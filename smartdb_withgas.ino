#include <Servo.h>

const int trigPin = 12;  // Ultrasonic sensor trigger pin
const int echoPin = 13; // Ultrasonic sensor echo pin
const int moisturePin = A0; // Analog pin for soil moisture sensor
const int gasPin = A1; // Analog pin for gas sensor

const int buzzerPin = 2;  // Digital pin for the buzzer
const int servoPin = 9;   // Digital pin for the servo motor
Servo servo;

const float Vcc = 5.0;       // Voltage supplied to the sensors


void setup() {
  Serial.begin(9600);        // Initialize serial communication
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
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

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  Serial.print("Gas Concentration (PPM): ");
  Serial.println(ppm);

  // Actions based on sensor readings
  if (distance < 20) {
    if (moisturePercentage > 50) {
      servo.write(90);  // Rotate the servo to 90 degrees
    } else {
      servo.write(180); // Rotate the servo to 180 degrees
    }

    if (ppm > 60) {
      digitalWrite(buzzerPin, HIGH); // Activate the buzzer
    } else {
      digitalWrite(buzzerPin, LOW); // Deactivate the buzzer
    }

    delay(1000); // Buzzer on/off time (adjust as needed)
  } else {
    servo.write(0); // Reset servo to initial position
    digitalWrite(buzzerPin, LOW); // Deactivate the buzzer
  }

  delay(1000); // Delay for one second (adjust as needed)
}
