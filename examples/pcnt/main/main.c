#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/pcnt.h"


void app_main(void)
{
    pcnt_config_t cfg;
    cfg.pulse_gpio_num = 0;
    cfg.ctrl_gpio_num = -1;
    cfg.lctrl_mode = PCNT_MODE_KEEP;
    cfg.hctrl_mode = PCNT_MODE_KEEP;
    cfg.pos_mode = PCNT_COUNT_DIS;
    cfg.neg_mode = PCNT_COUNT_INC;
    cfg.counter_h_lim = INT16_MAX;
    cfg.counter_l_lim = INT16_MIN;
    cfg.unit = PCNT_UNIT_0;
    cfg.channel = PCNT_CHANNEL_0;
    pcnt_unit_config(&cfg);
    pcnt_set_filter_value(PCNT_UNIT_0, 1023);
    pcnt_filter_enable(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);

    while (1)
    {
        int16_t ctr = 0;
        pcnt_get_counter_value(PCNT_UNIT_0, &ctr);
        pcnt_counter_clear(PCNT_UNIT_0);
        ESP_LOGI(__func__, "Ctr: [%d]", ctr);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}