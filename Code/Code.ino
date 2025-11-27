#include <WiFi.h>
#include <HTTPClient.h>

// -------- WiFi Details --------
const char* ssid     = "Hii";
const char* password = "12345678";

// -------- ThingSpeak --------
String apiKey = "M82TFTKJ6PFJLILK";     // Your ThingSpeak Write API Key
const char* serverName = "http://api.thingspeak.com/update";

// -------- Sensor Pins --------
const int MQ2_PIN = 18;     // MQ2 working pin
const int MQ3_PIN = 23;    // MQ3 working pin
const int LED_PIN = 2;     // LED pin

int thresholdValue = 1500;

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ3_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // ---- WiFi Connection ----
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("MQ2 + MQ3 IoT System Started...");
}

void loop() {
  // Read sensors
  int mq2Value = analogRead(MQ2_PIN);
  int mq3Value = analogRead(MQ3_PIN);

  Serial.print("MQ2: ");
  Serial.print(mq2Value);
  Serial.print(" | MQ3: ");
  Serial.println(mq3Value);

  // LED Alert if threshold crossed
  if (mq2Value > thresholdValue || mq3Value > thresholdValue) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // ---- Send to ThingSpeak ----
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(serverName)
                 + "?api_key=" + apiKey
                 + "&field1=" + String(mq2Value)
                 + "&field2=" + String(mq3Value);

    http.begin(url);
    int httpCode = http.GET();

    Serial.print("ThingSpeak Response: ");
    Serial.println(httpCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }

  delay(20000); // ThingSpeak requires minimum 15 seconds
}
