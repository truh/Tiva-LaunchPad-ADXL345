/******************************************************************************
 *
 * adxl345.c
 *
 * Copyright (c) 2014 Jakob Klepp
 *
 *****************************************************************************/

#include "./adxl345.h"

/******************************************************************************
 *
 * Read a byte of data from given device address and register
 *
 *****************************************************************************/
int8_t ReadI2C(uint16_t device_address, uint16_t device_register) {
  int8_t byteIn;

  /* Tell the master to read data. */
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

  /* Wait until the slave is done sending data. */
  while (!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_TREQ)) {
  }

  /* Read the data from the master. */
  byteIn = I2CMasterDataGet(I2C0_BASE);

  return byteIn;
}

/*
 * Write a byte of data to given device address and register
 */
void WriteI2C(uint16_t device_address, uint16_t device_register,
               int8_t device_data) {
  /* Place the data to be sent in the data register */
  I2CMasterDataPut(I2C0_BASE, device_data);

  /*
   * Initiate send of data from the master.  Since the loopback
   * mode is enabled, the master and slave units are connected
   * allowing us to receive the same data that we sent out.
   */
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
}

/******************************************************************************
 *
 * This function sets up I2C0 to be used for communicating with the
 * accelerometer.
 *
 *****************************************************************************/
void InitI2C(void) {
  /* The I2C0 peripheral must be enabled before use. */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

  /*
   * For this example I2C0 is used with PortB[3:2].  The actual port and
   * pins used may be different on your part, consult the data sheet for
   * more information.  GPIO port B needs to be enabled so these pins can
   * be used.
   * TODO(TI): change this to whichever GPIO port you are using.
   */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  /*
   * Configure the pin muxing for I2C0 functions on port B2 and B3.
   * This step is not necessary if your part does not support pin muxing.
   * TODO(TI): change this to select the port/pin you are using.
   */
  GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);

  /*
   * Select the I2C function for these pins.  This function will also
   * configure the GPIO pins pins for I2C operation, setting them to
   * open-drain operation with weak pull-ups.  Consult the data sheet
   * to see which functions are allocated per pin.
   * TODO(TI): change this to select the port/pin you are using.
   */
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

  /*
   * Enable and initialize the I2C0 master module.  Use the system clock for
   * the I2C0 module.  The last parameter sets the I2C data transfer rate.
   * If false the data rate is set to 100kbps and if true the data rate will
   * be set to 400kbps.  For this example we will use a data rate of 100kbps.
   */
  I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

  /*
   * Tell the master module what address it will place on the bus when
   * communicating with the slave.  Set the address to SLAVE_ADDRESS
   * (as set in the slave module).  The receive parameter is set to false
   * which indicates the I2C Master is initiating a writes to the slave.  If
   * true, that would indicate that the I2C Master is initiating reads from
   * the slave.
   */
  I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);

  /*
   * Enable loopback mode.  Loopback mode is a built in feature that is
   * useful for debugging I2C operations.  It internally connects the I2C
   * master and slave terminals, which effectively let's you send data as
   * a master and receive data as a slave.
   * NOTE: For external I2C operation you will need to use external pullups
   * that are stronger than the internal pullups.  Refer to the datasheet for
   * more information.
   */
  HWREG(I2C0_BASE + I2C_O_MCR) |= 0x01;
}

/******************************************************************************
 *
 * Return the id of the accelerometer.
 *
 *****************************************************************************/
int8_t ADXL_getAccelerometer_ID(void) {
  return ADXL_ALT_ADDRESS_HIGH;
}

/******************************************************************************
 *
 * Set the power mode of the accelerometer.
 *
 *****************************************************************************/
void ADXL_SetPowerMode(uint8_t powerMode) {
  /* Set the Slave Address and write */
  I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, WRITE);

  /* Send the Register */
  I2CMasterDataPut(I2C0_BASE, ADXL_POWER_MODE_REGISTER);

  /* Sending */
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

  /* Waiting until the Master is done */
  while (I2CMasterBusy(I2C0_BASE)) {}

  /* Get error code, if there is one */
  int8_t error = I2CMasterErr(I2C0_BASE);

  if (error != I2C_MASTER_ERR_NONE) {
    UARTprintf("\n error-code : %i \n", error);
  }
}

/******************************************************************************
 *
 * private
 *
 * Converts raw sensor values to decimal value.
 *
 *****************************************************************************/
int32_t __convertRawToDecimal(const int16_t rawValue) {
  int32_t value;

  /* TODO(jakob): find out the right algorithm */
  value = rawValue << 2;

  return value;
}

/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawX(void) {

}

/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawY(void) {

}

/******************************************************************************
 *
 * Reads the raw acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int16_t ADXL_getAcceleration_rawZ(void) {

}

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_X(void) {

}

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_Y(void) {

}

/******************************************************************************
 *
 * Reads the decoded acceleration  of the x axisfrom the adxl345
 *
 *****************************************************************************/
int32_t ADXL_getAcceleration_Z(void) {

}
