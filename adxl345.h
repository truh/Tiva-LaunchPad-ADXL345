/******************************************************************************
 *
 * adxl345.h
 *
 * Copyright (c) 2014 Jakob Klepp
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "utils/uartstdio.h"

#ifndef ADXL_H
#define ADXL_H

/******************************************************************************
 *
 * The 7-bit I2C address for the device when the "alt address pin" is high.
 *
 *****************************************************************************/
#define ADXL_ALT_ADDRESS_HIGH 0x1D

/******************************************************************************
 *
 * The 7-bit I2C address for the device when the "alt address pin" is low.
 *
 *****************************************************************************/
#define ADXL_ALT_ADDRESS_LOW 0x53

/******************************************************************************
 *
 * Followed by the R/W bit
 *
 *****************************************************************************/
#define WRITE 0
#define READ 1

/******************************************************************************
 *
 * Full registermap can be seen in the Register Map example on page 23
 * of the ADXL345.pdf file.
 *
 *****************************************************************************/
#define ADXL_AXIS_X_P0 0x32
#define ADXL_AXIS_X_P1 0x33
#define ADXL_AXIS_Y_P0 0x34
#define ADXL_AXIS_Y_P1 0x35
#define ADXL_AXIS_Z_P0 0x36
#define ADXL_AXIS_Z_P1 0x37
#define ADXL_POWER_MODE_REGISTER 0x2D

#define ADXL_POWER_MODE 0x3f

/******************************************************************************
 *
 * Number of I2C data packets to send.
 *
 *****************************************************************************/
#define NUM_I2C_DATA 3

/******************************************************************************
 *
 * Set the address for slave module. This is a 7-bit address sent in the
 * following format:
 *                      [A6:A5:A4:A3:A2:A1:A0:RS]
 *
 * A zero in the "RS" position of the first byte means that the master
 * transmits (sends) data to the selected slave, and a one in this position
 * means that the master receives data from the slave.
 *
 *****************************************************************************/
#define SLAVE_ADDRESS ADXL_ALT_ADDRESS_LOW

/******************************************************************************
 *
 * Read a byte of data from given device address and register
 *
 *****************************************************************************/
int8_t ReadI2C(uint16_t device_address, uint16_t device_register);

/******************************************************************************
 *
 * Write a byte of data to given device address and register
 *
 *****************************************************************************/
void WriteI2C(uint16_t device_address, uint16_t device_register,
               int8_t device_data);

/******************************************************************************
 *
 * This function sets up I2C0 to be used for communicating with the
 * accelerometer.
 *
 *****************************************************************************/
void InitI2C(void);

/******************************************************************************
 *
 * Check if there was an I2C error.
 * Write it to UART if there was one.
 *
 *****************************************************************************/
void I2C_checkError(void);

/******************************************************************************
 *
 * Return the id of the accelerometer.
 *
 *****************************************************************************/
int8_t ADXL_getAccelerometer_ID(void);

/******************************************************************************
 *
 * Set the power mode of the accelerometer.
 *
 *****************************************************************************/
void ADXL_SetPowerMode(uint8_t powerMode);

/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawX(void);

/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawY(void);
/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawZ(void);

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_X(void);

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_Y(void);

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_Z(void);

#endif /* ADXL_H */
