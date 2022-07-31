#include <stdio.h>
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
#include "tcp_server.h"

#define PORT 6205

static int sock = -1;
struct sockaddr_in server_addr = {0};


void serverInit(void* client_handler)
{


    if (client_handler == NULL)
    {
        ESP_LOGE(SOCK_TAG, "Client Handler is null ptr, server stopped");
        vTaskDelete(NULL);
    }
    server_addr.sin_len = 0;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;


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

    int err = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (err != 0) 
    {
        ESP_LOGE(SOCK_TAG, "Socket unable to bind: errno %s", strerror(errno));   
    }
    else
    {
        ESP_LOGI(SOCK_TAG, "Successfully bound");
    }

    err = listen(sock, 5);
    if (err != 0) 
    {
        ESP_LOGE(SOCK_TAG, "Socket unable to listen: errno %s", strerror(errno));   
    }
    else
    {
        ESP_LOGI(SOCK_TAG, "Successfully listening");
    }
    while (1)
    {
        struct sockaddr_in client_addr = {0};
        int * client_ptr = (int*)malloc(sizeof(int));
        size_t len = sizeof(client_addr);
        *client_ptr = accept(sock, (struct sockaddr*)&client_addr, &len);
        
        if (*client_ptr < 0)
        {
            ESP_LOGE(SOCK_TAG, "Server unable to accept: errno %s", strerror(errno));   
        }
        else
        {
            ESP_LOGI(SOCK_TAG, "Accepted client");
            
            int ret = xTaskCreatePinnedToCore(client_handler, "client handler task", 1024*4, (void*)client_ptr, 10, NULL, 1);
            if (ret != pdPASS)
            {
                ESP_LOGE(SOCK_TAG, "Failed to create client handler, freertos err = %d", ret);
                closesocket(*client_ptr);
                free(client_ptr);
            }
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    } 
}



