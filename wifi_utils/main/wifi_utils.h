#ifndef __WIFI_UTILS_H__
#define __WIFI_UTILS_H__


#include "wifi_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Connect to a wifi network. ESP32 is in station mode
 * 
 * @param sta_ssid your router ssid
 * @param sta_password your router password
 */
void wifiInitSTA(const char* sta_ssid, const char* sta_password);

/**
 * @brief Set up an access point. ESP32 is in softAP mode
 * 
 * @param ap_ssid ESP32 AP SSID
 * @param ap_password ESP32 AP Password
 */
void wifiInitAP(const char* ap_ssid, const char* ap_password);


/**
 * @brief Connect to a wifi network and set up an access point. ESP32 is in station + softAP mode
 * 
 * @param sta_ssid your router ssid
 * @param sta_password your router password
 * @param ap_ssid ESP32 AP SSID
 * @param ap_password ESP32 AP Password
 */
void wifiInitSTAAP(const char* sta_ssid, const char* sta_password, const char* ap_ssid, const char* ap_password);



#ifdef __cplusplus
}
#endif

#endif