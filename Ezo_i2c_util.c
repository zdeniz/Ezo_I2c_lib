
#include "Ezo_i2c_util.h"
#include "serial_port.h"
#include <stdio.h>

// prints the boards name and I2C address
void print_device_info(Ezo_board &Device) {
  Serial_Write("%s %s", Device.get_name(), Device.get_address());
}

// used for printing either a success_string message if a command was successful or the error type if it wasnt
void print_success_or_error(Ezo_board &Device, const char* success_string) {
  switch (Device.get_error()) {             //switch case based on what the response code is.
    case Ezo_board::SUCCESS:
      Serial_Write("%s", success_string);   //the command was successful, print the success string
      break;

    case Ezo_board::FAIL:
      Serial_Write("Failed ");        //means the command has failed.
      break;

    case Ezo_board::NOT_READY:
      Serial_Write("Pending ");       //the command has not yet been finished calculating.
      break;

    case Ezo_board::NO_DATA:
      Serial_Write("No Data ");       //the sensor has no data to send.
      break;

    case Ezo_board::NOT_READ_CMD:
      Serial_Write("No Command ");       //the sensor has no data to send.
      break;
  }
}

void receive_and_print_response(Ezo_board &Device) {
  char receive_buffer[32];                  //buffer used to hold each boards response
  Device.receive_cmd(receive_buffer, 32);   //put the response into the buffer

  print_success_or_error(Device, " - ");          //print if our response is an error or not
  print_device_info(Device);                //print our boards name and address
  Serial_Write(": %s", receive_buffer);
}

void receive_and_print_reading(Ezo_board &Device) {              // function to decode the reading after the read command was issued
  Serial_Write("%s: ", Device.get_name());      // print the name of the circuit getting the reading
  Device.receive_read_cmd();              //get the response data and put it into the [Device].reading variable if successful
  
  char s[15];
  sprintf(s, "%.*f", 2, Device.get_last_received_reading());
  print_success_or_error(Device, s);  //print either the reading or an error message
}
