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
#include <stdio.h>

#include <logging.h>
#include <armv8.h>
#include <processor.h>
#include <exceptions.h>
#include <mmu.h>
#include <cachectrl.h>

struct bootloader_args bootloader_args = BOOTLOADER_ARGS_INIT(CONFIG_BSP_MIPD,
                                                              CONFIG_SERIAL_CONSOLE_BASE);

void bootloader_init(struct bootloader_args *data) __attribute__((noreturn));
void bootloader_init(struct bootloader_args *data)
{
    /* disable interrupts */
    armv8_disable_interrupts();

    /* switch to non-secure world */
    armv8_switch_to_non_secure_world();

    /* initialize the serial console */
    serial_console_early_init(data->serial[CONFIG_SERIAL_CONSOLE_PORT]);
    serial_debug_early_init(data->serial[CONFIG_SERIAL_DEBUG_PORT]);

    DBGG("After: Console init");

    /* print the boot banner */
    // MSG("###############################################################################\n");
    // MSG("FastModels bootloader starting on %s %s core rev. r%u in EL%u\n",
    //     armv8_processor_implementer_string(), armv8_processor_model_string(),
    //     armv8_processor_revision_number, armv8_get_current_el());
    // MSG("###############################################################################\n");

    /* setup exception vectors */
    exceptions_setup_vectors();
    DBGG("After: exceptions setup");
    

    /* configure and enable MMU */
    mmu_configure_and_enable();
    DBGG("After: MMU Config and enable");

    /* enable caches */
    // cachectrl_enable_all_caches();
    DBGG("DISABLED: cache enable");


    // MSG("Skipping lower EL configuration.");
    // MSG("Reached end of initialization sequence...\n");


    printf("Testing pritnf: %s\n", "--HAHAH");
    DBGG("We are done");
    while (1);
        
}