#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "BluetoothA2DPSink.h"
void setupBluetooth(void (*bluetoothMetadataCallback)(int, const uint8_t *text), uint8_t BCK_PIN, uint8_t WS_PIN, uint8_t DATA_OUT_PIN);
bool isBluetoothOn();
void startBluetooth();
void stopBluetooth();
void playBluetooth();
void pauseBluetooth();

#endif  // _BLUETOOTH_H_