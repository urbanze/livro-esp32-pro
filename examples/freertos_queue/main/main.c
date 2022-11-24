#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/semphr.h>

QueueHandle_t buffer;

//Task responsavel por enviar valores para a fila
void task(void*z)
{
    uint32_t x = 0;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        ESP_LOGI(__func__, "Enviando item: %d", ++x);
        xQueueSend(buffer, &x, pdMS_TO_TICKS(0)); //Envia o valor de [X] para a fila

        if (x % 3 == 0)
        {
            ESP_LOGW(__func__, "Aguardando 2seg");
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void app_main(void)
{
    buffer = xQueueCreate(10, sizeof(uint32_t)); //Cria a fila com [10] posicoes de tamanho [UINT32] cada
    xTaskCreatePinnedToCore(task, "task", 4096, NULL, 1, NULL, 0);

    uint32_t rcv;
    while (1)
    {
        //Tenta obter dados na fila durante 500ms
        if (xQueueReceive(buffer, &rcv, pdMS_TO_TICKS(500)))
        {
            //Sucesso ao obter item
            ESP_LOGI(__func__, "Item recebido: %d", rcv);
        }
        else
        {
            //Falha ao obter item dentro do tempo maximo
            ESP_LOGE(__func__, "Falha ao obter item");
        }
    }
}