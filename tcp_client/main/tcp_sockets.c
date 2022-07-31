
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "wifi_utils.h"

#define SOCK_TAG "SOCKET"
#define SERVER_IP "192.168.43.23"
#define PORT 6205

#define DELAY(time) vTaskDelay(time/portTICK_PERIOD_MS)

int sock = -1;

struct sockaddr_in dest_addr = {0};

void app_main(void)
{
    wifiInitSTA(STA_SSID, STA_PASSWORD);

    dest_addr.sin_len = 0;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) 
    {
        ESP_LOGE(SOCK_TAG, "Unable to create socket: errno %s", strerror(errno));
        return;   
    }
    else
    {
        ESP_LOGI(SOCK_TAG, "Successfully created socket with channel: %d", sock);
    }   

    int err = -1;
    while (err != 0)
    {
        err = connect(sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        if (err != 0) 
        {
            ESP_LOGE(SOCK_TAG, "Socket unable to connect: errno %s", strerror(errno));   
        }
        else
        {
            ESP_LOGI(SOCK_TAG, "Successfully connected");
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }


    char data[] = "hello from esp32"; 
    err = send(sock, data, strlen(data), 0);
    if (err < 0) 
    {
        ESP_LOGE(SOCK_TAG, "Error occurred while sending data: errno %s", strerror(errno));
    }
    else
    {
        ESP_LOGI(SOCK_TAG, "Successfully sent data, waiting for response");
    }

    char buf[64] = {0};
    int len = recv(sock, buf, 64, 0);
    if (len < 0)
    {
        ESP_LOGE(SOCK_TAG, "Error occured while recieving data errno: %s", strerror(errno));
    }
    else
    {
        ESP_LOGI(SOCK_TAG, "response from server: %s", buf);
    }
    closesocket(sock);
}