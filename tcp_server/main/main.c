#include <stdio.h>
#include <esp_log.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_utils.h"
#include "tcp_server.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"

TaskHandle_t server_task_handle = 0;

#define BUFFER_MAX_SIZE 1024
void clientHandler(void* client_ptr)
{
    char* tcp_buf = (char*)malloc(BUFFER_MAX_SIZE);
  
    memset(tcp_buf, 0, BUFFER_MAX_SIZE);
  
    int client = *(int*)client_ptr;
  
    free(client_ptr);

    recv(client, tcp_buf, 20, 0);
  
    ESP_LOGI("CLIENT_HANDLER", "recieved from client: %s", tcp_buf);
  
   
    char send_data[20] = "Hi from ESP32";
    send(client, send_data, 20, 0);
           
    closesocket(client);
    free(tcp_buf);
    vTaskDelete(NULL);
} 


void app_main(void)
{

    wifiInitSTA(STA_SSID, STA_PASSWORD);

    xTaskCreatePinnedToCore(serverInit, "Server Task", 1024*2, (void*)clientHandler, 10, NULL, 1);
    

    
}
