#############
I2C & ADXL345
#############

Erstellen Sie ein Projekt, wo der ADXL345 über den I2C-Bus die Informationen
an den uC sendet. Geben Sie diese drei Achsen auf der seriellen Schnittstelle
(UART) zur Kontrolle aus.

facts to ADX345
===============

==================== ============= ======
Fact name            Value         Source
==================== ============= ======
com.Address:         0x53          p.18
Power-Mode Register: 0x2d          p.23
Power_modi:          see config on p.25
x-Axis:              0x32, 0x33    p.23
y-Axis:              0x34, 0x35    p.23
z-Axis:              0x36, 0x37    p.23
==================== ============= ======

x-Axis, y-Axis, z-Axis: 8-bit/register, necessary to join those
two value to one 16-bit value. Use bitshifting!

Be aware, this is raw-Data => convert to floating point info
through multiplication!

Tasks
=====

1. Define *usefull* commands in a Header-File. Stick to the order defined
   by the library functions.

   .. code:: txt
   
       read Process: I2CMasterSlaveAddrSet(..., ..., false)
                     I2CMasterDataPut
                     i2CMasterControl
                     while (I2CMasterBusy(...))
                     I2CMasterSlaveAddrSet(..., ..., true)
                     I2CMasterControl(..., ...)
                     while(I2CMasterBusy(...))
                     I2CMasterDataGet(...)

   write process  is similar to read process

   => check chapter 17 in FW-Datasheet
   (TivaWare Peripherial Driver Library User Guide)
                         --     -       -    -
   SW-TM4C-DRL-UG [2]_

TivaWare .c files needed
========================

driverlib
~~~~~~~~~

* gpio.c
* i2c.c
* sysctl.c
* uart.c

utils
~~~~~

* uartstdio.c

API
===

adxl345.h
~~~~~~~~~

int8_t ReadI2C(uint16_t, uint16_t)
----------------------------------

Read a byte of data from given device address and register

Arguments:

* device_address
* device_register

Returns:

Byte read from the specified register.

void WriteI2C(uint16_t, uint16_t, int8_t)
-----------------------------------------

Write a byte of data to given device address and register

Arguments:

* device_address
* device_register
* device_data

void InitI2C(void)
------------------

This function sets up I2C0 to be used for communicating with the
accelerometer.

void I2C_checkError(void)
-------------------------

Check if there was an I2C error. Write it to UART if there was one.

void ADXL_SetPowerMode(uint8_t)
-------------------------------

Set the power mode of the accelerometer.

Arguments:

* powerMode

int16_t ADXL_getAcceleration_rawX(void)
---------------------------------------

Reads the raw acceleration  of the x axis from the adxl345.

Returns:

"Raw" Acceleration data.

int16_t ADXL_getAcceleration_rawY(void)
---------------------------------------

Reads the raw acceleration of the y axis from the adxl345.

Returns:

"Raw" Acceleration data.

int16_t ADXL_getAcceleration_rawZ(void)
---------------------------------------

Reads the raw acceleration of the z axis from the adxl345.

Returns:

"Raw" Acceleration data.

int32_t ADXL_getAcceleration_X(void)
------------------------------------

Reads the decoded acceleration of the x axis from the adxl345.

Returns:

Acceleration data.

int32_t ADXL_getAcceleration_Y(void)
------------------------------------

Reads the decoded acceleration of the y axis from the adxl345.

Returns:

Acceleration data.

int32_t ADXL_getAcceleration_Z(void)
------------------------------------

Reads the decoded acceleration of the z axis from the adxl345.

Returns:

Acceleration data.

Deployment & Testing
====================

The project can be build with ``make all`` and flashed to the device with
``make flash``.

Pins
~~~~

========= =======
Launchpad ADXL345
========= =======
+3.3V     VCC
+3.3V     CS
GND       GND
PB3       SDA
PB2       SD0
========= =======

SDA and SD0 are connected to GND with resistors.

Output
~~~~~~

The output looks like this:

.. code: txt

    X=96, Y=0, Z=-168

Each time a new value is printed to the command line the old one will be
overwritten, no scrolling. The unit of these values is mg.

Source
======


.. _1: 

[1] ADXL345.pdf,
    http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf,
    last viewed: 2015-01-14

.. _2: 

[2] TivaWare™ Peripheral Driver Library,
    http://www.ti.com/lit/ug/spmu298a/spmu298a.pdf,
    last viewed: 2015-01-14

.. header::

    +-------------+-------------------+------------+
    | Titel       | Autor             | Date       |
    +=============+===================+============+
    | ###Title### | Jakob Klepp       | 2015-01-14 |
    +-------------+-------------------+------------+

.. footer::

    ###Page### / ###Total###
