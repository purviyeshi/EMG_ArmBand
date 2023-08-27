#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_log.h"
#include "nvs_flash.h"  // Include the NVS flash header

static const char *TAG = "READINGS";

// Define the sender and receiver MAC addresses
uint8_t unicastAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C};
uint8_t receiverAddress[] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C}; // Replace with the MAC address of the receiving ESP    Receiver MAC Address: E8:31:CD:C3:EF:8C

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

    // The message to be sent
    const char* message = "Hello, ESP-NOW!";

    // Send the message
    esp_err_t result = esp_now_send(unicastAddress, (uint8_t *)message, strlen(message));
    if (result == ESP_OK) {
        ESP_LOGI(TAG, "Message sending succeeded");
    } else {
        ESP_LOGE(TAG, "Message sending failed_______");
    }
}

