#define BLYNK_TEMPLATE_ID "TMPL3IK2SUtLZ"
#define BLYNK_TEMPLATE_NAME "ESP8266 Home Automation"
#define BLYNK_AUTH_TOKEN "CDqXdFsDCZXoFM-H2A4IJSHbqjZ_-SSn" // Enter your auth token
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Define the relay pins
#define relay1 D0
#define relay2 D1

// Define sensor pins
#define DHTPIN D2  // DHT11 sensor pin
#define DHTTYPE DHT11
#define LDRPIN A0  // LDR sensor pin

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Galaxy F15 5G F33A"; // Enter your WIFI name
char pass[] = "SG@janani07"; // Enter your WIFI password

int ldrThreshold = 500; // Adjust this threshold based on your LDR sensor readings

// Relay control from Blynk
BLYNK_WRITE(V0) {
  digitalWrite(relay1, param.asInt() ? LOW : HIGH);
}

BLYNK_WRITE(V1) {
  digitalWrite(relay2, param.asInt() ? LOW : HIGH);
}

// Function to send sensor data to Blynk and control lights based on LDR
void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int ldrValue = analogRead(LDRPIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue); // Debugging

  // Send sensor values to Blynk
  if (!isnan(temperature) && !isnan(humidity)) {
    Blynk.virtualWrite(V3, temperature); // Temperature to V3
    Blynk.virtualWrite(V2, humidity);    // Humidity to V2
  }
  Blynk.virtualWrite(V4, ldrValue);      // LDR value to V4

  // **LDR-based Automatic Light Control**
  if (ldrValue < ldrThreshold) {
    digitalWrite(relay1, LOW); // Turn ON lights when it's dark
  } else {
    digitalWrite(relay1, HIGH); // Turn OFF lights when it's bright
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);
  pinMode(LDRPIN, INPUT);

  digitalWrite(relay1, HIGH); // Lights OFF initially
  digitalWrite(relay2, HIGH);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  timer.setInterval(2000L, sendSensorData); // Send sensor data every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}    
