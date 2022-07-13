#include "Ezo_hw_iface.h"

uint8_t i2c_transmit_data(uint8_t dev_address, const uint8_t *data) {
    uint8_t W_Addr = dev_address << 1; /* I2C Write Address */
    return HAL_I2C_Master_Transmit(&hi2c1, W_Addr, (uint8_t *)data, sizeof(data) / sizeof(uint8_t), 100);
}

uint8_t i2c_recieve_data(uint8_t dev_address, const uint8_t *data, uint16_t size) {
    uint8_t R_Addr = (dev_address << 1) + 1; /* I2C Read Address */
    return HAL_I2C_Master_Receive(&hi2c1, R_Addr, (uint8_t *)data, size, 100);
}
