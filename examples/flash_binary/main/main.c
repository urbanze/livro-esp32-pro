#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

//Binarios embarcados na memoria flash pelo compilador
extern const uint8_t txt_start[] asm("_binary_test_txt_start");
extern const uint8_t txt_end[]   asm("_binary_test_txt_end");


void app_main(void)
{
    //Mostra o texto que foi embarcado na flash
    ESP_LOGI(__func__, "[%s]", txt_start);
}