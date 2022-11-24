#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/event_groups.h>

//Define a posicao do bit responsavel por esse status
#define flag_1seg (1 << 0)

EventGroupHandle_t evt;

//Tarefa responsavel por habilitar o bit
void task(void*z)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupSetBits(evt, flag_1seg); //Habilita (set) o bit
    }
}

void app_main(void)
{
    evt = xEventGroupCreate(); //Cria o grupo de eventos
    xTaskCreatePinnedToCore(task, "task", 4096, NULL, 1, NULL, 0);

    while (1)
    {
        //Espera infinitamente pelo bit
        EventBits_t bits = xEventGroupWaitBits(evt, flag_1seg, true, true, portMAX_DELAY);

        //Verifica se o bit esta habilitado
        if (bits & flag_1seg)
        {
            ESP_LOGI(__func__, "Evento de 1seg gerado");
        }
    }
}