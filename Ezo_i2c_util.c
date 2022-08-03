
#include "Ezo_i2c_util.h"

#include <stdio.h>
#include <string.h>

#include "serial_port.h"

char s_buffer[16] = {0};
char receive_buffer[32] = {0};  // buffer used to hold each boards response
char answer_buffer[50] = {0};

// prints the boards name and I2C address
void print_device_info(Ezo_i2c* device) {
    Serial_Write("Board: %s, Address: %d\r\n", Ezo_board_get_name(device), Ezo_board_get_address(device));
}

// used for printing either a successmak_string message if a command was successful or the error type if it wasnt
void print_success_or_error(Ezo_i2c* device, const char* success_string) {
    strcpy(answer_buffer, Ezo_board_get_name(device));
    switch (Ezo_board_get_error(device)) {  // switch case based on what the response code is.
        case EZO_SUCCESS:
            strcat(answer_buffer, (char*)success_string);  // the command was successful, print the success string
            break;
        case EZO_FAIL:
            strcat(answer_buffer, "Failed ");  // means the command has failed.
            break;
        case EZO_NOT_READY:
            strcat(answer_buffer, "Pending ");  // the command has not yet been finished calculating.
            break;
        case EZO_NO_DATA:
            strcat(answer_buffer, "No Data ");  // the sensor has no data to send.
            break;
        case EZO_NOT_READ_CMD:
            strcat(answer_buffer, "No Command ");  // the sensor has no data to send.
            break;
    }
    Serial_Write(answer_buffer);
    strcat(answer_buffer, ",");
}

void receive_and_print_response(Ezo_i2c* device) {
    Ezo_board_receive_cmd(device, receive_buffer, device->bufferlen);  // put the response into the buffer
    print_success_or_error(device, " - ");                             // print if our response is an error or not
    print_device_info(device);                                         // print our boards name and address
    Serial_Write(":%s", receive_buffer);
}

void receive_and_print_reading(Ezo_i2c* device) {     // function to decode the reading after the read command was issued
    Serial_Write("%s:", Ezo_board_get_name(device));  // print the name of the circuit getting the reading
    Ezo_board_receive_read_cmd(device);               // get the response data and put it into the [Device].reading variable if successful
    sprintf(s_buffer, "%.*f", 3, Ezo_board_get_last_received_reading(device));
    print_success_or_error(device, (const char*)s_buffer);  // print either the reading or an error message
}
