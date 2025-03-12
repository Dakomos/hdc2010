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
 
 /**
  * @brief  Initializes the HDC2010 sensor.
  * 
  * Diese Funktion initialisiert den HDC2010-Sensor und überprüft die Geräte- und Hersteller-IDs.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @param  i2cHandle   Pointer auf die I2C-Handle-Struktur.
  * @retval uint8_t     Anzahl der Fehler während der Initialisierung. Wenn > 0, dann ist die Initialisierung fehlgeschlagen.
  */
 uint8_t HDC2010_Init(HDC2010 *dev, I2C_HandleTypeDef *i2cHandle)
 {
     dev->i2cHandle = i2cHandle;
     dev->temp_C = 0.0f;
     dev->humid = 0.0f;
 
     uint8_t errNum = 0;
     HAL_StatusTypeDef status;
     uint8_t regData[2];
 
     /* Check device and part IDs ( Datasheet page 17 ) */
 
     /* Check device ID */
     status = HDC2010_ReadRegister(dev, HDC2010_DEV_ID_HIGH_REG, &regData[0]);
     status = HDC2010_ReadRegister(dev, HDC2010_DEV_ID_LOW_REG, &regData[1]);
     errNum += (status != HAL_OK);
 
     if (((regData[0] << 8) | regData[1]) != HDC2010_DEV_ID)
     {
         return 255;
     }
 
     /* Check manufacturer ID */
     status = HDC2010_ReadRegister(dev, HDC2010_MANU_ID_HIGH_REG, &regData[1]); // switch because our registers seem to be switched
     status = HDC2010_ReadRegister(dev, HDC2010_MANU_ID_LOW_REG, &regData[0]);
     errNum += (status != HAL_OK);
 
     if (((regData[0] << 8) | regData[1]) != HDC2010_MANU_ID)
     {
         return 255;
     }
 
     /* Start measurement ( Data sheet page 26 ) */
 
     return errNum;
 }
 
 /**
  * @brief  Starts a measurement in one-shot mode.
  * 
  * Diese Funktion startet eine Messung im One-Shot-Modus. Die Messung wird in den Registern des Geräts gespeichert.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_StartMeasurement(HDC2010 *dev)
 {
     uint8_t regData[2];
     regData[0] = 0x01;
 
     uint8_t errNum = 0;
     HAL_StatusTypeDef status;
 
     status = HDC2010_WriteRegister(dev, HDC2010_MEASURE_CONF_REG, &regData[0]);
     errNum += (status != HAL_OK);
 
     return errNum;
 }
 
 /**
  * @brief  Reads the temperature from the HDC2010 sensor.
  * 
  * Diese Funktion liest die Temperatur vom HDC2010-Sensor und speichert sie in der Struktur.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_ReadTemp(HDC2010 *dev)
 {
     uint8_t temp[2];
     HAL_StatusTypeDef status;
 
     HDC2010_StartMeasurement(dev);
 
     status = HDC2010_ReadRegister(dev, HDC2010_TEMP_LOW_REG, &temp[1]);
     if (status != HAL_OK)
         return status;
 
     status = HDC2010_ReadRegister(dev, HDC2010_TEMP_HIGH_REG, &temp[0]);
     if (status != HAL_OK)
         return status;
 
     dev->temp_C = ((((temp[0] << 8) | temp[1]) * 165.0) / 65536) - 40;
 
     return status;
 }
 
 /**
  * @brief  Reads the humidity from the HDC2010 sensor.
  * 
  * Diese Funktion liest die Luftfeuchtigkeit vom HDC2010-Sensor und speichert sie in der Struktur.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_ReadHum(HDC2010 *dev)
 {
     uint8_t humid[2];
     HAL_StatusTypeDef status;
 
     HDC2010_StartMeasurement(dev);
 
     status = HDC2010_ReadRegister(dev, HDC2010_HUM_LOW_REG, &humid[1]);
     if (status != HAL_OK)
         return status;
 
     status = HDC2010_ReadRegister(dev, HDC2010_HUM_HIGH_REG, &humid[0]);
     if (status != HAL_OK)
         return status;
 
     dev->humid = ((((humid[0] << 8) | humid[1]) * 100.0) / 65536);
 
     return status;
 }
 
 /**
  * @brief  Reads a register from the HDC2010 sensor.
  * 
  * Diese Funktion liest ein Register vom HDC2010-Sensor.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @param  reg         Registeradresse, die gelesen werden soll.
  * @param  data        Pointer auf den Speicher, in dem die gelesenen Daten gespeichert werden.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_ReadRegister(HDC2010 *dev, uint8_t reg, uint8_t *data)
 {
     return HAL_I2C_Mem_Read(dev->i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
 }
 
 /**
  * @brief  Reads multiple registers from the HDC2010 sensor.
  * 
  * Diese Funktion liest mehrere Register vom HDC2010-Sensor.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @param  reg         Startadresse des ersten Registers, das gelesen werden soll.
  * @param  data        Pointer auf den Speicher, in dem die gelesenen Daten gespeichert werden.
  * @param  length      Anzahl der zu lesenden Register.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_ReadRegisters(HDC2010 *dev, uint8_t reg, uint8_t *data, uint8_t length)
 {
     return HAL_I2C_Mem_Read(dev->i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
 }
 
 /**
  * @brief  Writes a register to the HDC2010 sensor.
  * 
  * Diese Funktion schreibt ein Register des HDC2010-Sensors.
  * 
  * @param  dev         Pointer auf die HDC2010-Struktur.
  * @param  reg         Registeradresse, die geschrieben werden soll.
  * @param  data        Pointer auf den Speicher, der die zu schreibenden Daten enthält.
  * @retval HAL_StatusTypeDef  Status der HAL-Funktion. HAL_OK bei Erfolg, sonst Fehlercode.
  */
 HAL_StatusTypeDef HDC2010_WriteRegister(HDC2010 *dev, uint8_t reg, uint8_t *data)
 {
     return HAL_I2C_Mem_Write(dev->i2cHandle, HDC2010_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
 }
 