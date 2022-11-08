#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/event_groups.h>

#define flag_1seg (1 << 0)

EventGroupHandle_t evt;

void task(void*z)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupSetBits(evt, flag_1seg);
    }
}

void app_main(void)
{
    evt = xEventGroupCreate();
    xTaskCreatePinnedToCore(task, "task", 4096, NULL, 1, NULL, 0);

    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(evt, flag_1seg, true, true, portMAX_DELAY);

        if (bits & flag_1seg)
        {
            ESP_LOGI(__func__, "Evento de 1seg gerado");
        }
    }
}