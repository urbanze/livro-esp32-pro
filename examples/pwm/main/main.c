#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"

#define GPIO_LED 2

void app_main(void)
{
    //Configura o timer para PWM
    ledc_timer_config_t cfg;
    cfg.speed_mode = LEDC_HIGH_SPEED_MODE; //Modo de alta velocidade
    cfg.duty_resolution = LEDC_TIMER_10_BIT; //Resolucao
    cfg.timer_num = LEDC_TIMER_0; //Timer utilizado
    cfg.freq_hz = 1000; //Frequencia
    cfg.clk_cfg = LEDC_AUTO_CLK; //Clock utilizado
    ledc_timer_config(&cfg);

    //Configura o canal de PWM
    ledc_channel_config_t cfg2;
    cfg2.gpio_num = GPIO_LED; //GPIO
    cfg2.speed_mode = LEDC_HIGH_SPEED_MODE; //Modo de alta velocidade
    cfg2.channel = LEDC_CHANNEL_0; //Canal de PWM0
    cfg2.intr_type = LEDC_INTR_DISABLE; //Interrupcoes OFF
    cfg2.timer_sel = LEDC_TIMER_0; //Timer utilizado
    cfg2.duty = 0; //Valor inicial de duty cycle
    cfg2.hpoint = 0;
    ledc_channel_config(&cfg2);
    ledc_fade_func_install(0);

    while (1)
    {
        //Configura o canal PWM0 para ir a [0] de duty cycle por 1seg.
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);

        //Configura o canal PWM0 para ir a [1023] de duty cycle por 1seg.
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1023, 1000, LEDC_FADE_WAIT_DONE);
    }
}