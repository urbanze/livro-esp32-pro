#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include "mqtt_client.h"
#include <esp_wifi.h>
#include <nvs_flash.h>


//Prototipo de funcoes
static void net_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void mqtt_event(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
esp_err_t mqtt_handler(esp_mqtt_event_handle_t evt);
void wifi_connect(const char *ssid, const char* password);
void mqtt_init(const char *host);

//Variaveis e objetos
esp_mqtt_client_handle_t mqtt_client;
bool mqtt_connection = false;
bool wifi_connection = false;

//Handler para eventos do WiFi
static void net_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT)
    {
        if (event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect();
            ESP_LOGI(__func__, "WiFi STA: Start");
        }
        else if (event_id == WIFI_EVENT_STA_CONNECTED)
        {
            ESP_LOGI(__func__, "WiFi STA: Connected");
        }
        else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            wifi_connection = false;

            wifi_event_sta_disconnected_t *rsn = (wifi_event_sta_disconnected_t*)event_data;
            ESP_LOGW(__func__, "WiFi STA: Disconnected [%d]", rsn->reason);
        }
    }
    else if (event_base == IP_EVENT)
    {
        if (event_id == IP_EVENT_STA_GOT_IP)
        {
            wifi_connection = true;

            ESP_LOGI(__func__, "Got IP");
        }
    }
}

//Handler temporario para eventos do MQTT
void mqtt_event(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    mqtt_handler(event_data);
}

//Handler para eventos do MQTT
esp_err_t mqtt_handler(esp_mqtt_event_handle_t evt)
{
    if (evt->event_id == MQTT_EVENT_CONNECTED)
    {
        mqtt_connection = true;
        ESP_LOGI(__func__, "MQTT connected");
        
        esp_mqtt_client_subscribe(mqtt_client, "esp32/input", 0);
    }
    else if (evt->event_id == MQTT_EVENT_DISCONNECTED)
    {
        mqtt_connection = false;
        ESP_LOGW(__func__, "MQTT disconnected");
    }
    else if (evt->event_id == MQTT_EVENT_SUBSCRIBED)
    {
        ESP_LOGI(__func__, "MQTT Subscribed");
    }
    else if (evt->event_id == MQTT_EVENT_DATA)
    {
        char data[evt->data_len+1];
        memset(data, 0, sizeof(data));
        strncpy(data, evt->data, evt->data_len);
        ESP_LOGI(__func__, "MQTT received: [%s]", data);
    }
    else if (evt->event_id == MQTT_EVENT_PUBLISHED)
    {
        ESP_LOGI(__func__, "MQTT published");
    }
    else if (evt->event_id == MQTT_EVENT_BEFORE_CONNECT)
    {
        //Nop
    }
    else
    {
        ESP_LOGE(__func__, "Evento desconhecido [%d]", evt->event_id);
    }

    return ESP_OK;
}



void wifi_connect(const char *ssid, const char* password)
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
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wcfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wcfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &net_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &net_handler, NULL, NULL));


    wifi_config_t sta_cfg;
    memset(&sta_cfg, 0, sizeof(sta_cfg));
    strncpy((char*)sta_cfg.sta.ssid, ssid, 32);
    strncpy((char*)sta_cfg.sta.password, password, 64);
    sta_cfg.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    sta_cfg.sta.bssid_set = 0;
    sta_cfg.sta.channel = 0;
    sta_cfg.sta.listen_interval = 0;
    sta_cfg.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    sta_cfg.sta.threshold.rssi = -127;
    sta_cfg.sta.threshold.authmode = WIFI_AUTH_OPEN;
    

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());


    ESP_LOGI(__func__, "Aguardando conexao WiFi");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        if (wifi_connection == true) {break;}
    }
    ESP_LOGI(__func__, "WiFi conectado");
}

void mqtt_init(const char *host)
{
    esp_mqtt_client_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.keepalive = 60;
    cfg.username = "jose";
    cfg.password = "morais";
    cfg.reconnect_timeout_ms = 2000;
    cfg.uri = host;
    //cfg.client_id = "JoseMorais";
    //cfg.network_timeout_ms = 5000;
    //cfg.refresh_connection_after_ms = 300000;

    mqtt_client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event, mqtt_client);
    esp_mqtt_client_start(mqtt_client);
}

void app_main(void)
{
    //Inicializa e conecta no WiFi
    wifi_connect("...", "...");

    //Inicializa e conecta no broker MQTT
    //Nao se esqueca de alterar o IP e porta!
    mqtt_init("mqtt://192.168.1.10:1883");


    int32_t ctr = 0;
    while (1)
    {
        if (mqtt_connection == true)
        {
            char out[32];
            snprintf(out, sizeof(out), "ESP32: [%d]", ctr++);

            esp_mqtt_client_publish(mqtt_client, "esp32/output", out, 0, 1, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}