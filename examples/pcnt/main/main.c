#include <stdio.h>
#include <stdint.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/pcnt.h"


void app_main(void)
{
    //Configura o canal de PCNT
    pcnt_config_t cfg;
    cfg.pulse_gpio_num = 0; //GPIO responsavel por contar os pulsos
    cfg.ctrl_gpio_num = -1; //GPIO responsavel pelo controle de fluxo
    cfg.lctrl_mode = PCNT_MODE_KEEP; //O que fazer quando o GPIO_CTRL estiver em LOW: Nada.
    cfg.hctrl_mode = PCNT_MODE_KEEP; //O que fazer quando o GPIO_CTRL estiver em HIGH: Nada.
    cfg.pos_mode = PCNT_COUNT_DIS; //O que fazer em bordas de subida: Nada.
    cfg.neg_mode = PCNT_COUNT_INC; //O que fazer em bordas de descida: Incrementar.
    cfg.counter_h_lim = INT16_MAX; //Valor maximo (usado de threshold para interrupcoes)
    cfg.counter_l_lim = INT16_MIN; //Valor minimo (usado de threshold para interrupcoes)
    cfg.unit = PCNT_UNIT_0; //Unidade utilizada
    cfg.channel = PCNT_CHANNEL_0; //Canal utilizado
    pcnt_unit_config(&cfg);

    //Habilita o filtro para ignorar pulsos pequenos
    pcnt_set_filter_value(PCNT_UNIT_0, 1023);
    pcnt_filter_enable(PCNT_UNIT_0);

    //Reseta o contador
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