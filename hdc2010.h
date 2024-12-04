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


/*
*   REGISTERS
*/

#define HDC2010_TEMP_LOW_REG            0x00 // Data sheet page 18
#define HDC2010_TEMP_HIGH_REG           0x01 // Temperatur is a 14 bit integer stored in two registers. The two LSB are always 0
#define HDC2010_HUM_LOW_REG             0x02 // Data sheet page 18
#define HDC2010_HUM_HIGH_REG            0x03 // Humidity is a 14 bit integer stored in two registers. The two LSB are always 0
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
#define HDC2010_RESET_CONF_REG          0x0E // Data sheet page 25. Settings for auto measurement modes
#define HDC2010_MEASURE_CONF_REG        0x0F // Data sheet page 26. Setting for standby or measurement. Setting for precision of temperatur and humidty integer
#define HDC2010_MANU_ID_LOW_REG         0xFC
#define HDC2010_MANU_ID_HIGH_REG        0xFD
#define HDC2010_DEV_ID_LOW_REG          0xFE
#define HDC2010_DEV_ID_HIGH_REG         0xFF

/*
*   SENSOR STRUCT
*/

typedef struct {
    /* I2C handle */
    I2C_HandleTypeDef *i2cHandle;

    /* Temperatur Data */
    float temp_C;

    /* Humidity Data */
    float humid;
} HDC2010;

/*
*   Init
*/

uint8_t HDC2010_Init ( HDC2010 *dev, I2C_HandleTypeDef *i2cHandle );

/*
*   Data
*/

HAL_StatusTypeDef HDC2010_ReadTemp ( HDC2010 *dev );
HAL_StatusTypeDef HDC2010_ReadHum ( HDC2010 *dev );

/*
*   Low Level
*/

HAL_StatusTypeDef HDC2010_ReadRegister ( HDC2010 *dev, uint8_t reg, uin8_t *data );
HAL_StatusTypeDef HDC2010_ReadRegisters ( HDC2010 *dev, uint8_t reg, uin8_t *data, uint8_t length );

HAL_StatusTypeDef HDC2010_WriteRegister ( HDC2010 *dev, uint8_t reg, uin8_t *data );

int bernd = 42;

#endif