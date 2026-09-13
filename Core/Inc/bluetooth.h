#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "main.h"

typedef enum {
  BT_CMD_NONE = 0x00,
  BT_CMD_FORWARD = 0x01,
  BT_CMD_BACKWARD = 0x02,
  BT_CMD_LEFT = 0x03,
  BT_CMD_RIGHT = 0x04,
  BT_CMD_STOP = 0x05

} Bluetooth_Command_t;

typedef struct {
  Bluetooth_Command_t command;
  uint16_t speed;
} Bluetooth_Packet_t;

void Bluetooth_Init(void);

void Bluetooth_Task(void);

uint8_t Bluetooth_GetPacket(Bluetooth_Packet_t *packet);

#endif /* BLUETOOTH_H */
