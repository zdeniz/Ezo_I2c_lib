
#include "Ezo_i2c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ezo_hw_iface.h"

void Ezo_board_init(Ezo_i2c* device, uint8_t address, const char* name) {
    device->i2c_address = address;
    device->name = name;
    device->bufferlen = 40;
}

const char* Ezo_board_get_name(Ezo_i2c* device) {
    return device->name;
}

uint8_t Ezo_board_get_address(Ezo_i2c* device) {
    return device->i2c_address;
}

void Ezo_board_set_address(Ezo_i2c* device, uint8_t address) {
    device->i2c_address = address;
}

void Ezo_board_send_cmd(Ezo_i2c* device, const char* command) {
    i2c_transmit_data(device->i2c_address, (const uint8_t*)command);
    device->issued_read = false;
}

void Ezo_board_send_read_cmd(Ezo_i2c* device) {
    Ezo_board_send_cmd(device, "r");
    device->issued_read = true;
}

void Ezo_board_send_cmd_with_num(Ezo_i2c* device, const char* cmd, float num, uint8_t decimal_amount) {
    char buf[30];
    sprintf(buf, "%s %.*f", cmd, decimal_amount, num);

    Ezo_board_send_cmd(device, (const char*)buf);
}

void Ezo_board_send_read_with_temp_comp(Ezo_i2c* device, float temperature) {
    Ezo_board_send_cmd_with_num(device, "rt,", temperature, 3);
    device->issued_read = true;
}

ezo_errors Ezo_board_receive_read_cmd(Ezo_i2c* device) {
    char _sensordata[device->bufferlen];
    device->error = Ezo_board_receive_cmd(device, _sensordata, device->bufferlen);

    if (device->error == EZO_SUCCESS) {
        if (device->issued_read == false) {
            device->error = EZO_NOT_READ_CMD;
        } else {
            device->reading = atof(_sensordata);
        }
    }
    return device->error;
}

bool Ezo_board_is_read_poll(Ezo_i2c* device) {
    return device->issued_read;
}

float Ezo_board_get_last_received_reading(Ezo_i2c* device) {
    return device->reading;
}

ezo_errors Ezo_board_get_error(Ezo_i2c* device) {
    return device->error;
}

ezo_errors Ezo_board_receive_cmd(Ezo_i2c* device, char* sensordata_buffer, uint16_t buffer_len) {
    uint8_t code = 255;
    uint8_t data[buffer_len];
    memset(data, 0, buffer_len);  // clear sensordata array;

    if (i2c_recieve_data(device->i2c_address, data, buffer_len) == 0) {  // Readşing OK
        code = data[0];
        strcpy(sensordata_buffer, (const char*)&data[1]);
    }

    // should last array point be set to 0 to stop string overflows?
    switch (code) {
        case 1:
            device->error = EZO_SUCCESS;
            break;

        case 2:
            device->error = EZO_FAIL;
            break;

        case 254:
            device->error = EZO_NOT_READY;
            break;

        case 255:
            device->error = EZO_NO_DATA;
            break;
    }
    return device->error;
}