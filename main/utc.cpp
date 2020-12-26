#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "lwip/apps/sntp.h"


static const char *TAG = "sntp";

static bool received = false;

int utc(void)
{
    time_t now;

    if(!received){
        ESP_LOGI(TAG, "Initializing SNTP");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();

        // wait for time to be set
        struct tm timeinfo = { 0 };
        time_t received_utc = 0;
        int retry = 0;
        const int retry_count = 10;

        while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
            ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            time(&received_utc);
            localtime_r(&received_utc, &timeinfo);

            received = true;
        }
    }

    time(&now);
    ESP_LOGI(TAG, "UTC time is %ld", now);

    return now;
}