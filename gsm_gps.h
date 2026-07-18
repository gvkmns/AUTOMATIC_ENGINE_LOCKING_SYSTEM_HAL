/*
 * gsm_gps.h
 *
 * SIM800C GSM/GPRS/GNSS driver over UART using STM32 HAL.
 * Sends AT commands for SMS and, if the module/firmware supports
 * AT+CGNSINF, retrieves a GNSS fix.
 */

#ifndef GSM_GPS_H
#define GSM_GPS_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define GSM_RX_BUF_SIZE   160U

void    GSM_Init(UART_HandleTypeDef *huart);
void    GSM_SendCommand(const char *cmd, uint32_t timeout_ms);
uint8_t GSM_GetLocation(char *out_buf, uint16_t out_buf_len);
void    GSM_SendSMS(const char *phone_number, const char *message);

#endif /* GSM_GPS_H */
