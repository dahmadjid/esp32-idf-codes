#ifndef _NTP_UTILS_H_
#define _NTP_UTILS_H_

#include "esp_sntp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NTP_TAG "NTP"

void ntpInit(const char* time_zone) // example: "UTC-1" is UTC+1 (offset is inverted (hours to be added to get UTC0))
{
    setenv("TZ", time_zone, 1);   
    tzset();
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(NTP_TAG, "Waiting for system time to be set... (%d/%d)", (int)retry, (int)retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}


void getTime(time_t* now, tm* timeinfo)
{
    time(now);
    localtime_r(now, timeinfo);
    ESP_LOGI(NTP_TAG, "Year: %d || Hours: %d || Minutes: %d || Seconds: %d || Day of week: %d ", timeinfo->tm_year, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_wday);
}


#ifdef __cplusplus
}
#endif

#endif
