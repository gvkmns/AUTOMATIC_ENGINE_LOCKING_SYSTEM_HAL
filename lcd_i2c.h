/*
 * lcd_i2c.h
 *
 * Minimal 16x2 character LCD driver over I2C using a PCF8574 backpack,
 * built on STM32 HAL's HAL_I2C_Master_Transmit(). Assumes I2C peripheral
 * already configured in CubeMX (handle passed in at init, e.g. hi2c1).
 */

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define LCD_I2C_ADDR   (0x27 << 1)   /* Common PCF8574 address, verify yours */

void LCD_Init(I2C_HandleTypeDef *hi2c);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Print(const char *str);

#endif /* LCD_I2C_H */
