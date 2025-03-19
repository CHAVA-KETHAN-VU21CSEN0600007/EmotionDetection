#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define ECG_PIN 34      // Analog pin for AD8232 ECG
#define GSR_PIN 35      // Analog pin for GSR sensor
#define LO_PLUS 32      // Lead-off detection pin
#define LO_MINUS 33     // Lead-off detection pin
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
// 🛜 WiFi and ThingSpeak credentials
const char* ssid = "GITAM";            // Replace with your WiFi SSID
const char* password = "Gitam$$123";    // Replace with your WiFi Password
String apiKey = "9B5LJZ27ILACI4K9";      // Replace with your ThingSpeak API Key
void setup() {
    Serial.begin(115200);
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n✅ WiFi Connected.");
    // Initialize MLX90614 sensor
    mlx.begin();
    Serial.println("Emotion Detection System Initialized.");
}
void sendDataToThingSpeak(float ambientF, float objectF, float ecgVoltage, float gsrResistance) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                     "&field1=" + String(ambientF) +
                     "&field2=" + String(objectF) +
                     "&field3=" + String(ecgVoltage, 2) +
                     "&field4=" + String(gsrResistance, 2);
        http.begin(url);
        int httpCode = http.GET();
        http.end();
        if (httpCode > 0) {
            Serial.println("📡 Data sent to ThingSpeak.");
        } else {
            Serial.println("❌ Failed to send data.");
        }
    }
}
void loop() {
    // 🌡 Temperature Readings in Fahrenheit
    float ambientF = (mlx.readAmbientTempC() * 9 / 5) + 32;
    float objectF = (mlx.readObjectTempC() * 9 / 5) + 32;
    // ❤ ECG Readings - Convert ADC to voltage
    float ecgVoltage = analogRead(ECG_PIN) * (3.3 / 4095.0);  
    // 🖐 GSR Readings - Convert ADC to resistance
    int gsrRaw = analogRead(GSR_PIN);
    float gsrResistance = (10000.0 / gsrRaw) * (4095 - gsrRaw);  
    // ⚠ Check for ECG lead-off
    int loPlus = digitalRead(LO_PLUS);
    int loMinus = digitalRead(LO_MINUS);
    if (loPlus == 1 || loMinus == 1) {
        Serial.println("⚠ ECG leads not properly connected! Check electrode placement.");
        delay(1000);
        return;
    }
    // 📡 Send data to ThingSpeak
    sendDataToThingSpeak(ambientF, objectF, ecgVoltage, gsrResistance);
    // 🖥 Print Data for Debugging
    Serial.print("Ambient Temp: "); Serial.print(ambientF); Serial.print(" °F | ");
    Serial.print("Object Temp: "); Serial.print(objectF); Serial.print(" °F | ");
    Serial.print("ECG: "); Serial.print(ecgVoltage, 2); Serial.print("V | ");
    Serial.print("GSR: "); Serial.print(gsrResistance, 2); Serial.println(" Ω");

    delay(15000);  // Delay 15 seconds (ThingSpeak allows updates every 15s)
}
