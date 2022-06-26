
#include "Ezo_i2c.h"

#include <stdlib.h>
#include <string.h>

#include "i2c.h"
#include "main.h"
#include "stdio.h"

Ezo_board::Ezo_board(uint8_t address) {
    this->i2c_address = address;
}

Ezo_board::Ezo_board(uint8_t address, const char* name) {
    this->i2c_address = address;
    this->name = name;
}

const char* Ezo_board::get_name() {
    return this->name;
}

uint8_t Ezo_board::get_address() {
    return i2c_address;
}

void Ezo_board::set_address(uint8_t address) {
    this->i2c_address = address;
}

void Ezo_board::send_cmd(const char* command) {
    HAL_I2C_Master_Transmit(&hi2c1, this->i2c_address, (uint8_t*)command, sizeof(command), 100);
    // Wire.beginTransmission(this->i2c_address);
    // Wire.write(command);
    // Wire.endTransmission();
    this->issued_read = false;
}

void Ezo_board::send_read_cmd() {
    send_cmd("r");
    this->issued_read = true;
}

void Ezo_board::send_cmd_with_num(const char* cmd, float num, uint8_t decimal_amount) {
    char buf[30];

    sprintf(buf, "%s %.*f", cmd, decimal_amount, num);

    const char* pointer = buf;
    send_cmd(pointer);
}

void Ezo_board::send_read_with_temp_comp(float temperature) {
    send_cmd_with_num("rt,", temperature, 3);
    this->issued_read = true;
}

enum Ezo_board::errors Ezo_board::receive_read_cmd() {
    char _sensordata[this->bufferlen];
    this->error = receive_cmd(_sensordata, bufferlen);

    if (this->error == SUCCESS) {
        if (this->issued_read == false) {
            this->error = NOT_READ_CMD;
        } else {
            this->reading = atof(_sensordata);
        }
    }
    return this->error;
}

bool Ezo_board::is_read_poll() {
    return this->issued_read;
}

float Ezo_board::get_last_received_reading() {
    return this->reading;
}

enum Ezo_board::errors Ezo_board::get_error() {
    return this->error;
}

enum Ezo_board::errors Ezo_board::receive_cmd(char* sensordata_buffer, uint8_t buffer_len) {
    uint8_t code = 0;
    // uint8_t in_char = 0;s

    memset(sensordata_buffer, 0, buffer_len);  // clear sensordata array;

    HAL_I2C_Master_Receive(&hi2c1, this->i2c_address, (uint8_t*)sensordata_buffer, (uint8_t)(buffer_len - 1), 1000);
    // Wire.requestFrom(this->i2c_address, (uint8_t)(buffer_len-1), (uint8_t)1);
    // code = Wire.read();

    // Wire.beginTransmission(this->i2c_address);
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
            this->error = SUCCESS;
            break;

        case 2:
            this->error = FAIL;
            break;

        case 254:
            this->error = NOT_READY;
            break;

        case 255:
            this->error = NO_DATA;
            break;
    }
    return this->error;
}