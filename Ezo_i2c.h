/*
MIT License

Copyright (c) 2019 Atlas Scientific

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/

#ifndef EZO_I2C_H
#define EZO_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum { EZO_SUCCESS,
               EZO_FAIL,
               EZO_NOT_READY,
               EZO_NO_DATA,
               EZO_NOT_READ_CMD
} ezo_errors;

typedef struct __Ezo_Board_I2C{
    uint8_t i2c_address;
    const char* name;
    float reading;
    bool issued_read;
    ezo_errors error;
    uint8_t bufferlen;
} Ezo_i2c;

void Ezo_board_init(Ezo_i2c* device, uint8_t address, const char * name);
const char* Ezo_board_get_name(Ezo_i2c* device);
uint8_t Ezo_board_get_address(Ezo_i2c* device);
void Ezo_board_set_address(Ezo_i2c* device, uint8_t address);
void Ezo_board_send_cmd(Ezo_i2c* device, const char* command);
void Ezo_board_send_read_cmd(Ezo_i2c* device);
void Ezo_board_send_cmd_with_num(Ezo_i2c* device, const char* cmd, float num, uint8_t decimal_amount);
void Ezo_board_send_read_with_temp_comp(Ezo_i2c* device, float temperature);
ezo_errors Ezo_board_receive_read_cmd(Ezo_i2c* device);
bool Ezo_board_is_read_poll(Ezo_i2c* device);
float Ezo_board_get_last_received_reading(Ezo_i2c* device);
ezo_errors Ezo_board_get_error(Ezo_i2c* device);
ezo_errors Ezo_board_receive_cmd(Ezo_i2c* device, char* sensordata_buffer, uint16_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif /* EZO_I2C_H */
