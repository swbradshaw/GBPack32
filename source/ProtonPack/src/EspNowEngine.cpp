#include "EspNowEngine.h"

EspNowEngine::EspNowEngine()
{
    // Constructor implementation
}

EspNowEngine::~EspNowEngine()
{
    // Destructor implementation
}

void EspNowEngine::deinit()
{
    esp_now_deinit();
    WiFi.mode(WIFI_OFF);
    delay(100);
    Serial.println("ESP Now deinitialized");
}
void EspNowEngine::init()
{
    WiFi.mode(WIFI_STA);

    delay(100);

    Serial.print("ESP Now initialized for MAC: ");
    Serial.println(WiFi.macAddress());
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register send callback
    esp_now_register_send_cb(onDataSentCallback);

    // Register receive callback
    esp_now_register_recv_cb(onDataRecvCallback);

    debugln("ESPNOW ready.");
}

void EspNowEngine::notify(EventArgs args) {
  if (args.eventName == EVENT_AUDIO_INIT) {
      init();
  }
  if (args.eventName == EVENT_AUDIO_SHUTDOWN) {
      deinit();
  }
}

void EspNowEngine::sendEvent(String eventName)
{
    struct_message message;
    message.version = 1; // Example version
    message.message_type = 1; // Example message type
    eventName.toCharArray(message.event_name, sizeof(message.event_name)); // Copy event name to message data

    esp_err_t result = esp_now_send(peerInfo.peer_addr, (uint8_t *)&message, sizeof(message));
    if (result == ESP_OK) {
        Serial.printf("Event %s sent successfully\n", eventName.c_str());
    } else {
        Serial.printf("Error sending event: %s\n", esp_err_to_name(result));
    }
}

void EspNowEngine::sendEventWithDetail(String eventName, String eventDetail)
{
    struct_message message;
    message.version = 1; // Example version
    message.message_type = 1; // Example message type
    eventName.toCharArray(message.event_name, sizeof(message.event_name)); // Copy event name to message data
    eventDetail.toCharArray(message.event_detail1, sizeof(message.event_detail1)); // Copy event name to message data

    esp_err_t result = esp_now_send(peerInfo.peer_addr, (uint8_t *)&message, sizeof(message));
    if (result == ESP_OK) {
        Serial.printf("Event %s sent successfully\n", eventName.c_str());
    } else {
        Serial.printf("Error sending event: %s\n", esp_err_to_name(result));
    }
}

void EspNowEngine::setPeerAddress(const uint8_t *address)
{
    memcpy(peerInfo.peer_addr, address, sizeof(peerInfo.peer_addr));
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    } else {
        Serial.print("Peer added: ");
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", address[0], address[1], address[2], address[3], address[4], address[5]);
    }
}

void EspNowEngine::setOnDataSentCallback(esp_now_send_cb_t callback)
{
    onDataSentCallback = callback;
}
void EspNowEngine::setOnDataRecvCallback(esp_now_recv_cb_t callback)
{
    onDataRecvCallback = callback;
}