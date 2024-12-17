#include <DHT.h>

#define DHTPIN 2            // DHT11 data pin
#define DHTTYPE DHT11       // DHT11 sensor type
#define TRIGGER_PIN 3       // HC-SR04 Trigger pin
#define ECHO_PIN 4          // HC-SR04 Echo pin
#define MQ4_PIN A0          // MQ-4 gas sensor analog pin
#define FAN_RELAY_PIN 8     // Relay pin for exhaust fan - white LED
#define ALARM_RELAY_PIN 9   // Relay pin for buzzer
#define GREEN_LED_PIN 5     // Green LED pin
#define BLUE_LED_PIN 6      // BLUE LED pin
#define RED_LED_PIN 7       // Red LED pin
#define Exaust_valve_PIN 10 // yellow led

DHT dht(DHTPIN, DHTTYPE); // Create DHT object

long duration;
int distance;
float gasLevel;
int temperature;
int humidity;
int gasThreshold = 180; // Threshold for MQ-4 gas detection (adjust according to your need)
int temperatureThreshold = 33; // Temperature threshold for fan control 33
int humidityThreshold = 60;
   // Humidity threshold for fan control 60

void setup() {
  Serial.begin(250000);
 
  
  // Initialize DHT11 sensor
  dht.begin();
  
  // Set pins for LEDs
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(Exaust_valve_PIN, OUTPUT); // yellow
  pinMode(FAN_RELAY_PIN, OUTPUT); //white
  pinMode(ALARM_RELAY_PIN, OUTPUT); //buzzer

  
  // Set pins for ultrasonic sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize relay state
  digitalWrite(FAN_RELAY_PIN, LOW);
  digitalWrite(ALARM_RELAY_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(Exaust_valve_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void loop() {

  
  // Read temperature and humidity from DHT11
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Read gas level from MQ-4 sensor
  gasLevel = analogRead(MQ4_PIN);

  // Calculate distance from ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.0344 / 2;

  // Check for gas leakage
  if (gasLevel > gasThreshold) {
    digitalWrite(ALARM_RELAY_PIN, HIGH);
    
      // Turn on alarm
    digitalWrite(FAN_RELAY_PIN, HIGH);  
     // Turn on exhaust fan
    digitalWrite(RED_LED_PIN, LOW);     // Red light indicates danger
    digitalWrite(GREEN_LED_PIN, LOW);    // Turn off green light
    digitalWrite(BLUE_LED_PIN, LOW);   // Turn off yellow light
    digitalWrite(Exaust_valve_PIN, HIGH);
  } 
  // Check for high temperature or humidity
   if (temperature > temperatureThreshold || humidity < humidityThreshold) {
    digitalWrite(FAN_RELAY_PIN, HIGH);  
    
    digitalWrite(ALARM_RELAY_PIN, LOW);
    
    digitalWrite(RED_LED_PIN, LOW);    // Turn off red light
    digitalWrite(GREEN_LED_PIN, LOW);  // Turn off green light
    digitalWrite(BLUE_LED_PIN, HIGH); // 
    digitalWrite(ALARM_RELAY_PIN, LOW);  // Turn off alarm
    digitalWrite(Exaust_valve_PIN, HIGH);
  } 
  // Check for object or person detected by ultrasonic sensor
   if (distance < 150) {  // 2 meters = 200 cm
    digitalWrite(FAN_RELAY_PIN, LOW);  
    digitalWrite(ALARM_RELAY_PIN, HIGH);
    
    digitalWrite(RED_LED_PIN, HIGH);     // Red light indicates danger
    digitalWrite(GREEN_LED_PIN, LOW);    // Turn off green light
    digitalWrite(BLUE_LED_PIN, LOW);
    
    digitalWrite(Exaust_valve_PIN, HIGH);
  } 
  else {
    // No issues, safe zone
    digitalWrite(ALARM_RELAY_PIN, LOW);  // Turn off alarm
    digitalWrite(FAN_RELAY_PIN, LOW);    // Turn off exhaust fan
    digitalWrite(RED_LED_PIN, LOW);      // Turn off red light
    digitalWrite(GREEN_LED_PIN, HIGH);   // Green light indicates safe zone
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(Exaust_valve_PIN, LOW);
  }

  // Print sensor data to serial monitor
  
  Serial.print(gasLevel);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.println(distance);
 

  delay(100);  // Delay before reading again
  
  }
