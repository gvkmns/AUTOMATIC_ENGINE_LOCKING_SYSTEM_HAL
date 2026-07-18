# Automatic Engine Locking System — STM32 HAL Implementation

Modular STM32 HAL firmware, split into one driver module per subsystem
(as in a typical CubeMX-generated project). Locks the engine on either
of two independent triggers:

1. **Alcohol detected** (MQ-3 sensor above threshold)
2. **Tilt / rollover detected** (tilt switch, e.g. after a crash)

Either trigger stops the motor/relay standing in for the engine, sounds
the buzzer + LED, updates the LCD, and — rate-limited by a cooldown —
sends an SMS with a GPS location link via the SIM800C module.

## File layout

```
Core/
  Inc/
    alcohol_sensor.h   - MQ-3 ADC driver
    engine_lock.h      - motor/H-bridge "engine" actuator driver
    buzzer_led.h        - buzzer + status LED driver
    lcd_i2c.h            - 16x2 LCD over I2C (PCF8574 backpack)
    gsm_gps.h            - SIM800C AT-command driver (SMS + GNSS)
    tilt_sensor.h        - tilt/rollover switch driver (debounced)
  Src/
    alcohol_sensor.c
    engine_lock.c
    buzzer_led.c
    lcd_i2c.c
    gsm_gps.c
    tilt_sensor.c
    main.c              - application logic, wires modules together
```

## Assumed peripheral configuration (set these up in CubeMX)

| Peripheral | Purpose                                          |
|-----------|----------------------------------------------------|
| ADC1      | MQ-3 analog output (single channel)                 |
| TIM2 CH1  | PWM to the motor driver's ENABLE pin                 |
| GPIO      | Motor IN1/IN2, buzzer, LED, reset button, tilt switch |
| I2C1      | LCD1602 I2C backpack (PCF8574, addr 0x27)             |
| USART1    | SIM800C GSM/GPS module, 9600 baud, 8N1                |

Regenerate `SystemClock_Config()` and the `MX_*_Init()` functions in
`main.c` from your own `.ioc` file for your specific STM32 part/board —
the placeholders here only note *what* each peripheral needs to do,
since the exact register-level config depends on your MCU variant,
crystal frequency, and pin mapping.

## Things to calibrate / fill in before flashing

1. **`ALCOHOL_THRESHOLD`** in `alcohol_sensor.h` — the MQ-3 needs
   on-bench calibration against your supply voltage and ADC resolution.
2. **`REGISTERED_NUMBER`** in `main.c` — destination phone number for
   SMS alerts.
3. **`LCD_I2C_ADDR`** in `lcd_i2c.h` — confirm your backpack's I2C
   address with an I2C scanner (0x27 and 0x3F are the two common
   defaults).
4. **Pin assignments** throughout `main.c`'s `MX_GPIO_Init()` — match
   these to your actual wiring/PCB, including `TILT_SENSOR_PORT` /
   `TILT_SENSOR_PIN`.
5. **`hTilt.active_low`** in `main()` — set to match your tilt module:
   most ball-tilt switches (e.g. SW-520D) pull the input LOW when
   tilted if wired with a pull-up, but confirm with a multimeter.
6. **`TILT_DEBOUNCE_SAMPLES`** in `tilt_sensor.h` — raise this if
   ordinary bumps/potholes are triggering false tilt alerts; lower it
   for a faster response.
7. **SIM800C power supply** — needs a separate 3.7–4.4V source capable
   of ~2A transient current; do not power it from the MCU board's
   regulator. Use a logic-level shifter/voltage divider on the UART
   RX line into the SIM800C if your MCU is 5V logic (not needed on
   3.3V STM32 parts talking to a 3.3V-side buffer).

## Notes on the tilt sensor

`TiltSensor_Poll()` is called on the same periodic cadence as the
alcohol sensor read (every `SAMPLE_INTERVAL_MS`) and requires
`TILT_DEBOUNCE_SAMPLES` consecutive tilted readings before it reports
a detection — this rejects momentary jolts from potholes or hard
gear changes. A mercury tilt switch or a ball-and-ring switch
(SW-520D/SW-200D) both work as drop-in replacements; only the
`active_low` polarity needs adjusting.

## Notes on the GSM/GPS driver

`GSM_GetLocation()` sends `AT+CGNSINF` and does light in-place parsing
of the response into a Google Maps link. Not all SIM800C
firmware/hardware revisions expose GNSS this way — some setups pair a
separate discrete GPS module (e.g. NEO-6M) on its own UART instead. If
that's your hardware, swap `GSM_GetLocation()` for a small NMEA
`$GPGGA`/`$GPRMC` parser reading from that second UART, and keep
`GSM_SendSMS()` as-is for the SIM800C leg.
