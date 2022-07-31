#include "sd_utils.h"


void sdInit(void)
{
    esp_err_t ret;
    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        false,
        5,
        16 * 1024};
    sdmmc_card_t *card;

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY); // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ONLY);  // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);  // D1, needed in 4-line mode only
    gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY); // D2, needed in 4-line mode only
    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY); // D3, needed in 4- and 1-line modes

    ESP_LOGI(SD_TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(SD_TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else
        {
            ESP_LOGE(SD_TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.",
                     esp_err_to_name(ret));
        }
        return;
    }

    ESP_LOGI(SD_TAG, "Filesystem mounted");
    sdmmc_card_print_info(stdout, card);
}


int writeBytes(const char* file_path, void* bytes, size_t element_size, int len)
{

  
    FILE* f = fopen(file_path, "w");
    if (!f) 
    {
        ESP_LOGE(SD_TAG, "Failed to open file for writing");
        fclose(f);
        return -1;
    }

    size_t out = fwrite(bytes, element_size, len, f);
    ESP_LOGI(SD_TAG, "Wrote %zu elements out of %d", out, len);

    fclose(f);
    
   
    return out;
}

int readBytes(const char* file_path, void* buf, size_t element_size, int len)
{
    FILE* f = fopen(file_path, "r");
    if (!f) 
    {
        ESP_LOGE(SD_TAG, "Failed to open file for reading");
        return -1;
    }

    size_t out = fread(buf, element_size, len, f);
    ESP_LOGI(SD_TAG, "read %zu elements out of %d", out, len);
    if (feof(f))
    {
        fclose(f);
        return out; 
    }
    fclose(f);
    return 0;
}

void writeTxt(const char* file_path, const char* text)
{
    FILE* f = fopen(file_path, "w");
    if (!f) 
    {
        ESP_LOGE(SD_TAG, "Failed to open txt file for writing");
        return;
    }
    int ret = fprintf(f, text);
    if (ret < 0) 
    {
        ESP_LOGE(SD_TAG, "Failed to write on txt file");
        return;
    }
    fclose(f);

}


void readTxt(const char* file_path, char* buf)
{
    FILE* f = fopen(file_path, "r");
    if (!f) 
    {
        ESP_LOGE(SD_TAG, "Failed to open txt file for reading");
        return;
    }
    char* ret = fgets(buf, 100, f);
    if (!ret) 
    {
        ESP_LOGE(SD_TAG, "Failed to read on txt file");
        return;
    }
    fclose(f);
}



bool exists(const char* file_path)
{

    FILINFO fno;
    FRESULT fr = f_stat(file_path, &fno);
    if (fr == FR_OK)
    {
        ESP_LOGI(SD_TAG, "File %s exists", file_path);
        return true;
    }
    else if (fr == FR_NO_FILE || fr == FR_NO_PATH)
    {
        ESP_LOGW(SD_TAG, "File %s doesnt exists", file_path);
        return false;
    }
    else
    {  
        ESP_LOGE(SD_TAG, "Uknown Error err=%d", (int)fr);
        return false;
    }
}


