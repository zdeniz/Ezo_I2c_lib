
#include "Ezo_i2c.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ezo_hw_iface.h"
#include "stdbool.h"

enum ezo_errors { SUCCESS,
                  FAIL,
                  NOT_READY,
                  NO_DATA,
                  NOT_READ_CMD };

typedef struct {
    uint8_t i2c_address;
    const char* name;
    float reading;
    bool issued_read;
    enum ezo_errors error;
    const uint8_t bufferlen;
} Ezo_i2c;

const char* Ezo_board_get_name(Ezo_i2c* ezo) {
    return ezo->name;
}

uint8_t Ezo_board_get_address(Ezo_i2c* ezo) {
    return ezo->i2c_address;
}

void Ezo_board_set_address(Ezo_i2c* ezo, uint8_t address) {
    ezo->i2c_address = address;
}

void Ezo_board_send_cmd(Ezo_i2c* ezo, const char* command) {
    i2c_transmit_data(ezo->i2c_address, (const uint8_t*)command);
    ezo->issued_read = false;
}

void Ezo_board_send_read_cmd(Ezo_i2c* ezo) {
    Ezo_board_send_cmd(ezo, "r");
    ezo->issued_read = true;
}

void Ezo_board_send_cmd_with_num(Ezo_i2c* ezo, const char* cmd, float num, uint8_t decimal_amount) {
    char buf[30];
    sprintf(buf, "%s %.*f", cmd, decimal_amount, num);

    Ezo_board_send_cmd(ezo, (const char*)buf);
}

void Ezo_board_send_read_with_temp_comp(Ezo_i2c* ezo, float temperature) {
    Ezo_board_send_cmd_with_num(ezo, "rt,", temperature, 3);
    ezo->issued_read = true;
}

enum ezo_errors Ezo_board_receive_read_cmd(Ezo_i2c* ezo) {
    char _sensordata[ezo->bufferlen];
    ezo->error = receive_cmd(_sensordata, bufferlen);

    if (ezo->error == SUCCESS) {
        if (ezo->issued_read == false) {
            ezo->error = NOT_READ_CMD;
        } else {
            ezo->reading = atof(_sensordata);
        }
    }
    return ezo->error;
}

bool Ezo_board_is_read_poll(Ezo_i2c* ezo) {
    return ezo->issued_read;
}

float Ezo_board_get_last_received_reading(Ezo_i2c* ezo) {
    return ezo->reading;
}

enum ezo_errors Ezo_board_get_error(Ezo_i2c* ezo) {
    return ezo->error;
}

enum ezo_errors Ezo_board_receive_cmd(Ezo_i2c* ezo, char* sensordata_buffer, uint16_t buffer_len) {
    uint8_t code = 255;
    uint8_t data[buffer_len];
    // uint8_t in_char = 0;

    memset(data, 0, buffer_len);  // clear sensordata array;

    i2c_recieve_data(ezo->i2c_address, data, buffer_len);
    strcpy(sensordata_buffer, (const char *)data);

    code = sensordata_buffer[0];
    // Wire.requestFrom(ezo->i2c_address, (uint8_t)(buffer_len-1), (uint8_t)1);
    // code = Wire.read();

    // Wire.beginTransmission(ezo->i2c_address);
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
            ezo->error = SUCCESS;
            break;

        case 2:
            ezo->error = FAIL;
            break;

        case 254:
            ezo->error = NOT_READY;
            break;

        case 255:
            ezo->error = NO_DATA;
            break;
    }
    return ezo->error;
}