#include <stdio.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "math.h"
#include "driver/gpio.h"

#define LED_PIN         GPIO_NUM_2   // Chân điều khiển LED
#define SCL_PIN         GPIO_NUM_9
#define SDA_PIN         GPIO_NUM_8

const char SSID[] = "VLDCNC1";
const char PASS[] = "khoavatly01";
const char broker_ip[] = "mqtt://113.161.84.132:8883";

esp_mqtt_client_handle_t client;

void wifi_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void connect_to_wifi();
void connect_to_mqtt();
void adxl345_start();

void app_main(void)
{
    // Khởi tạo NVS và kết nối Wi-Fi
    ESP_ERROR_CHECK(nvs_flash_init());
    connect_to_wifi();
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    // Khởi tạo MQTT
    connect_to_mqtt();
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    printf("Finish initialize system!");

    // // Khởi chạy task đọc cảm biến ADXL345
    // xTaskCreate(adxl345_start, "adxl345_start", 4096, NULL, 4, NULL);
    // printf("Finish create sensor tasks!");
}

void wifi_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_STA_START)
    {
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("Connected to wifi\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("Disconnected from wifi\nRetrying to Connect...\n");
        esp_wifi_connect();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        printf("Wifi got IP...\n");
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        printf("CONNECTED TO BROKER\n");
        // Đăng ký topic điều khiển LED
        esp_mqtt_client_subscribe(client, "led_control", 0);
        break;

    case MQTT_EVENT_DISCONNECTED:
        printf("DISCONNECTED FROM BROKER\n");
        esp_mqtt_client_reconnect(client);
        break;

    case MQTT_EVENT_DATA:
        printf("MQTT_EVENT_DATA received\n");
        printf("TOPIC: %.*s\n", event->topic_len, event->topic);
        printf("DATA: %.*s\n", event->data_len, event->data);

        // Kiểm tra xem topic có phải là "led_control" không
        if (strncmp(event->topic, "led_control", event->topic_len) == 0)
        {
            // Kiểm tra nội dung của tin nhắn
            if (strncmp(event->data, "on", event->data_len) == 0)
            {
                gpio_set_level(LED_PIN, 1);  // Bật LED
                printf("LED TURNED ON\n");
            }
            else if (strncmp(event->data, "off", event->data_len) == 0)
            {
                gpio_set_level(LED_PIN, 0);  // Tắt LED
                printf("LED TURNED OFF\n");
            }
        }
        break;

    default:
        break;
    }
}

void connect_to_wifi()
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "VLDCNC1",
            .password = "khoavatly01"}};

    strcpy((char *)wifi_config.sta.ssid, SSID);
    strcpy((char *)wifi_config.sta.password, PASS);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);

    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();
    printf("wifi_init_STA finished ----> SSID:%s  password:%s", SSID, PASS);
}

void connect_to_mqtt()
{
   esp_mqtt_client_config_t mqtt_cfg = {
    .broker = {
        .address = {
            .uri = broker_ip
        }
    },
    .credentials = {
        .username = "iot",
        .authentication = {
            .password = "iot@123456"
        }
    }
};

    // Khởi tạo client MQTT
    client = esp_mqtt_client_init(&mqtt_cfg);  // Sử dụng biến client toàn cục
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}