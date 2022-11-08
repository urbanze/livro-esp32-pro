#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <driver/rtc_cntl.h>

void IRAM_ATTR bod_isr(void*z)
{
    //Limpa o bit de BOD_INT
    REG_SET_BIT(0x3FF48048, (1<<7));

    //Faz algo especifico do usuario
    for (int i = 0; i < 5; i++)
    {
        ESP_EARLY_LOGE(__func__, "BOD Trigged [%d]", i);
    }
    
    //Aborta (reinicia) o sistema
    abort();
}

void app_main(void)
{
    //Registra a funcao de ISR para o bit 7 (BOD_INT) do reg. 0x3FF4803C
    rtc_isr_register(bod_isr, NULL, (1<<7));
}