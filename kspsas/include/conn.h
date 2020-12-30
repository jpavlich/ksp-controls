#pragma once
#include <Arduino.h>
#include <cstdint>
#define KRPC_COMMS_CUSTOM
typedef struct
{
  uint32_t speed;
  uint8_t config;
} krpc_connection_config_t;

typedef USBSerial *krpc_connection_t;