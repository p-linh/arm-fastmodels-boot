/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef ARMV8_REGISTERS_H_
#define ARMV8_REGISTERS_H_ 1

#include <types.h>

/*
 * ======================================================================================
 * Exception Levels
 * ======================================================================================
 */


/**
 * @brief gets the current exception level
 */
static inline uint8_t armv8_get_current_el(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], CurrentEL\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    /* the exception level bits are:  EL, bits [3:2] */
    return (char)((val >> 2) & 0x3);
}


/*
 * ======================================================================================
 * Processor Identification Registers
 * ======================================================================================
 */


/**
 * @brief reads the main ID registesr
 */
static inline uint64_t armv8_midr_el1_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], MIDR_EL1\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

/**
 * @brief gets the implementer from the main id register
 */
static inline uint8_t armv8_midr_el1_get_implementer(void)
{
    uint64_t val = armv8_midr_el1_read();
    return (uint8_t)((val >> 24) & 0xff);
}

/**
 * @brief gets the implementer from the main id register
 */
static inline uint16_t armv8_midr_el1_get_partnum(void)
{
    uint64_t val = armv8_midr_el1_read();
    return (uint16_t)((val >> 4) & 0xfff);
}

/**
 * @brief gets the implementer from the main id register
 */
static inline uint8_t armv8_midr_el1_get_revision(void)
{
    uint64_t val = armv8_midr_el1_read();
    return (uint8_t)(val & 0xf);
}


/*
 * ======================================================================================
 * Exception Vectors
 * ======================================================================================
 */

static inline void armv8_vbar_el3_write(uint64_t addr)
{
    __asm volatile("msr vbar_el3, %[vbar_el3]\n"
                   "isb \n"
                   :
                   : [ vbar_el3 ] "r"(addr));
}

static inline void armv8_vbar_el2_write(uint64_t addr)
{
    __asm volatile("msr vbar_el2, %[vbar_el2]\n"
                   "isb \n"
                   :
                   : [ vbar_el2 ] "r"(addr));
}

static inline void armv8_vbar_el1_write(uint64_t addr)
{
    __asm volatile("msr vbar_el1, %[vbar_el1]\n"
                   "isb \n"
                   :
                   : [ vbar_el1 ] "r"(addr));
}

/*
 * ======================================================================================
 * SCR_EL3, Secure Configuration Register
 * ======================================================================================
 */

static inline uint64_t armv8_scr_el3_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], SCR_EL3\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

static inline void armv8_scr_el3_write(uint64_t val)
{
    __asm volatile("msr SCR_EL3, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}


static inline void armv8_switch_to_non_secure_world(void)
{
    if (armv8_get_current_el() == 3) {
        uint64_t val = armv8_scr_el3_read();
        armv8_scr_el3_write(val | 0x1);
    }
}


/*
 * ======================================================================================
 * Interrupts
 * ======================================================================================
 */


/**
 * @brief masks the interrupts on this processor
 */
static inline void armv8_disable_interrupts(void)
{
    __asm volatile("msr DAIFSet, #3\n");
}


/*
 * ======================================================================================
 * Memory Address Translation
 * ======================================================================================
 */


/*
 * ======================================================================================
 * Data and Instruction Cache Management
 * ======================================================================================
 */

#endif /* ARMV8_REGISTERS_H_ */