/*
 * Arm FastModels Hello World Boot Example
 *
 * Bootloader Main File
 * File: boot.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */


#include <types.h>
#include <config.h>
#include <bootloader_args.h>
#include <serial.h>

struct bootloader_args bootloader_args = BOOTLOADER_ARGS_INIT(CONFIG_BSP_MIPD);



void bootloader_init(struct bootloader_args *data) __attribute__((noreturn));
void bootloader_init(struct bootloader_args *data)
{
    serial_console_early_init(data->serial[CONFIG_SERIAL_CONSOLE_PORT]);
    serial_debug_early_init(data->serial[CONFIG_SERIAL_DEBUG_PORT]);

    while(1)
        ;
}