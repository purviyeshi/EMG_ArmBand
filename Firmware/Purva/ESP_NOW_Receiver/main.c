#include "esp_now.h"
#include "esp_log.h"
#include <string.h>
#include "esp_wifi.h"

static const char *TAG = "READINGS";

struct esp_now_peer_info peerInfo;

uint8_t receiverMacAddress[ESP_NOW_ETH_ALEN] = {0xE8, 0x31, 0xCD, 0xC3, 0xEF, 0x8C};         //       Receiver MAC address      E8:31:CD:C3:EF:8C

// ESP-NOW data must be transmitted after Wi-Fi is started, so it is recommended to start Wi-Fi before initializing ESP-NOW
void Initialize_esp_now() {
 esp_err_t init_result = esp_now_init();
    if (init_result != ESP_OK) {
        ESP_LOGE(TAG, "ESP-NOW initialization failed.");
        return;
    } else {
        ESP_LOGI(TAG, "ESP-NOW initialization successful.");
    }
}


void add_peer_device() {
    //         Define peer information

    memcpy(peerInfo.peer_addr, receiverMacAddress, ESP_NOW_ETH_ALEN);             //       Copy the receiver MAC address to the peer info structure
    peerInfo.channel = 0;       // Set the communication channel                  //	   CHANNEL IS FREQUENCY BAND HERE
    peerInfo.encrypt = false;   // Set encryption to false (for testing purposes)

    // Add the peer
    esp_err_t add_peer_result = esp_now_add_peer(&peerInfo);
    if (add_peer_result == ESP_OK) {
        ESP_LOGI(TAG, "Peer added successfully");
    } else {
        ESP_LOGE(TAG, "Failed to add peer");
    }
}



void on_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        printf("Message sent successfully\n");
    } else {
        printf("Error sending message\n");
    }
}



void app_main(void) {

	// Initialize WiFi
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());


	void Initialize_esp_now();
	void add_peer_device();
	
	// Send message
	const char *message = "Hello I'm Purva";
	esp_now_send(receiverMacAddress, (uint8_t *)message, strlen(message) + 1);
	
	void on_send_cb();
}


