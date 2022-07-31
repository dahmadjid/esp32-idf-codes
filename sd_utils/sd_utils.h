#ifndef _SD_UTILS_H_
#define _SD_UTILS_H_

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "sdkconfig.h"
#include "driver/sdmmc_host.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define SD_TAG "SDCARD"

#ifdef __cplusplus
extern "C" {
#endif


#define MOUNT_POINT "/sdcard"



/**
 * @brief Initializes SDMMC and mount the FAT file system
 * 
 */
void sdInit(void);


/**
 * @brief Simply write buffer into a file, used to write jpeg images from the framebuffer ptr
 * 
 * @param file_path name of the file to write to. example /mountpoint/img.jpg 
 * @param bytes ptr to buffer that holds bytes data
 * @param element_size; size of each element to write
 * @param len number of elements in the buffer
 * @return number of elements written
 */
int writeBytes(const char* file_path, void* bytes, size_t element_size, int len);


/**
 * @brief read bytes from a file into a buffer
 * 
 * @param file_path name of the file to read from. example /mountpoint/info.data
 * @param buf ptr to output buffer
 * @param element_size; size of each element to read
 * @param len number of elements to read
 * @return -1 failed to open file
 * @return 0 feof is false (end of file not reached)
 * @return >0 feof is true (end of file reached) and number corresponds to number of elements read
 * 
 */
int readBytes(const char* file_path, void* buf, size_t element_size, int len);


/**
 * @brief 
 * 
 * @param file_path name of the file to write to. example /mountpoint/newfile.txt 
 * @param text string you want to write
 */

void writeTxt(const char* file_path, const char* text);

/**
 * @brief 
 * 
 * @param file_path name of the file to read. example /mountpoint/newfile.txt 
 * @param buf output char buffer, currently supports reading upto 100 char including null terminator. enough for the purposes of the cctv-cam-project
 */
void readTxt(const char* file_path, char* buf);

/**
 * @brief tests if a file exists, this uses fatfs api directly
 * 
 * @param file_path name of the file without mountpoint, so its smth like newfile.txt instead of /mountpoint/newfile.txt  
 * @return true if it exists
 * @return false if it doesnt exist
 */
bool exists(const char* file_path);



#ifdef __cplusplus
}
#endif

#endif
