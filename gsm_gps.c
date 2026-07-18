/*
 * gsm_gps.c
 */

#include "gsm_gps.h"
#include <string.h>
#include <stdio.h>

static UART_HandleTypeDef *s_huart = NULL;
static uint8_t s_rxByte;
static char s_rxBuf[GSM_RX_BUF_SIZE];
static volatile uint16_t s_rxIdx = 0;

/* Call this from HAL_UART_RxCpltCallback() in main.c/stm32xx_it.c when
 * huart == s_huart, then re-arm with HAL_UART_Receive_IT(). See main.c. */
void GSM_UART_RxCallback(void)
{
    if (s_rxIdx < (GSM_RX_BUF_SIZE - 1))
    {
        s_rxBuf[s_rxIdx++] = (char)s_rxByte;
    }
    HAL_UART_Receive_IT(s_huart, &s_rxByte, 1);
}

void GSM_Init(UART_HandleTypeDef *huart)
{
    s_huart = huart;
    memset(s_rxBuf, 0, sizeof(s_rxBuf));
    HAL_UART_Receive_IT(s_huart, &s_rxByte, 1);

    GSM_SendCommand("AT\r\n", 500);
    GSM_SendCommand("ATE0\r\n", 500);        /* echo off */
    GSM_SendCommand("AT+CMGF=1\r\n", 500);   /* SMS text mode */
}

void GSM_SendCommand(const char *cmd, uint32_t timeout_ms)
{
    s_rxIdx = 0;
    memset(s_rxBuf, 0, sizeof(s_rxBuf));
    HAL_UART_Transmit(s_huart, (uint8_t *)cmd, (uint16_t)strlen(cmd), 500);
    HAL_Delay(timeout_ms);
}

/**
 * @brief Queries GNSS fix via AT+CGNSINF and writes a maps URL (or a
 *        "no fix" message) into out_buf.
 * @return 1 if a fix was parsed, 0 otherwise.
 */
uint8_t GSM_GetLocation(char *out_buf, uint16_t out_buf_len)
{
    GSM_SendCommand("AT+CGNSINF\r\n", 1500);

    /* Very small, dependency-free field splitter looking for
     * +CGNSINF: 1,1,<time>,<lat>,<lon>,<alt>,...                */
    char *resp = s_rxBuf;
    char *fields[8] = {0};
    uint8_t fieldCount = 0;
    char *token = resp;

    for (char *p = resp; *p != '\0' && fieldCount < 8; p++)
    {
        if (*p == ',')
        {
            *p = '\0';
            fields[fieldCount++] = token;
            token = p + 1;
        }
    }

    if (fieldCount >= 5 && strlen(fields[3]) > 0 && strlen(fields[4]) > 0)
    {
        snprintf(out_buf, out_buf_len,
                 "https://maps.google.com/?q=%s,%s", fields[3], fields[4]);
        return 1U;
    }

    snprintf(out_buf, out_buf_len, "Location unavailable (no GPS fix)");
    return 0U;
}

void GSM_SendSMS(const char *phone_number, const char *message)
{
    char cmd[64];

    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r\n", phone_number);
    GSM_SendCommand(cmd, 500);

    HAL_UART_Transmit(s_huart, (uint8_t *)message,
                       (uint16_t)strlen(message), 1000);

    uint8_t ctrlZ = 0x1A; /* Ctrl+Z terminates and sends the SMS body */
    HAL_UART_Transmit(s_huart, &ctrlZ, 1, 3000);
    HAL_Delay(3000);
}
