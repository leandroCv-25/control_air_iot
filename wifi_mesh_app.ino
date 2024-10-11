#include "WiFiProv.h"
#include "WiFi.h"

#include "wifi_mesh_app.h"

// #define USE_SOFT_AP // Uncomment if you want to enforce using the Soft AP method instead of BLE
const char *pop = "abcd1234";    // Proof of possession - otherwise called a PIN - string provided by the device, entered by the user in the phone app
const char *service_key = NULL;  // Password used for SofAP method (NULL = no password needed)
bool reset_provisioned = true;   // When true the library will automatically delete previously provisioned data.


static String get_device_service_name() {
  uint8_t eth_mac[6];
  WiFi.macAddress(eth_mac);

  return "Air_" + String(eth_mac[3]) + ":" + String(eth_mac[4]) + ":" + String(eth_mac[5]);
}

// WARNING: SysWifiEvent is called from a separate FreeRTOS task (thread)!
void SysWifiEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\nConnected IP address : ");
      Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED: Serial.println("\nDisconnected. Connecting to the AP again... "); break;
    case ARDUINO_EVENT_PROV_START: break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
      {
        Serial.println("\nReceived Wi-Fi credentials");
        Serial.print("\tSSID : ");
        Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
        Serial.print("\tPassword : ");
        Serial.println((char const *)sys_event->event_info.prov_cred_recv.password);
        break;
      }
    case ARDUINO_EVENT_PROV_CRED_FAIL:
      {
        Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
        if (sys_event->event_info.prov_fail_reason == WIFI_PROV_STA_AUTH_ERROR) {
          Serial.println("\nWi-Fi AP password incorrect");
        } else {
          Serial.println("\nWi-Fi AP not found....Add API \" nvs_flash_erase() \" before beginProvision()");
        }
        break;
      }
    case ARDUINO_EVENT_PROV_CRED_SUCCESS: Serial.println("\nProvisioning Successful"); break;
    case ARDUINO_EVENT_PROV_END: Serial.println("\nProvisioning Ends"); break;
    default: break;
  }
}



String wifi_prov_app_start(void) {

  uint8_t uuid[16] = { 0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf, 0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02 };

  WiFiProv.beginProvision(
    WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BLE, WIFI_PROV_SECURITY_1, pop, get_device_service_name().c_str(), service_key, uuid, reset_provisioned);

  char payload[150] = { 0 };

  snprintf(
    payload, sizeof(payload),
    "name:%s\npop:\"%s",
    get_device_service_name().c_str(), pop);

  Serial.println(payload);

  return payload;
}

void wifi_app_start(void) {
  WiFi.begin();  // no SSID/PWD - get it from the Provisioning APP or from NVS (last successful connection)
  WiFi.onEvent(SysWifiEvent);
}