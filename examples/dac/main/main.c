#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/dac.h"

void app_main(void)
{
    dac_output_enable(DAC_CHANNEL_1); //Habilita o DAC1

    while (1)
    {
        //Gera a forma de onda 'dente de serra'
        for (int i = 0; i < 256; i++)
        {
            dac_output_voltage(DAC_CHANNEL_1, i);
            ets_delay_us(5);
        }
    }
}