#include <string.h>
#include <sstream>
#include <string>
#include <iostream>

#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "socket.h"
#include "utc.h"


const int IPV4_GOTIP_BIT = BIT0;

static const char *TAG = "socket";

static const string clip_name = "mario";
static string bs = "/";
// image url = req1 + str_img(num_img, num_part) + req2 
static string req1 = string("GET /image/conv_") +clip_name + bs + clip_name;
static string req2 = string(" HTTP/1.1\r\nConnection: keep-alive\r\n\r\n");


static int sock;

void connect_server()
{        
    struct sockaddr_in destAddr;
    inet_pton(AF_INET, HOST_IP_ADDR, &(destAddr.sin_addr.s_addr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(PORT);

    sock =  socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    int err = connect(sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Successfully connected");
}


static void flush_until_body(){
  uint8_t temp;
  while(true){
    recv(sock, &temp, 1, 0);
    if(temp == 0x0d){
        recv(sock, &temp, 1, 0);
        recv(sock, &temp, 1, 0);
      if(temp == 0x0d){
        recv(sock, &temp, 1, 0);
        return;
      }
    }
  }
}


static string str_img(int num_img, int num_part){
  char temp[3];
  stringstream sstream;
  sstream << num_part;
  string s2 = sstream.str();

  sprintf(temp, "%02d", num_img);
  temp[2] = '\0';
  string s1 = temp;
  return s1 + string("_") + s2;
}


void get_image(int num_img, int num_part, uint8_t* buf, int size){
// error handling when socket is closed is needed
    int timestamp = utc();
    string req = req1 + str_img(num_img, num_part) + req2;
    ESP_LOGI(TAG, "request on %s", req.c_str());

    int err = send(sock, req.c_str(), req.length(), 0);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
        return;
    }

    int len = recv(sock, buf, size, 0);
    // Error occured during receiving
    if (len < 0) {
        ESP_LOGE(TAG, "recv failed: errno %d", errno);
        return;
    }
    // Data received
    else {
        ESP_LOGI(TAG, "Received %d bytes from %s:", len, HOST_IP_ADDR);
    }
}