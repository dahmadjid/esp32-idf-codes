
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "wifi_utils.h"

#include "wifi_config.h"  // Configure wifi parameters in this file

void app_main(void)
{

    wifiInitSTA(STA_SSID, STA_PASSWORD);                            // station mode
    // wifiInitAP(AP_SSID, AP_PASSWORD);                               // AP mode
    // wifiInitSTAAP(STA_SSID, STA_PASSWORD, AP_SSID, AP_PASSWORD);    // station + AP 

}
