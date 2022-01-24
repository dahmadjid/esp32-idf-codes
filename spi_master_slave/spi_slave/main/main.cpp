#include <iostream>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/spi_slave.h"

#include "esp_log.h"


spi_device_handle_t spi;
esp_err_t ret;

void spiSlaveInit(void)
{
    spi_bus_config_t buscfg = {0};  
    
    buscfg.mosi_io_num = GPIO_NUM_13;
    buscfg.miso_io_num = GPIO_NUM_12;
    buscfg.sclk_io_num = GPIO_NUM_14;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    
    spi_slave_interface_config_t slvcfg = {0};
    slvcfg.mode = 0;
    slvcfg.spics_io_num = GPIO_NUM_27;
    slvcfg.queue_size = 3;
    slvcfg.flags = 0;
    

    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_NUM_14, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_NUM_27, GPIO_PULLUP_ONLY);
    spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, NULL);
}

extern "C" void app_main(void)
{
    spiSlaveInit();
   
    uint8_t n = 0;
    char sendbuf[1]= {0};
    char recvbuf[1]= {0};

    while(1) 
    {
        spi_slave_transaction_t t = {0};
        
        t.length = 8;
        t.tx_buffer = sendbuf;
        t.rx_buffer = recvbuf;
        sendbuf[0] = n;
        ret = spi_slave_transmit(SPI2_HOST, &t, portMAX_DELAY);
        if (ret != ESP_OK)
        {
            ESP_LOGE("SPI_SLAVE","Failed to transmit: %s", esp_err_to_name(ret));
            continue;
        }

        ESP_LOGI("SPI_SLAVE","Transmission is successful: received: %d, sent: %d", recvbuf[0], sendbuf[0]);
        
        n++;  
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
