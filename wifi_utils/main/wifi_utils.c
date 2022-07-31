#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <string.h>
#include "lwip/err.h"
#include "lwip/sys.h"

#include "wifi_utils.h"


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define WIFI_TAG "WIFI"

static EventGroupHandle_t wifi_event_group;
static int retry_num = 0;


static void eventHandlerSTA(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_err_t err;
    if (event_base == WIFI_EVENT)
    {
        switch(event_id)
        {   
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_num < STA_MAX_RETRY)
                {
                    err = esp_wifi_connect();
                    retry_num++;
                    ESP_LOGI(WIFI_TAG, "Failed to connect err: %s. Retrying to connect to the AP", esp_err_to_name(err));
                }
                else 
                {
                    xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
                }
                break;
                
            default:
                ESP_LOGI(WIFI_TAG, "event_id for wifi: %d", event_id);
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
    
        switch(event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            {
                ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
                ESP_LOGI(WIFI_TAG, "STA Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                retry_num = 0;
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
                break;
            }
            default:
                ESP_LOGI(WIFI_TAG, "event_id for IP: %d", event_id);
                break;
        }
    }
}


static void eventHandlerAP(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if (event_base == WIFI_EVENT)
    {
        switch(event_id)
        {
            case WIFI_EVENT_AP_STACONNECTED:
            {
                wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(WIFI_TAG, "station "MACSTR" joined, AID=%d", MAC2STR(event->mac), event->aid);
                break;
            }
            case WIFI_EVENT_AP_STADISCONNECTED:
            {
                wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(WIFI_TAG, "station "MACSTR" left, AID=%d", MAC2STR(event->mac), event->aid);
                break;
            }
            default:
                ESP_LOGI(WIFI_TAG, "event_id for wifi: %d", event_id);
                break;
        }
    }

}


static void eventHandlerSTAAP(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) 
{
    if (event_base == WIFI_EVENT)
    {
        switch(event_id)
        {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_num < STA_MAX_RETRY)
                {
                    esp_wifi_connect();
                    retry_num++;
                    ESP_LOGI(WIFI_TAG, "Retrying to connect to the AP");
                }
                else 
                {
                    xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
                }
                break;
                
            case WIFI_EVENT_AP_STACONNECTED:
            {
                wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(WIFI_TAG, "station "MACSTR" joined, AID=%d", MAC2STR(event->mac), event->aid);
                break;
            }
            case WIFI_EVENT_AP_STADISCONNECTED:
            {
                wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(WIFI_TAG, "station "MACSTR" left, AID=%d", MAC2STR(event->mac), event->aid);
                break;
            }
            default:
                ESP_LOGI(WIFI_TAG, "event_id for wifi: %d", event_id);
                break;
        }
    }
    else if (event_base == IP_EVENT)
    {
    
        switch(event_id)
        {
            case IP_EVENT_STA_GOT_IP:
            {
                ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
                ESP_LOGI(WIFI_TAG, "STA Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                retry_num = 0;
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
                break;
            }
            default:
                ESP_LOGI(WIFI_TAG, "event_id for IP: %d", event_id);
                break;
        }
    }


    // if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    // {
    //     esp_wifi_connect();
    // } 
    // else if (event_base == WIFI_EVENT &&event_id == WIFI_EVENT_STA_DISCONNECTED) 
    // {
    //     if (retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) 
    //     {
    //         esp_wifi_connect();
    //         retry_num++;
    //         ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
    //     } 
    //     else 
    //     {
    //         xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
    //     }
    //     ESP_LOGI(WIFI_TAG, "connect to the AP fail");
    // } 
    // if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    //     wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
    //     ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
    //              MAC2STR(event->mac), event->aid);
    // } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    //     wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
    //     ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
    //              MAC2STR(event->mac), event->aid);
    // }
    // else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    // {
    //     ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    //     ESP_LOGI(WIFI_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    //     retry_num = 0;
    //     xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    // }
    
}

static void commonInit() 
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

void wifiInitSTA(const char* sta_ssid, const char* sta_password)
{
    commonInit();
    wifi_event_group = xEventGroupCreate();
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandlerSTA, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandlerSTA, NULL, &instance_got_ip));


    wifi_sta_config_t sta_config = {0};

    memcpy(sta_config.ssid, sta_ssid, strlen(sta_ssid));
    memcpy(sta_config.password, sta_password, strlen(sta_password));
    sta_config.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    sta_config.pmf_cfg.capable = true;
    sta_config.pmf_cfg.required = false;


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT) 
    {
        ESP_LOGW(WIFI_TAG, "Connected to ap SSID: %s", sta_ssid);
    } 
    else if (bits & WIFI_FAIL_BIT) 
    {
        ESP_LOGE(WIFI_TAG, "Failed to connect to SSID: %s", sta_ssid);
    } 
    else 
    {
        ESP_LOGE(WIFI_TAG, "UNEXPECTED EVENT");
    }
}

void wifiInitAP(const char* ap_ssid, const char* ap_password)
{
    commonInit();
    wifi_event_group = xEventGroupCreate();
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandlerAP, NULL, &instance_any_id));

    wifi_ap_config_t ap_config = {0};

    memcpy(ap_config.ssid, ap_ssid, strlen(ap_ssid));
    memcpy(ap_config.password, ap_password, strlen(ap_password));
    ap_config.ssid_len = strlen(ap_ssid);
    ap_config.authmode = WIFI_AUTH_WPA2_PSK;
    ap_config.channel = 1;
    ap_config.max_connection = AP_MAX_CONNECTIONS;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(WIFI_TAG, "AP "AP_SSID" Started");
}

void wifiInitSTAAP(const char* sta_ssid, const char* sta_password, const char* ap_ssid, const char* ap_password)
{   
    commonInit();
    wifi_event_group = xEventGroupCreate();
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandlerSTAAP, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandlerSTAAP, NULL, &instance_got_ip));

    wifi_sta_config_t sta_config = {0};

    memcpy(sta_config.ssid, sta_ssid, strlen(sta_ssid));
    memcpy(sta_config.password, sta_password, strlen(sta_password));
    sta_config.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    sta_config.pmf_cfg.capable = true;
    sta_config.pmf_cfg.required = false;

    wifi_ap_config_t ap_config = {0};

    memcpy(ap_config.ssid, ap_ssid, strlen(ap_ssid));
    memcpy(ap_config.password, ap_password, strlen(ap_password));
    ap_config.ssid_len = strlen(ap_ssid);
    ap_config.authmode = WIFI_AUTH_WPA2_PSK;
    ap_config.channel = 1;
    ap_config.max_connection = AP_MAX_CONNECTIONS;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT) 
    {
        ESP_LOGW(WIFI_TAG, "Connected to ap SSID: %s", sta_ssid);
    } 
    else if (bits & WIFI_FAIL_BIT) 
    {
        ESP_LOGE(WIFI_TAG, "Failed to connect to SSID: %s", sta_ssid);
    } 
    else 
    {
        ESP_LOGE(WIFI_TAG, "UNEXPECTED EVENT");
    }
}
