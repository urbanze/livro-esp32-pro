#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include "esp_sntp.h"
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_http_server.h>


bool connection = false;
httpd_handle_t http_server;
esp_err_t webpage_main_handler(httpd_req_t *req);
esp_err_t webpage_form_handler(httpd_req_t *req);

static const httpd_uri_t webpage_main_cfg = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = webpage_main_handler,
    .user_ctx = NULL
};



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
            connection = false;

            wifi_event_sta_disconnected_t *rsn = (wifi_event_sta_disconnected_t*)event_data;
            ESP_LOGW(__func__, "WiFi STA: Disconnected [%d]", rsn->reason);
        }
    }
    else if (event_base == IP_EVENT)
    {
        if (event_id == IP_EVENT_STA_GOT_IP)
        {
            connection = true;

            ESP_LOGI(__func__, "Got IP");
        }
    }
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
        if (connection == true) {break;}
    }
    ESP_LOGI(__func__, "WiFi conectado");
}



esp_err_t webpage_404_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Falha ao acessar este link");
    return ESP_FAIL;
}

esp_err_t webpage_main_handler(httpd_req_t *req)
{
    const char res[] = "Livro ESP32 Profissional. Por: Jose Morais.<br><br>";
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}


bool start_webserver()
{
    http_server = NULL;
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    //cfg.lru_purge_enable = true;

    ESP_LOGI(__func__, "Starting HTTP Server...");
    if (httpd_start(&http_server, &cfg) != ESP_OK)
    {
        ESP_LOGE(__func__, "Fail to init HTTP Server");
        return false;
    }

    httpd_register_err_handler(http_server, HTTPD_404_NOT_FOUND, webpage_404_handler);
    httpd_register_uri_handler(http_server, &webpage_main_cfg);
    
    ESP_LOGI(__func__, "HTTP Server started");
    return true;
}



void app_main(void)
{
    //Inicializa e conecta no WiFi
    wifi_connect("...", "...");

    if (start_webserver() == false)
    {
        ESP_LOGE(__func__, "Error on startup HTTP, restarting...");
        vTaskDelay(pdMS_TO_TICKS(3000));
        esp_restart();
    }
}