#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <soc/rtc_wdt.h>

void app_main(void)
{
    //Loop1, que nao ocasiona o reinicio
    ESP_LOGI(__func__, "Loop1 nao ira reiniciar");
    for (int i = 0; i < 10; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        rtc_wdt_feed(); //Alimenta o watchdog
    }

    //Loop2, que ocasiona o reinicio
    ESP_LOGW(__func__, "Loop2 ira reiniciar");
    for (int i = 0; i < 10; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}