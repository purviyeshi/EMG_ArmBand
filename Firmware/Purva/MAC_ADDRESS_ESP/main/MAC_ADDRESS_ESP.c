#include "esp_system.h"
#include "esp_wifi.h"

void app_main() 
{
    
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("Transmitter MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);                        
}


//	Transmitter MAC Address: 44:17:93:E6:35:08
//	Receiver MAC Address: E8:31:CD:C3:EF:8C
