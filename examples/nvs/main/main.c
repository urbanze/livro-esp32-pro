#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <nvs_flash.h>

nvs_handle_t nvs_handler;

void app_main(void)
{
    //Inicializa o driver da NVS
    esp_err_t err = nvs_flash_init_partition("nvs");
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase_partition("nvs"));
        err = nvs_flash_init_partition("nvs");
    }

    ESP_ERROR_CHECK(err);


    //Inicializa o namespace especifico e atribui ao handler
    err = nvs_open_from_partition("nvs", "cfgs", NVS_READWRITE, &nvs_handler);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__, "Falha ao abrir particao, errno: [%d]", err);
    }


    //Le o valor da chave
    int32_t ctr = 0;
    err = nvs_get_i32(nvs_handler, "boot_ctr", &ctr);
    if (err == ESP_OK)
    {
        ESP_LOGI(__func__, "Valor da chave: [%d]", ctr);
    }
    else if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW(__func__, "Chave ainda nao existe em memoria");
    }
    else
    {
        ESP_LOGE(__func__, "Falha ao ler chave, errno: [%d]", err);
    }


    //Atualiza o valor da chave
    ESP_LOGI(__func__, "Atualizando valor da chave para [%d]", ++ctr);
    err = nvs_set_i32(nvs_handler, "boot_ctr", ctr);
    if (err != ESP_OK)
    {
        ESP_LOGE(__func__, "Falha ao atualizar chave, errno: [%d]", err);
    }

    //Salva as alteracoes na flash e fecha a NVS para reinicio
    nvs_commit(nvs_handler);
    nvs_close(nvs_handler);
    vTaskDelay(pdMS_TO_TICKS(5000));
    esp_restart();
}