/*
 * Arm FastModels Hello World Boot Example
 *
 * Exception Vector Table
 * File: exception_vectors.S
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <offsets.h>
#include <exceptions.h>

/* make the symbold global */
.global __exn_vectors_start, __exn_vectors_end


/*
 * ###############################################################################################
 * EXCEPTION VECTOR TABLE
 * ###############################################################################################
 *
 * there are four exception groups:
 *  1) from the current level, with the EL0 stack
 *  2) from the current level, with our own stack
 *  3) from a lower level, using AArch64
 *  4) from a lower level, using AArch32
 *
 * Each exception group has four different types of exceptions
 *  1) Synchronous exceptions (system calls)
 *  2) Interrupts
 *  3) Fast interupts (higher priority)
 *  4) Syncrhonous errors
 *
 * So there are 16 entries in this table, each of which occuping 128B.
 * The table is 2 KiB in size and  needs to be naturally aligned
 *
 */


/*
 * This macro branches to the unsupported exception handler
 */
 .macro invalid_exception vector

    /* set the stack */
    adr x0,  __exception_stack
    add sp, x0, #(EXCEPTION_STACK_SIZE - 16)

    /* store spsr and other information */
    mrs x0, elr_el3
    mrs x1, spsr_el3

    /* Exception Syndrome Register */
    mrs x2, esr_el3

    /* Exception vector */
    mov x3, \vector

    /* Arguments: x0 = EPC, x1 = SPSR, x2 = ESR, x3 = vector */
    b exceptions_handle_unsupported
.endm



.align 11
__exn_vectors_start:


/*
 * ==========================================================================
 * Current EL, with EL0 Stack   -   not used
 * ==========================================================================
 */


/*
 * 0x000 - Current EL, SP0 - Synchronous Exception
 * --------------------------------------------------------------------------
 */

.align 7
el1_sp_el0_sync:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SYNC

/*
 * 0x080 - Current EL, SP0 - IRQ / vIRQ
 * --------------------------------------------------------------------------
 */

.align 7
el1_sp_el0_irq:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_IRQ

/*
 * 0x100 - Current EL, SP0 - FIQ / vFIQ
 * --------------------------------------------------------------------------
 */

.align 7
el1_sp_el0_fiq:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_FIQ


/*
 * 0x180 - Current EL, SP0 - SError / vSError
 * --------------------------------------------------------------------------
 */

.align 7
el1_sp_el0_serror:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SERROR


/*
 * ==========================================================================
 * Current EL, with own Stack   -   exceptions from ourselves
 * ==========================================================================
 */


/*
 * 0x200 - Current EL, SPx - Synchronous Exception
 * --------------------------------------------------------------------------
 */

.align 7
elx_spx_sync:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SYNC

/*
 * 0x280 - Current EL, SPx - IRQ / vIRQ
 * --------------------------------------------------------------------------
 */

.align 7
elx_spx_irq:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_IRQ

/*
 * 0x300 - Current EL, SPx - FIQ / vFIQ
 * --------------------------------------------------------------------------
 */

.align 7
elx_spx_fiq:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_FIQ


/*
 * 0x380 - Current EL, SPx - SError / vSError
 * --------------------------------------------------------------------------
 */

.align 7
elx_spx_serror:
    invalid_exception AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SERROR


/*
 * ==========================================================================
 * Lower EL using AArch64 - Those are basically SMC calls
 * ==========================================================================
 */


/*
 * 0x400 - Lower EL using AArch64  - Synchronous Exception
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch64_sync:

    /* SMC Calls:
     * w0:    function identifier
     * x1-x6: arguments
     * x0-x3: results
     * x9-x15: temporary registers.
     * Registers X18-X30 and stack pointers SP_EL0 and SP_ELx are saved by the
     * function that is called, and must be preserved over the SMC or HVC call.
     */

    /* set up the stack - XXX: this is not thread safe!*/
    adr x9, __exception_stack
    add sp, x9, #(EXCEPTION_STACK_SIZE - 16)

    /* Enable the SError interrupt */
    msr    daifclr, #(1 << 2)

    /* we expect only SMC calls to happen */
    mrs    x8, esr_el3
    ubfx   x8, x8, #26, #6
    cmp    x8, #0x17
    b.ne   el_lo_aarch64_sync_err

    /* make room on the stack for caller save values, the context */
    add sp, sp, #-(16 * 8)

    /* store the values */
    stp x18, x19, [sp]
    stp x20, x21, [sp, #(2 * 8)]
    stp x22, x23, [sp, #(4 * 8)]
    stp x24, x25, [sp, #(6 * 8)]
    stp x26, x27, [sp, #(8 * 8)]
    stp x28, x29, [sp, #(10 * 8)]
    stp x30, x30, [sp, #(12 * 8)]
    mrs x9, sp_el0
    mrs x10, sp_el1
    stp x9, x10, [sp, #(14 * 8)]

    /* set the context pointer in 7th argument */
    mov x7, sp

    /* branch off to the exception handler */
    b exceptions_handle_aarch64_sync


/*
 * 0x480 - Lower EL using AArch64  - IRQ / vIRQ
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch64_irq:
    invalid_exception AARCH64_EVECTOR_LOWER_EL_IRQ

/*
 * 0x500 - Lower EL using AArch64  - FIQ / vFIQ
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch64_fiq:
    invalid_exception AARCH64_EVECTOR_LOWER_EL_FIQ


/*
 * 0x580 - Lower EL using AArch64  - SError / vSError
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch64_serror:
    invalid_exception AARCH64_EVECTOR_LOWER_EL_SERROR


/*
 * ==========================================================================
 * Lower EL using AArch32   -   not used
 * ==========================================================================
 */


/*
 * 0x600 - Lower EL using AArch32 - Synchronous Exception
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch32_sync:
    invalid_exception AARCH32_EVECTOR_LOWER_EL_SYNC

/*
 * 0x680 - Lower EL using AArch32  IRQ / vIRQ
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch32_irq:
    invalid_exception AARCH32_EVECTOR_LOWER_EL_IRQ

/*
 * 0x700 - Lower EL using AArch32 - FIQ / vFIQ
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch32_fiq:
    invalid_exception AARCH32_EVECTOR_LOWER_EL_FIQ


/*
 * 0x780 - Lower EL using AArch32 - SError / vSError
 * --------------------------------------------------------------------------
 */

.align 7
el_lo_aarch32_serror:
    invalid_exception AARCH32_EVECTOR_LOWER_EL_SERROR


/*
 * 0x800 - End of Exception Table
 * --------------------------------------------------------------------------
 */

.align 7
__exn_vectors_end:



el_lo_aarch64_sync_err:
    invalid_exception AARCH64_EVECTOR_LOWER_EL_SYNC




/*
 * ===============================================================================================
 * Exception Return Function used to drop to lower EL
 * ===============================================================================================
 */

.global eret
.type eret, @function

eret:
    eret


/*
 * ###############################################################################################
 * The Exception Stack
 * ###############################################################################################
 */

.data
.align 4

 __exception_stack:
    .space EXCEPTION_STACK_SIZE, 0