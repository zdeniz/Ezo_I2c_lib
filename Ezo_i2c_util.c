
#include "Ezo_i2c_util.h"

#include <stdio.h>

#include "serial_port.h"

char s_buffer[16] = {0};
char receive_buffer[32] = {0};  // buffer used to hold each boards response

// prints the boards name and I2C address
void print_device_info(Ezo_i2c* device) {
    Serial_Write("Board: %s, \tAddress: %d\r\n", Ezo_board_get_name(device), Ezo_board_get_address(device));
}

// used for printing either a successmak_string message if a command was successful or the error type if it wasnt
void print_success_or_error(Ezo_i2c* device, const char* success_string) {
    switch (Ezo_board_get_error(device)) {  // switch case based on what the response code is.
        case EZO_SUCCESS:
            Serial_Write(success_string);  // the command was successful, print the success string
            break;

        case EZO_FAIL:
            Serial_Write("Failed ");  // means the command has failed.
            break;

        case EZO_NOT_READY:
            Serial_Write("Pending ");  // the command has not yet been finished calculating.
            break;

        case EZO_NO_DATA:
            Serial_Write("No Data ");  // the sensor has no data to send.
            break;

        case EZO_NOT_READ_CMD:
            Serial_Write("No Command ");  // the sensor has no data to send.
            break;
    }
}

void receive_and_print_response(Ezo_i2c* device) {
    Ezo_board_receive_cmd(device, receive_buffer, device->bufferlen);  // put the response into the buffer

    print_success_or_error(device, " - ");  // print if our response is an error or not
    print_device_info(device);              // print our boards name and address
    Serial_Write(": %s", receive_buffer);
}

void receive_and_print_reading(Ezo_i2c* device) {      // function to decode the reading after the read command was issued
    Serial_Write("%s: ", Ezo_board_get_name(device));  // print the name of the circuit getting the reading
    Ezo_board_receive_read_cmd(device);                // get the response data and put it into the [Device].reading variable if successful

    sprintf(s_buffer, "%.*f", 3, Ezo_board_get_last_received_reading(device));
    print_success_or_error(device, (const char*)s_buffer);  // print either the reading or an error message
}
