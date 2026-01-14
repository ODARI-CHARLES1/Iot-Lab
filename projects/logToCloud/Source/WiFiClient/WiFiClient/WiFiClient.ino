#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LCD_ADDR 0x27
LiquidCrystal_I2C lcd(LCD_ADDR, 20, 4);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* host = "api.thingspeak.com";
const int httpPort = 80;
const String writeApiKey = "KZ5PU7BCML4U7MW2";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected!");
}

void readResponse(WiFiClient* client) {
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout!");
      client->stop();
      return;
    }
  }

  while (client->available()) {
    String line = client->readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("\nClosing connection\n");
}

void loop() {
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    temperature = random(20, 35);
    humidity = random(40, 80);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(humidity);
  lcd.print(" %");

  if (client.connect(host, httpPort)) {
    String writeURL = "/update?api_key=" + writeApiKey +
                      "&field1=" + String(int(temperature)) +
                      "&field2=" + String(int(humidity));

    client.print(String("GET ") + writeURL + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.print("Sending Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    unsigned long timeout = millis();
    bool success = false;
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout!");
        client.stop();
        break;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      if (line.indexOf("200") != -1) success = true;
    }
    client.stop();

    lcd.setCursor(0, 2);
    if (success) {
      Serial.println("✅ Data successfully sent to ThingSpeak!");
      lcd.print("Data sent ✅");
    } else {
      Serial.println("Failed to send data.");
      lcd.print("Data failed ❌");
    }
  } else {
    Serial.println("Connection failed to ThingSpeak");
    lcd.setCursor(0, 2);
    lcd.print("No Connection ❌");
  }

  delay(10000); 
}
