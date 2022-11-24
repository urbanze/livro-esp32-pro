#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <freertos/timers.h>

TimerHandle_t tmr;


//ISR responsavel por processar a interrupcao dos timers
void isr(TimerHandle_t timer_id)
{
    //Obtem o ID do timer que ocorreu o estouro
    uint32_t id = (uint32_t) pvTimerGetTimerID(timer_id);

    if (id == 1)
    {
        ets_printf("Timer X\n");
    }
    else if (id == 2)
    {
        ets_printf("Timer Y\n");
    }
}

void app_main(void)
{
    //Cria os timers
    TimerHandle_t tmr_x = xTimerCreate("tmr_x", pdMS_TO_TICKS(500),  true, (void*)1, isr);
    TimerHandle_t tmr_y = xTimerCreate("tmr_y", pdMS_TO_TICKS(1000), true, (void*)2, isr);

    //Inicia os timers
    xTimerStart(tmr_x, pdMS_TO_TICKS(1000));
    xTimerStart(tmr_y, pdMS_TO_TICKS(1000));
}