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
#define AARCH64_EVECTOR_UNDEF       0x00

/* current exception level, with EL0 stack */
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SYNC    0x01
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_IRQ     0x02
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_FIQ     0x03
#define AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SERROR  0x04

/* current exception level, with own stack */
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SYNC    0x05
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_IRQ     0x06
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_FIQ     0x07
#define AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SERROR  0x08

/* lower exception level using AARCH64 */
#define AARCH64_EVECTOR_LOWER_EL_SYNC    0x09
#define AARCH64_EVECTOR_LOWER_EL_IRQ     0x0a
#define AARCH64_EVECTOR_LOWER_EL_FIQ     0x0b
#define AARCH64_EVECTOR_LOWER_EL_SERROR  0x0c

/* lower exception level using AARCH32 */
#define AARCH32_EVECTOR_LOWER_EL_SYNC    0x10
#define AARCH32_EVECTOR_LOWER_EL_IRQ     0x11
#define AARCH32_EVECTOR_LOWER_EL_FIQ     0x12
#define AARCH32_EVECTOR_LOWER_EL_SERROR  0x13


#endif /* __EXCEPTIONS_H_ */