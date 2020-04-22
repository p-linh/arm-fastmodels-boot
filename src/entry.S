/*
 * Arm FastModels Hello World Boot Example
 *
 * Bootloader Entry File
 * File: entry.S
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <offsets.h>


/*
 * ###############################################################################################
 * bootseq Text Section
 * ###############################################################################################
 */

.section .bootseq, "a"


/*
 * ============================================================================
 * This is the entry point of the bootloader in  FastModels simulator.
 * Note: When multiple cores are present, this instructions are executed
 *       in parallel, as all cores start execution at the same time.
 * Highest exeption level, MMU and caches disabled.
 * ============================================================================
 */

.global boot_entry
.type boot_entry, @function

boot_entry:

    /* clear the entire 64-bit x0 register, as we load 32-bits into that later */
    mov x0, xzr

    /* figure out which processor we are by loading the Multiprocessor Affinity Register into x0 */
    mrs x0, MPIDR_EL1

    /* get the [cluster,core,thread] part of the MIPDIR. */
    /* XXX: this currenlty ignores higher-order affinity information */
    and x1, x0, #0x00ffffff

    /* load the value of the coredata mpidr */
    adr x2, bootloader_args
    ldr x0, [x2, #BOOTINFO_MPID_OFFSET]

    /* compare our MPID with the one stored in the bootloader_args struct */
    cmp x1, x0

    /* if our MPID matches, the we continue booting */
    b.eq continue_boot

    /* there was no match, we need to try again, make sure all instructions are executed*/
    dsb ish

    /* wait for the event signal */
    wfe

    /* retry with booting */
    dsb ish
    b boot_entry


continue_boot:
    /* from here on, this core should be the only one running for now */

    /* setup the stack frame using the shared bootloader stack */
    adr x0, __bootloader_stack
    add sp, x0, #(BOOT_STACK_SIZE - 16)

    /* load the argument in register x0, by default this points to bootloader_args */
    ldr x0, bootloader_startup_fn_arg

    /*
     * load the function address to be called in register x1
     * This can be changed to another function
     */
    ldr x1, bootloader_startup_fn

    /* jump to register x1 */
    br x1


/*
 * ###############################################################################################
 * DATA SECTION
 * ###############################################################################################
 */

.data

/*
 * ============================================================================
 * Startup function and its argument
 * ============================================================================
 */

.align 3
bootloader_startup_fn:
    .dword bootloader_init
.type bootloader_startup_fn, @object
.global bootloader_startup_fn

bootloader_startup_fn_arg:
    .dword bootloader_args
.type bootloader_startup_fn_arg, @object
.global bootloader_startup_fn_arg


/*
 * ============================================================================
 * The boot Stack
 * ============================================================================
 */

/* NOTE: This stack is shared among the cores so ensure mutual exclusion */

.align 4
.global __bootloader_stack
.global __bootloader_stack_end
__bootloader_stack:
    .space BOOT_STACK_SIZE, 0
__bootloader_stack_end:
