/*
*
*   HDC2010 Humidity and Temperatur I2C Driver
*   
*   Author: Leonard Schmitt
*   Created: 3 Dezember 2024
*
*
*/


#include "hdc2010.h"
#include "stm32l4xx_hal.h"

uint8_t HDC2010_Init ( HDC2010 *dev, I2C_HandleTypeDef *i2cHandle ) {

    dev->i2cHandle  = i2cHandle;

    dev->temp_C     = 0.0f;

    dev->humid      = 0.0f;

    /* Store number of errors. If this initialisation returns > 0, then it's not initialised correctly */
    uint8_t errNum  = 0;
    HAL_StatusTypeDef status;

    uint8_t regData;
    /* Check device and part IDs ( Datasheet page 17 ) */
    
    /* Check device ID */
    status = HDC2010_ReadRegister ( dev, HDC2010_DEV_ID_LOW_REG, &regData );
    errNum += ( status != HAL_OK );

    if ( regData != HDC2010_DEV_ID_LOW_REG ) {
        
        return 255;

    }

    /* Check manufacturer ID */
    status = HDC2010_ReadRegister ( dev, HDC2010_MANU_ID_LOW_REG, &regData );
    errNum += ( status != HAL_OK );

    if ( regData != HDC2010_MANU_ID_LOW_REG ) {
        
        return 255;

    }

    /* Start measurement ( Data sheet page 26 )*/
    regData = 0x01

    status = HDC2010_WriteRegister ( dev, HDC2010_MEASURE_CONF_REG, &regData );
    errNum += ( status != HAL_OK );

    return errNum;

}

/*
*   Data
*/

HAL_StatusTypeDef HDC2010_ReadTemp ( HDC2010 *dev ){

    uint8_t temp[2];

    HDC2010_ReadRegister(dev, HDC2010_TEMP_LOW_REG, temp[0])
    HDC2010_ReadRegister(dev, HDC2010_TEMP_HIGH_REG, temp[1])

    dev->temp_C = ((((temp[0] << 8) | temp[1]) *165)/65536) - 40;

}

HAL_StatusTypeDef HDC2010_ReadHum ( HDC2010 *dev ){

    uint8_t humid[2];

    HDC2010_ReadRegister(dev, HDC2010_HUM_LOW_REG, humid[0])
    HDC2010_ReadRegister(dev, HDC2010_HUM_HIGH_REG, humid[1])

    dev->humid = ((((humid[0] << 8) | humid[1]) *100)/65536); 


}

/*
*   Low Level
*/

HAL_StatusTypeDef HDC2010_ReadRegister ( HDC2010 *dev, uint8_t reg, uin8_t *data ){
    
    return HAL_I2C_Mem_Read( (*dev).i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );

}

HAL_StatusTypeDef HDC2010_ReadRegisters ( HDC2010 *dev, uint8_t reg, uin8_t *data, uint8_t length ){

    return HAL_I2C_Mem_Read( (*dev).i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY );

}

HAL_StatusTypeDef HDC2010_WriteRegister ( HDC2010 *dev, uint8_t reg, uin8_t *data ){

    return HAL_I2C_Mem_Write( dev->i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY );

}
