
#include "../common.h"
#include "bluetooth.h"

BluetoothA2DPSink a2dp_sink;

void (*bluetoothCallback)(int, const uint8_t *text);

int volume = 0;
bool bluetoothOn = false;
void volumeChanged(int newVolume) {

    debugln(newVolume);
    volume = newVolume;
}

// for esp_a2d_connection_state_t see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_a2dp.html#_CPPv426esp_a2d_connection_state_t
void connection_state_changed(esp_a2d_connection_state_t state, void* ptr) {

    debugln("connection_state_changed");
    debugln(a2dp_sink.to_str(state));

}

// for esp_a2d_audio_state_t see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_a2dp.html#_CPPv421esp_a2d_audio_state_t
void audio_state_changed(esp_a2d_audio_state_t state, void* ptr) {

    debugln("audio_state_changed");
    debugln(a2dp_sink.to_str(state));
}


void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  bluetoothCallback((int)id, text);
}

void setupBluetooth(void (*bluetoothMetadataCallback)(int, const uint8_t *text), uint8_t BCK_PIN, uint8_t WS_PIN, uint8_t DATA_OUT_PIN) {
    bluetoothCallback = bluetoothMetadataCallback;

    i2s_pin_config_t my_pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = BCK_PIN,
        .ws_io_num = WS_PIN,
        .data_out_num = DATA_OUT_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    a2dp_sink.set_pin_config(my_pin_config);

    a2dp_sink.set_on_volumechange(volumeChanged);
    a2dp_sink.set_on_connection_state_changed(connection_state_changed);
    a2dp_sink.set_on_audio_state_changed(audio_state_changed);
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);

}
bool isBluetoothOn() {
    return bluetoothOn;
}
void startBluetooth() {
    debugln("Starting Bluetooth");
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_BT);
    char ssid[17];
    sprintf(ssid, "Proton Pack %02X%02X", baseMac[4], baseMac[5]);

    a2dp_sink.set_auto_reconnect(true);
    a2dp_sink.start(ssid);
    bluetoothOn = true;
    debugln("Bluetooth started");
}
void stopBluetooth() {
    a2dp_sink.stop();
    //a2dp_sink.disconnect();
    a2dp_sink.end();
    // a2dp_source.set_connected(false);
    bluetoothOn = false;
}
void playBluetooth() {
    a2dp_sink.play();
}
void pauseBluetooth() {
    a2dp_sink.pause();
}
