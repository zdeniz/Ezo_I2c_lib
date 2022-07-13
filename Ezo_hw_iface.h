#ifndef __EZO_HW_IFACE_H_
#define __EZO_HW_IFACE_H_

#include "i2c.h"
#include "main.h"
#include "stdint.h"

uint8_t i2c_transmit_data(uint8_t dev_address, const uint8_t *data);
uint8_t i2c_recieve_data(uint8_t dev_address, const uint8_t *data, uint16_t size);

#endif /* __EZO_HW_IFACE_H_ */