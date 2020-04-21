#
# Arm FastModels Hello World Boot
#
# Copyright 2020 Reto Achermann
# SPDX-License-Identifier: GPL-3.0
#

IMGNAME=bootimg

#
# Setting up the tools to be used for compilation and linking
#

# The C Compiler to be used
CC=aarch64-linux-gnu-gcc

CPP=aarch64-linux-gnu-cpp

OBJCOPY=aarch64-linux-gnu-objcopy

OBJDUMP=aarch64-linux-gnu-objdump

#
# Compiler flags for linking and compiling
#

BUILDFLAGS =\
	-O2 -g -Wall -Wextra -std=c17 \
	-fno-builtin \
	-fno-unwind-tables \
	-nostdinc \
	-std=c99 \
	-mcpu=cortex-a57 \
	-march=armv8-a+nofp

LDFLAGS=\
	-O2 -g -Wl,-N -pie \
	-fno-builtin -nostdlib \
	-Wl,--fatal-warnings -Wl,--build-id=none

INC=-I./src/include

CFILES=src/boot.c src/string.c

SFILES=src/entry.S

OBJS=build/entry.o build/boot.o build/string.o build/serial.o build/printf.o


#
# build target
#

all: bootimg


#
# Compiling single files
#

build:
	mkdir -p build


build/%.o : src/%.S build
	$(CC) $(BUILDFLAGS) $(INC) -o $@ -c $<


build/%.o : src/%.c build
	$(CC) $(BUILDFLAGS) $(INC) -o $@ -c $<


# this generates the linker script for the bootimage
build/$(IMGNAME).lds : src/boot.lds.in
	$(CPP) $(INC) -D__ASSEMBLER__ -P src/boot.lds.in $@


#
# Building the boot image
#


# This is the bootloader elf with all debug symbols etc included
build/$(IMGNAME).full : $(OBJS) build/$(IMGNAME).lds
	$(CC) $(LDFLAGS)  $(INC) -T./build/$(IMGNAME).lds -Wl,-Map,./build/$(IMGNAME).map -o $@ $(OBJS)
	bash -c "echo -e '\0002'" | dd of=$@ bs=1 seek=16 count=1 conv=notrunc status=noxfer


# This is the debug information of the bootloader elf
build/$(IMGNAME).debug : build/$(IMGNAME).full
	$(OBJCOPY) --only-keep-debug ./build/$(IMGNAME).full $@


# This is the assembly dump of the bootloader
build/$(IMGNAME).asm : build/$(IMGNAME)
	$(OBJDUMP) -d -M reg-names-raw build/$(IMGNAME) > $@


# this is the bootimage elf
build/$(IMGNAME) : build/$(IMGNAME).full build/$(IMGNAME).debug
	$(OBJCOPY) -g -add-gnu-debuglink=./build/$(IMGNAME).debug ./build/$(IMGNAME).full $@


$(IMGNAME) : build/$(IMGNAME)
	cp build/$(IMGNAME) $(IMGNAME)

