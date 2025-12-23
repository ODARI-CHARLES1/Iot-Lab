#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32 WiFi Scan Started");

  // Set WiFi to station mode and disconnect from any AP
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  Serial.println("Scanning available networks...");

  int n = WiFi.scanNetworks();

  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print("Networks found: ");
    Serial.println(n);

    for (int i = 0; i < n; i++) {
      Serial.print(i + 1);
      Serial.print(") ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" | Signal: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm");
      Serial.print(" | Encryption: ");
      printEncryptionType(WiFi.encryptionType(i));
      Serial.println();
    }
  }

  Serial.println("-----------------------");
  delay(10000);  // scan every 10 seconds
}

void printEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case WIFI_AUTH_OPEN:
      Serial.print("Open");
      break;
    case WIFI_AUTH_WEP:
      Serial.print("WEP");
      break;
    case WIFI_AUTH_WPA_PSK:
      Serial.print("WPA");
      break;
    case WIFI_AUTH_WPA2_PSK:
      Serial.print("WPA2");
      break;
    case WIFI_AUTH_WPA_WPA2_PSK:
      Serial.print("WPA/WPA2");
      break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
      Serial.print("WPA2-Enterprise");
      break;
    case WIFI_AUTH_WPA3_PSK:
      Serial.print("WPA3");
      break;
    default:
      Serial.print("Unknown");
      break;
  }
}
