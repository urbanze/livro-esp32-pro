#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"


void app_main(void)
{
    ESP_LOGW(__func__, "Start");

    for (int i = 0; i < 3; i++)
    {
        ESP_LOGI(__func__, "Test [%d]", i);
        
        esp_sleep_enable_timer_wakeup(5000000); //Configura 5seg de sleep
        esp_deep_sleep_start(); //Entre em Deep sleep
    }

    ESP_LOGW(__func__, "End");
}