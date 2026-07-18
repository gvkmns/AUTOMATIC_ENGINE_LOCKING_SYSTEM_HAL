/*
 * buzzer_led.c
 */

#include "buzzer_led.h"

void AlertIO_Init(AlertIO_HandleTypeDef *hio)
{
    HAL_GPIO_WritePin(hio->buzzer_port, hio->buzzer_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hio->led_port, hio->led_pin, GPIO_PIN_RESET);
}

void AlertIO_On(AlertIO_HandleTypeDef *hio)
{
    HAL_GPIO_WritePin(hio->buzzer_port, hio->buzzer_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hio->led_port, hio->led_pin, GPIO_PIN_SET);
}

void AlertIO_Off(AlertIO_HandleTypeDef *hio)
{
    HAL_GPIO_WritePin(hio->buzzer_port, hio->buzzer_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hio->led_port, hio->led_pin, GPIO_PIN_RESET);
}

void AlertIO_Beep(AlertIO_HandleTypeDef *hio, uint32_t on_ms, uint32_t off_ms)
{
    HAL_GPIO_WritePin(hio->buzzer_port, hio->buzzer_pin, GPIO_PIN_SET);
    HAL_Delay(on_ms);
    HAL_GPIO_WritePin(hio->buzzer_port, hio->buzzer_pin, GPIO_PIN_RESET);
    HAL_Delay(off_ms);
}
