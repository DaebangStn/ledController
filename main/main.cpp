#include "main.h"


static const char* TAG = "main";


void app_main()
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    wait_for_ip();
    connect_server();
    uint8_t recv_buf[90];

    while(1){
        get_image(1, 0, recv_buf, 90);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}
