#pragma once

#include <stdio.h>
#include <string>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

#include "nvs_flash.h"

#include "wifi.h"
#include "utc.h"
#include "socket.h"


#define PAYLOAD_LEN         90
#define TIMEOUT_TCP         5000
#define WIFI_MAXIMUM_RETRY  5


using namespace std;

extern "C" {
	void app_main(void);
}