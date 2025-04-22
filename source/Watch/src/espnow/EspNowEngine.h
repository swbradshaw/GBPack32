#pragma once

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    byte version;
    byte message_type; // 0 = heartbeat, 1 = event
    char event_name[32];
    char event_detail1[32];
} struct_message;


class EspNowEngine
{
    public:
        EspNowEngine();
        ~EspNowEngine();

        void init();
        void deinit();
        void sendEvent(String eventName);
        void sendEventWithDetail(String eventName, String eventDetail);
        void setPeerAddress(const uint8_t *address);
        void setOnDataSentCallback(esp_now_send_cb_t callback);
        void setOnDataRecvCallback(esp_now_recv_cb_t callback);

    private:
        esp_now_peer_info_t peerInfo;
        esp_now_send_cb_t onDataSentCallback;
        esp_now_recv_cb_t onDataRecvCallback;
};
