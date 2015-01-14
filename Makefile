# Tiva Makefile
# #####################################
#
# user configuration:
#######################################
# TARGET: name of the output file
TARGET = main
ARCHIV = ADXL345_Klepp
#######################################
# MCU: part number to build for
MCU = TM4C123GH6PM
#######################################
# SOURCES: list of input source sources
SOURCES += $(wildcard *.c)
#######################################
# Tivaware driverlib
#SOURCES += adc.c
#SOURCES += aes.c
#SOURCES += can.c
#SOURCES += comp.c
#SOURCES += cpu.c
#SOURCES += crc.c
#SOURCES += des.c
#SOURCES += eeprom.c
#SOURCES += emac.c
#SOURCES += epi.c
#SOURCES += flash.c
#SOURCES += fpu.c
SOURCES += gpio.c
#SOURCES += hibernate.c
SOURCES += i2c.c
#SOURCES += interrupt.c
#SOURCES += lcd.c
#SOURCES += mpu.c
#SOURCES += onewire.c
#SOURCES += pwm.c
#SOURCES += qei.c
#SOURCES += shamd5.c
#SOURCES += ssi.c
#SOURCES += sw_crc.c
SOURCES += sysctl.c
#SOURCES += sysexc.c
#SOURCES += systick.c
#SOURCES += timer.c
SOURCES += uart.c
#SOURCES += udma.c
#SOURCES += usb.c
#SOURCES += watchdog.c
#######################################
# Tivaware utils
#SOURCES += cmdline.c
#SOURCES += cpu_usage.c
#SOURCES += flash_pb.c
#SOURCES += fswrapper.c
#SOURCES += isqrt.c
#SOURCES += locator.c
#SOURCES += lwiplib.c
#SOURCES += ptpdlib.c
#SOURCES += random.c
#SOURCES += ringbuf.c
#SOURCES += scheduler.c
#SOURCES += sine.c
#SOURCES += smbus.c
#SOURCES += softi2c.c
#SOURCES += softssi.c
#SOURCES += softuart.c
#SOURCES += speexlib.c
#SOURCES += spi_flash.c
#SOURCES += swupdate.c
#SOURCES += tftp.c
SOURCES += uartstdio.c
#SOURCES += ustdlib.c
#SOURCES += wavfile.c
#######################################
# TIVAWARE_PATH: path to tivaware folder
TIVAWARE_PATH = $(HOME)/opt/tivaware
#######################################
# Driverlib
#TI_LIB = $(TIVAWARE_PATH)/driverlib/gcc/libdriver.a
#######################################
# OUTDIR: directory to use for output
OUTDIR = build
#######################################
# GCC path
GCC_PATH = /usr/lib/gcc/arm-none-eabi/4.9.2
#######################################
# INCLUDES: list of includes, by default, use Includes directory
INCLUDES = -IInclude -I$(TIVAWARE_PATH)
#######################################
# LD_SCRIPT: linker script
LD_SCRIPT = $(MCU).ld
LDFLAGS = -T $(LD_SCRIPT) --entry ResetISR --gc-sections
LDFLAGS += -L$(GCC_PATH)/fpu/ -lgcc
#######################################
# Path to look for source files at
VPATH = src:libraries:$(TIVAWARE_PATH):$(TIVAWARE_PATH)/driverlib:$(TIVAWARE_PATH)/utils
#######################################
# define flags
CFLAGS += -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS += -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall -Dgcc
CFLAGS += -pedantic -DPART_$(MCU) -c -DTARGET_IS_BLIZZARD_RA1 $(INCLUDES)
CFLAGS += -g -D DEBUG
#######################################
# Clang Flags
CLANG_INCLUDES += -I$(GCC_PATH)/include 
CLANG_FLAGS += -O0 -g -nostdlib -ffreestanding -target arm-none-eabi
CLANG_FLAGS += -mfloat-abi=soft
CLANG_FLAGS += $(CLANG_INCLUDES)
#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
#C       = clang $(CLANG_FLAGS)
#C++     = clang++ $(CLANG_FLAGS)
C       = arm-none-eabi-gcc
C++     = arm-none-eabi-g++
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -f
MKDIR	= mkdir -p
#######################################
# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:=.o)))

# default: build bin
all: $(OUTDIR)/$(TARGET).bin
#all: $(OUTDIR)/$(ARCHIV).zip

archiv: $(OUTDIR)/$(ARCHIV).zip

$(OUTDIR)/%.c.o: %.c | $(OUTDIR)
	$(C) -o $@ $^ $(CFLAGS)

$(OUTDIR)/%.cpp.o: %.cpp | $(OUTDIR)
	$(C++) -o $@ $^ $(CFLAGS)

$(OUTDIR)/$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/$(TARGET)
	$(OBJCOPY) -O binary $< $@

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

README.pdf:
	rst2pdf README.rst

$(OUTDIR)/$(ARCHIV).zip: README.pdf
	zip -r $(OUTDIR)/$(ARCHIV).zip . -x build/* -x build/

flash: $(OUTDIR)/$(TARGET).bin
	lm4flash $(OUTDIR)/$(TARGET).bin

program: flash

debug: clean $(OUTDIR)/$(TARGET).bin
	debug/debug_nemiver.sh

clean:
	-$(RM) $(OUTDIR)/*
	-$(RM) README.pdf

print_objs:
	echo $(OBJECTS)

.PHONY: all flash program debug clean
