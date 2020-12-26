#pragma once


#include <string>


#define HOST_IP_ADDR "192.168.0.3"
#define PORT 50004


using namespace std;


void connect_server();
void get_server(string*);
void get_image(int, int, uint8_t*, int);