/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __EXCEPTIONS_H_
#define __EXCEPTIONS_H_ 1


/*
 * ===============================================================================================
 * ARMv8 - AARCH64 Exception Vectors
 * ===============================================================================================
 */


/* undefined exception */
#define AARCH64_EVECTOR_UNDEFINED 0x00

/* current exception level, with EL0 stack */
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SYNC 0x01
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_IRQ 0x02
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_FIQ 0x03
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SERROR 0x04

/* current exception level, with own stack */
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SYNC 0x05
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_IRQ 0x06
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_FIQ 0x07
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SERROR 0x08

/* lower exception level using AARCH64 */
#define AARCH64_EVECTOR_LOWER_EL_SYNC 0x09
#define AARCH64_EVECTOR_LOWER_EL_IRQ 0x0a
#define AARCH64_EVECTOR_LOWER_EL_FIQ 0x0b
#define AARCH64_EVECTOR_LOWER_EL_SERROR 0x0c

/* lower exception level using AARCH32 */
#define AARCH32_EVECTOR_LOWER_EL_SYNC 0x10
#define AARCH32_EVECTOR_LOWER_EL_IRQ 0x11
#define AARCH32_EVECTOR_LOWER_EL_FIQ 0x12
#define AARCH32_EVECTOR_LOWER_EL_SERROR 0x13


/*
 * ===============================================================================================
 * Base of the Exception Table
 * ===============================================================================================
 */

#ifndef __ASSEMBLER__

#include <types.h>

extern long __exn_vectors_start;
extern long __exn_vectors_end;


/**
 * @brief function to execute an exception return used to drop to lower execution level
 *
 * @param a0    the first argument
 * @param a1    the second argument
 * @param a2    the third arguent
 * @param a3    the fourth argument
 *
 * this function is defined in exceptions.S
 */
void eret(uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3);


/**
 * @brief handler function for unknown / unhandled exceptions
 *
 * @param epc    the program counter that caused the exception
 * @param spsr   the sps
 * @param esr    the exception syndrome register value
 * @param vector the exception vector
 */
void exceptions_handle_unsupported(uint64_t epc, uint64_t spsr, uint64_t esr, uint64_t vector);


/**
 * @brief handler function for synchronous exceptions from lower EL in AARCH64 Mode
 *
 * @param fid       the function identifier to be invoked
 * @param arg0      the first argument passed to the function
 * @param arg1      the second argument passed to the function
 * @param arg2      the third argument passed to the function
 * @param arg3      the fourth argument passed to the function
 * @param arg4      the fifth argument passed to the function
 * @param arg5      the sixth argument passed to the function
 * @param context   more context
 */
void exceptions_handle_aarch64_sync(uint32_t fid, uint64_t arg0, uint64_t arg1, uint64_t arg2,
                                    uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t context);

#endif /* __ASSEMBLER__ */

#endif /* __EXCEPTIONS_H_ */