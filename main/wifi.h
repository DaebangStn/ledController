#pragma once


#define WIFI_SSID "geon"
#define WIFI_PASS  "dlarjsgh1107"


#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

static EventGroupHandle_t wifi_event_group;

void wifi_init_sta();
void wait_for_ip();