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

#define MRS(reg, v) asm volatile("mrs %x0," reg : "=r"(v))
#define MSR(reg, v)                                \
    do {                                           \
        uint64_t _v = v;                             \
        asm volatile("msr " reg ",%x0" :: "r" (_v));\
    }while(0)

#define CPACR_EL1_FPEN          20     // FP regiters access
/* Enable FPU access in EL0 and EL1 */
static inline void enableFpuEL01(void)
{
    uint64_t cpacr;
    MRS("cpacr_el1", cpacr);
    cpacr |= (3 << CPACR_EL1_FPEN);
    MSR("cpacr_el1", cpacr);
    isb();
}

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

    enableFpuEL01();

    /* print the boot banner */
    MSG("###############################################################################\n");
    MSG("FastModels bootloader starting on %s %s core rev. r%u in EL%u\n",
        armv8_processor_implementer_string(), armv8_processor_model_string(),
        armv8_processor_revision_number, armv8_get_current_el());
    MSG("###############################################################################\n");

    /* setup exception vectors */
    exceptions_setup_vectors();

    /* configure and enable MMU */
    mmu_configure_and_enable();

    /* enable caches */
    // cachectrl_enable_all_caches();
    MSG("DISABLED: cache enable, in EL1\n");


    // MSG("Skipping lower EL configuration.");
    MSG("Reached end of initialization sequence...\n");
    while (1)
    ;
}