#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define NUM_SENSORS 5

#define DEFAULT_VREF 1100

static const char *TAG = "READINGS";

// Define the sender and receiver MAC addresses
uint8_t unicastAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C};            //   Receiver MAC Address: E8:31:CD:C3:EF:8C
uint8_t receiverAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C};

// Callback function to handle ESP-NOW sending status
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI(TAG, "Message sent successfully");
    } else {
        ESP_LOGE(TAG, "Message sending failed");
    }
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));  
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());
    esp_now_register_send_cb(OnDataSent);

    // Add the receiver
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));

    // Configuration for the ADC
    esp_err_t err;
    adc1_config_width(ADC_WIDTH_BIT_12);  // Configure ADC width to 12 bits
    err = adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);          //       Configure ADC channel attenuation
    err = adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    err = adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); 
    
    if (err != ESP_OK) {
        // Handle error here
    } 
    
    uint16_t raw_sensor_readings[NUM_SENSORS];          //       Read and store raw values from ADC channels    
    
    const adc1_channel_t sensor_channels[NUM_SENSORS] = {
        ADC1_CHANNEL_4,
        ADC1_CHANNEL_7,
        ADC1_CHANNEL_6,
        ADC1_CHANNEL_3,
        ADC1_CHANNEL_0
    };
    
    while (1) {
        // Read analog sensor data
        //uint32_t sensor_value = adc1_get_raw(ADC1_CHANNEL_0);  // Read sensor on ADC channel 0


	for (int i = 0; i < NUM_SENSORS; i++) 
        {
            raw_sensor_readings[i] = adc1_get_raw(sensor_channels[i]);
        }


        // Convert the sensor value to a string
        //char sensor_data_str[20];  // Adjust buffer size as needed
        //snprintf(sensor_data_str, sizeof(sensor_data_str), "%d", sensor_value);
        
        char readings_str[100];
        
        snprintf(readings_str, sizeof(readings_str), "LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d",raw_sensor_readings[0], raw_sensor_readings[1], raw_sensor_readings[2], raw_sensor_readings[3], raw_sensor_readings[4]);
        

        // Send the sensor data
        esp_err_t result = esp_now_send(unicastAddress, (uint8_t *)readings_str, strlen(readings_str));
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "Sensor data sent successfully");
        } else {
            ESP_LOGE(TAG, "Sensor data sending failed");
        }

        // Add a delay to control the data sending rate
        vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay time as needed (1 second in this example)
    }
}

