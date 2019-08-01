#include "config.h"

#include "wifi_local.h"

#include <WiFi.h>
#include <WiFiMulti.h>

static char hostname[sizeof("discoball-%02x%02x%02x") + 1];

static WiFiMulti wifiMulti;

static RTC_DATA_ATTR int wifi_failures = 0;

bool wifi_begin() {
  byte mac_address[6];

  WiFi.macAddress(mac_address);
  snprintf(hostname, sizeof(hostname), "discoball-%02x%02x%02x", (int)mac_address[3], (int)mac_address[4], (int)mac_address[5]);
  Serial.printf("Hostname: %s\n", hostname);
  Serial.printf("MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		mac_address[0],
		mac_address[1],
		mac_address[2],
		mac_address[3],
		mac_address[4],
		mac_address[5]);
  WiFi.setHostname(hostname);

  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);
  wifiMulti.addAP(WIFI_SSID2, WIFI_PASSWORD2);
  wifiMulti.addAP(WIFI_SSID3, WIFI_PASSWORD3);

  static int wifi_tries = 0;
  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);

    if(wifi_tries++ > 100) {
      wifi_failures++;
      ESP.restart();
    }
  }
}

const char* wifi_hostname() {
  return hostname;
}