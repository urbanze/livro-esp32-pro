#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <sys/time.h>

void app_main(void)
{
    //Mostra a data apos o boot
    time_t now = time(NULL);
    ESP_LOGI(__func__, "Boot time: %d", (int32_t)now);

    //Configura a data para um valor fixo
    struct timeval stv;
    stv.tv_sec = 1660243530;
    stv.tv_usec = 0;
    settimeofday(&stv, NULL);

    //Mostra a data novamente
    now = time(NULL);
    ESP_LOGI(__func__, "Time now: %d", (int32_t)now);

    //Espera um pouco para reiniciar o sistema
    vTaskDelay(pdMS_TO_TICKS(5000));
    esp_restart();
}