#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/semphr.h>


SemaphoreHandle_t SMF;

void task(void*z)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP_LOGI(__func__, "Liberando recurso");
        xSemaphoreGive(SMF);
    }
}

void app_main(void)
{
    SMF = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(task, "task", 4096, NULL, 1, NULL, 0);

    while (1)
    {
        if (xSemaphoreTake(SMF, pdMS_TO_TICKS(500)))
        {
            ESP_LOGI(__func__, "Recurso obtido");
        }
        else
        {
            ESP_LOGE(__func__, "Falha ao obter recurso");
        }
    }
}