#include "bluetooth.h"
#include "control.h"
#include "usart.h"

#define BLUETOOTH_FRAME_HEADER 0xA5
#define BLUETOOTH_FRAME_TAIL 0x5A
#define BLUETOOTH_SPEED 2400

static uint8_t rx_byte;
static uint8_t rx_state;
static volatile Bluetooth_Command_t pending_command;
static volatile uint8_t command_ready;

static void Bluetooth_ProcessByte(uint8_t byte) {
  switch (rx_state) {
  case 0:
    if (byte == BLUETOOTH_FRAME_HEADER) {
      rx_state = 1;
    }
    break;

  case 1:
    pending_command = (Bluetooth_Command_t)byte;
    rx_state = 2;
    break;

  case 2:
    rx_state = 0;

    if (byte == BLUETOOTH_FRAME_TAIL) {
      command_ready = 1;
    }
    break;

  default:
    rx_state = 0;
    break;
  }
}

void Bluetooth_Init(void) {
  rx_state = 0;
  rx_byte = 0;
  pending_command = BT_CMD_NONE;
  command_ready = 0;

  HAL_UART_Receive_DMA(&huart3, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance != USART3) {
    return;
  }

  Bluetooth_ProcessByte(rx_byte);
  HAL_UART_Receive_DMA(&huart3, &rx_byte, 1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance != USART3) {
    return;
  }

  rx_state = 0;
  command_ready = 0;
  HAL_UART_Receive_DMA(&huart3, &rx_byte, 1);
}

uint8_t Bluetooth_GetPacket(Bluetooth_Packet_t *packet) {
  if (packet == NULL || command_ready == 0) {
    return 0;
  }

  __disable_irq();
  packet->command = pending_command;
  packet->speed = BLUETOOTH_SPEED;
  command_ready = 0;
  __enable_irq();

  return 1;
}

void Bluetooth_Task(void) {
  Bluetooth_Packet_t packet;

  if (!Bluetooth_GetPacket(&packet)) {
    return;
  }

  switch (packet.command) {
  case BT_CMD_FORWARD:
    Control_Forward(packet.speed);
    break;

  case BT_CMD_BACKWARD:
    Control_Backward(packet.speed);
    break;

  case BT_CMD_LEFT:
    Control_TurnLeft(packet.speed);
    break;

  case BT_CMD_RIGHT:
    Control_TurnRight(packet.speed);
    break;

  case BT_CMD_STOP:
    Control_Stop();
    break;

  default:
    break;
  }
}
