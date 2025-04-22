#pragma once
#include <Arduino.h>

uint8_t devPackAddress[] = {0x30, 0xae, 0xa4, 0xfe, 0x07, 0x7c}; // desk dev device
uint8_t realPackAddress[] = {0xec, 0x64, 0xc9, 0x86, 0x57, 0x0c}; // proton pack

uint8_t* packs[] = {devPackAddress, realPackAddress};

int32_t packSelection = 0;