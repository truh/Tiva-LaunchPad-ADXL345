#############
I2C & ADXL345
#############

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
          SW-TM4C-DRL-UG


Source
======


.. _1:

[1] 

.. header::

    +-------------+-------------------+------------+
    | Titel       | Autor             | Date       |
    +=============+===================+============+
    | ###Title### | Jakob Klepp       | 2014-12-18 |
    +-------------+-------------------+------------+

.. footer::

    ###Page### / ###Total###
