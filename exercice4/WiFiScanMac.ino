/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.print((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      Serial.print(", MAC address : ");
      Serial.print(WiFi.BSSIDstr(i));
			// BSSID = Mac address
      Serial.print(", Encryption type : ");
      switch (WiFi.encryptionType(i))
      {
        case 5:
          Serial.println("WEP");
          break;
        case 2:
          Serial.println("WPA/PSK");
          break;
        case 4:
          Serial.println("WPA2/PSK");
          break;
        case 7:
          Serial.println("open network");
          break;
        case 8:
          Serial.println("WPA/WPA2/PSK");
          break;
				default:
					Serial.println("La documentation ne m'a pas fourni la traduction de cet encodage...");
      }
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}
