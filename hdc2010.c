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

void hdc2010_read_temp(I2C_HandleTypeDef *handle){
    uint16_t temp = 0;
    temp = temp/2^16*165-40 // for high
    temp = temp/2^8*165-40 // for low

}

void hdc2010_read_hum(I2C_HandleTypeDef *handle){
    uint16_t hum = 0;
    hum = hum/2^16*100 // for high
    hum = hum/2^8*100 // for low
}