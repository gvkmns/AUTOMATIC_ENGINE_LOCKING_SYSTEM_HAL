/*
 * lcd_i2c.c
 *
 * PCF8574-backed 16x2 LCD, 4-bit mode, driven over I2C.
 */

#include "lcd_i2c.h"
#include <string.h>

static I2C_HandleTypeDef *s_hi2c = NULL;
static uint8_t s_backlight = 0x08; /* backlight bit ON by default */

/* ---- low level PCF8574 <-> LCD bit mapping ----
 * P7 P6 P5 P4 P3 P2 P1 P0
 * D7 D6 D5 D4 BL EN RW RS
 */
static void LCD_I2C_Write(uint8_t data)
{
    HAL_I2C_Master_Transmit(s_hi2c, LCD_I2C_ADDR, &data, 1, 100);
}

static void LCD_PulseEnable(uint8_t data)
{
    LCD_I2C_Write(data | 0x04);   /* EN = 1 */
    HAL_Delay(1);
    LCD_I2C_Write(data & ~0x04);  /* EN = 0 */
    HAL_Delay(1);
}

static void LCD_SendNibble(uint8_t nibble, uint8_t rs)
{
    uint8_t data = (nibble & 0xF0) | s_backlight | (rs ? 0x01 : 0x00);
    LCD_I2C_Write(data);
    LCD_PulseEnable(data);
}

static void LCD_SendByte(uint8_t value, uint8_t rs)
{
    LCD_SendNibble(value & 0xF0, rs);
    LCD_SendNibble((uint8_t)((value << 4) & 0xF0), rs);
}

static void LCD_Command(uint8_t cmd)
{
    LCD_SendByte(cmd, 0);
}

static void LCD_Data(uint8_t data)
{
    LCD_SendByte(data, 1);
}

void LCD_Init(I2C_HandleTypeDef *hi2c)
{
    s_hi2c = hi2c;

    HAL_Delay(50);
    LCD_SendNibble(0x30, 0);
    HAL_Delay(5);
    LCD_SendNibble(0x30, 0);
    HAL_Delay(1);
    LCD_SendNibble(0x30, 0);
    HAL_Delay(1);
    LCD_SendNibble(0x20, 0); /* switch to 4-bit mode */

    LCD_Command(0x28); /* 4-bit, 2 line, 5x8 dots */
    LCD_Command(0x0C); /* display ON, cursor OFF */
    LCD_Command(0x06); /* entry mode: increment cursor */
    LCD_Command(0x01); /* clear display */
    HAL_Delay(2);
}

void LCD_Clear(void)
{
    LCD_Command(0x01);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
    static const uint8_t rowOffsets[2] = {0x00, 0x40};
    LCD_Command((uint8_t)(0x80 | (col + rowOffsets[row & 0x01])));
}

void LCD_Print(const char *str)
{
    while (*str)
    {
        LCD_Data((uint8_t)*str++);
    }
}
