#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/semphr.h>

QueueHandle_t buffer;

void task(void*z)
{
    uint32_t x = 0;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        ESP_LOGI(__func__, "Enviando item: %d", ++x);
        xQueueSend(buffer, &x, pdMS_TO_TICKS(0));

        if (x % 3 == 0)
        {
            ESP_LOGW(__func__, "Aguardando 2seg");
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void app_main(void)
{
    buffer = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreatePinnedToCore(task, "task", 4096, NULL, 1, NULL, 0);

    uint32_t rcv;
    while (1)
    {
        if (xQueueReceive(buffer, &rcv, pdMS_TO_TICKS(500)))
        {
            ESP_LOGI(__func__, "Item recebido: %d", rcv);
        }
        else
        {
            ESP_LOGE(__func__, "Falha ao obter item");
        }
    }
}