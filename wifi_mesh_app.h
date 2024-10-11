#ifndef wifi_mesh_app_H_
#define wifi_mesh_app_H_

#include "WiFiProv.h"
#include "WiFi.h"

bool wifi_status(void);

void wifi_app_start(void);

String wifi_prov_app_start(void);

void SysWifiEvent(arduino_event_t *sys_event) ;

#endif