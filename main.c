/******************************************************************************
 *
 * main.c
 *
 * Copyright (c) 2014 Jakob Klepp
 *
 ******************************************************************************
 *
 *                  Based on "master_slave_loopback.c":
 *
 * Copyright (c) 2010-2014 Texas Instruments Incorporated.  All rights reserved.
 * Software License Agreement
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This is part of revision 2.1.0.12573 of the Tiva Firmware Development
 * Package.
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdint.h>

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

#include "./adxl345.h"


#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

/******************************************************************************
 *
 *! \addtogroup i2c_examples_list
 *! <h1>I2C Master Loopback (i2c_master_slave_loopback)</h1>
 *!
 *! This example shows how to configure the I2C0 module for loopback mode.
 *! This includes setting up the master and slave module.  Loopback mode
 *! internally connects the master and slave data and clock lines together.
 *! The address of the slave module is set in order to read data from the
 *! master.  Then the data is checked to make sure the received data matches
 *! the data that was transmitted.  This example uses a polling method for
 *! sending and receiving data.
 *!
 *! This example uses the following peripherals and I/O signals.  You must
 *! review these and change as needed for your own board:
 *! - I2C0 peripheral
 *! - GPIO Port B peripheral (for I2C0 pins)
 *! - I2C0SCL - PB2
 *! - I2C0SDA - PB3
 *!
 *! The following UART signals are configured only for displaying console
 *! messages for this example.  These are not required for operation of I2C.
 *! - UART0 peripheral
 *! - GPIO Port A peripheral (for UART0 pins)
 *! - UART0RX - PA0
 *! - UART0TX - PA1
 *!
 *! This example uses the following interrupt handlers.  To use this example
 *! in your own application you must add these interrupt handlers to your
 *! vector table.
 *! - None.
 *
 *****************************************************************************/

void InitLED(void) {
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                     SYSCTL_OSC_MAIN);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);
}

/******************************************************************************
 *
 * This function sets up UART0 to be used for a console to display information
 * as the example is running.
 *
 *****************************************************************************/
void InitConsole(void) {
  /*
   * Enable GPIO port A which is used for UART0 pins.
   * TODO(TI): change this to whichever GPIO port you are using.
   */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  /*
   * Configure the pin muxing for UART0 functions on port A0 and A1.
   * This step is not necessary if your part does not support pin muxing.
   * TODO(TI): change this to select the port/pin you are using.
   */
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);

  /* Enable UART0 so that we can configure the clock. */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

  /* Use the internal 16MHz oscillator as the UART clock source. */
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

  /*
   * Select the alternate (UART) function for these pins.
   * TODO(TI): change this to select the port/pin you are using.
   */
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  /* Initialize the UART for console I/O. */
  UARTStdioConfig(0, 115200, 16000000);
}

void loop(void) {
  int32_t x, y, z;

  x = ADXL_getAcceleration_X();
  y = ADXL_getAcceleration_Y();
  z = ADXL_getAcceleration_Z();

  UARTprintf("  X=%i, Y=%i, Z=%i         \r", x, y, z);
}

/******************************************************************************
 *
 * Configure the I2C0 master and slave and connect them using loopback mode.
 *
 *****************************************************************************/
int main(void) {
  /*
   * Set the clocking to run directly from the external crystal/oscillator.
   * TODO(TI): The SYSCTL_XTAL_ value must be changed to match the value of the
   * crystal on your board.
   */
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_16MHZ);

  /* Set up I2C for communiction with the accelerometer. */
  InitI2C();

  /*
   * Set up the serial console to use for displaying messages.  This is
   * just for this example program and is not needed for I2C operation.
   */
  InitConsole();

  /* Display the example setup on the console. */
  while (true) {
    SysCtlDelay(SysCtlClockGet() / 4);
    loop();
  }

  /* Return no errors */
  return (0);
}
