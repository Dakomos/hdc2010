/*
*
*   HDC2010 Humidity and Temperatur I2C Driver
*   
*   Author: Leonard Schmitt
*   Created: 3 Dezember 2024
*
*
*/

#ifndef HDC2010_I2C_DRIVER_H
#define HDC2010_I2C_DRIVER_H

#include "stm32l4xx_hal.h" // Needed for I2C
#include <stdint.h>

/*
*   DEFINES
*/

#define HDC2010_I2C_ADDR                (0x80 <<) // 1000000 in BIN, Seite 3 Datasheet

#define HDC2010_HUM_LOW_REG             0x02
#define HDC2010_TEMP_LOW_REG            0x00
#define HDC2010_DEV_ID_LOW_REG          0xFE


/*
*   REGISTERS
*/

#define HDC2010_TEMP_HIGH_REG           0x01
#define HDC2010_HUM_HIGH_REG            0x03
#define HDC2010_INTERRUPT_REG           0x04
#define HDC2010_TEMP_MAX_REG            0x05
#define HDC2010_HUM_MAX_REG             0x06
#define HDC2010_INTERRUPT_ENABLE_REG    0x07
#define HDC2010_TEMP_OFFSET_REG         0x08
#define HDC2010_HUM_OFFSET_REG          0x09
#define HDC2010_TEMP_THR_LOW_REG        0x0A
#define HDC2010_TEMP_THR_HIGH_REG       0x0B
#define HDC2010_RH_THR_LOW_REG          0x0C
#define HDC2010_RH_THR_HIGH_REG         0x0D
#define HDC2010_RESET_CONF_REG          0x0E
#define HDC2010_MEASURE_CONF_REG        0x0F
#define HDC2010_MANU_ID_LOW_REG         0xFC
#define HDC2010_MANU_ID_HIGH_REG        0xFD
#define HDC2010_DEV_ID_HIGH_REG         0xFF

#endif