#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

void app_main(void)
{
    //Inicia o TWDT (apenas 1x)
    esp_task_wdt_init(2, true);

    //Adiciona essa task (main) ao TWDT
    esp_task_wdt_add(NULL);


    //Loop1, que nao ocasiona o reinicio
    ESP_LOGI(__func__, "Loop1 nao ira reiniciar");
    for (int i = 0; i < 10; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        esp_task_wdt_reset(); //Alimenta o TWDT
    }

    //Loop2, que ocasiona o reinicio
    ESP_LOGW(__func__, "Loop2 ira reiniciar");
    for (int i = 0; i < 10; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}