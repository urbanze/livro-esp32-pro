#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include <esp_wifi.h>
#include <nvs_flash.h>


//Prototipo de funcoes
static void net_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_ap(const char *ssid, const char* password);


//Handler para eventos do WiFi
static void net_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_AP_START)
        {
            ESP_LOGI(__func__, "WiFi AP: Start");
        }
        else if (event_id == WIFI_EVENT_AP_STACONNECTED)
        {
            ESP_LOGI(__func__, "WiFi AP: New station connected");
        }
        else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
        {
            ESP_LOGW(__func__, "WiFi AP: Station disconnected");
        }
    }
}

void wifi_ap(const char *ssid, const char* password)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t wcfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wcfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &net_handler, NULL, NULL));


    wifi_config_t ap_cfg;
    memset(&ap_cfg, 0, sizeof(ap_cfg));
    strncpy((char*)ap_cfg.ap.ssid, ssid, 32);
    strncpy((char*)ap_cfg.ap.password, password, 64);
    ap_cfg.ap.channel = 1;
    ap_cfg.ap.max_connection = 10;
    ap_cfg.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ap_cfg.ap.beacon_interval = 100;
    

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void)
{
    //Inicializa e cria uma rede WiFi
    wifi_ap("Livro-ESP32-Profissional", "1234567890");
}