
#include "Ezo_i2c.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ezo_hw_iface.h"

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
    // uint8_t in_char = 0;

    memset(data, 0, buffer_len);  // clear sensordata array;

    i2c_recieve_data(device->i2c_address, data, buffer_len);
    strcpy(sensordata_buffer, (const char*)data);

    code = sensordata_buffer[0];
    // Wire.requestFrom(device->i2c_address, (uint8_t)(buffer_len-1), (uint8_t)1);
    // code = Wire.read();

    // Wire.beginTransmission(device->i2c_address);
    //  while (Wire.available()) {
    //    in_char = Wire.read();

    //   if (in_char == 0) {
    //     //Wire.endTransmission();
    //     break;
    //   }
    //   else {
    //     sensordata_buffer[sensor_bytes_received] = in_char;
    //     sensor_bytes_received++;
    //   }
    // }

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