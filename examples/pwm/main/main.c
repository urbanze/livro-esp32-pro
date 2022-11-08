#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"


void app_main(void)
{
    ledc_timer_config_t cfg;
    cfg.speed_mode = LEDC_HIGH_SPEED_MODE;
    cfg.duty_resolution = LEDC_TIMER_10_BIT;
    cfg.timer_num = LEDC_TIMER_0;
    cfg.freq_hz = 1000;
    cfg.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&cfg);

    ledc_channel_config_t cfg2;
    cfg2.gpio_num = 2;
    cfg2.speed_mode = LEDC_HIGH_SPEED_MODE;
    cfg2.channel = LEDC_CHANNEL_0;
    cfg2.intr_type = LEDC_INTR_DISABLE;
    cfg2.timer_sel = LEDC_TIMER_0;
    cfg2.duty = 0;
    cfg2.hpoint = 0;
    ledc_channel_config(&cfg2);
    ledc_fade_func_install(0);

    while (1)
    {
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
        ledc_set_fade_time_and_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 1023, 1000, LEDC_FADE_WAIT_DONE);
    }
}