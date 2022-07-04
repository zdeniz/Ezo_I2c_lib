#include "ezo_hw_iface.h"

uint8_t i2c_transmit_data(uint8_t dev_address, const uint8_t *data)
{
    return HAL_I2C_Master_Transmit(&hi2c1, dev_address, (uint8_t*)data, sizeof(data)/sizeof(uint8_t), 100);
}

uint8_t i2c_recieve_data(uint8_t dev_address, const uint8_t *data, uint16_t size)
{
    return HAL_I2C_Master_Receive(&hi2c1, dev_address, (uint8_t*)data, size, 100);
}

