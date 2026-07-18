/*
 * buzzer_led.h
 *
 * Simple GPIO-driven buzzer and status LED.
 */

#ifndef BUZZER_LED_H
#define BUZZER_LED_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *buzzer_port;
    uint16_t      buzzer_pin;
    GPIO_TypeDef *led_port;
    uint16_t      led_pin;
} AlertIO_HandleTypeDef;

void AlertIO_Init(AlertIO_HandleTypeDef *hio);
void AlertIO_On(AlertIO_HandleTypeDef *hio);
void AlertIO_Off(AlertIO_HandleTypeDef *hio);
void AlertIO_Beep(AlertIO_HandleTypeDef *hio, uint32_t on_ms, uint32_t off_ms);

#endif /* BUZZER_LED_H */
