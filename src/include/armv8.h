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
 * Barriers
 * ======================================================================================
 */

static inline void dmb(void)
{
    __asm volatile("dmb sy" : : : "memory");
}

static inline void isb(void)
{
    __asm volatile("isb" ::: "memory");
}

static inline void dsb(void)
{
    __asm volatile("dsb sy" ::: "memory");
}


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
 * SCTLR_EL3, System Control Register (EL3)
 * ======================================================================================
 */


static inline uint64_t armv8_sctlr_el3_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], SCTLR_EL3\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

static inline void armv8_sctlr_el3_write(uint64_t val)
{
    __asm volatile("msr SCTLR_EL3, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}


static inline bool armv8_sctlr_el3_get_m(void)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    return (val & 0x1) == 0x1;
}

static inline void armv8_sctlr_el3_set_m(uint64_t value)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    val &= ~(0x1);
    val |= value;
    armv8_sctlr_el3_write(val);
}

static inline bool armv8_sctlr_el3_get_c(void)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    return ((val >> 2) & 0x1) == 0x1;
}

static inline void armv8_sctlr_el3_set_c(uint64_t value)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    val &= ~(0x1 << 2);
    val |= (value << 2);
    armv8_sctlr_el3_write(val);
}

static inline bool armv8_sctlr_el3_get_i(void)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    return ((val >> 12) & 0x1) == 0x1;
}

static inline void armv8_sctlr_el3_set_i(uint64_t value)
{
    uint64_t val;
    val = armv8_sctlr_el3_read();
    val &= ~(0x1 << 12);
    val |= (value << 12);
    armv8_sctlr_el3_write(val);
}


/*
 * ======================================================================================
 * TTBR0_EL3, Translation Base Register
 * ======================================================================================
 */

static inline void armv8_ttbr0_el3_write(uint64_t val)
{
    __asm volatile("msr TTBR0_EL3, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}

static inline uint64_t armv8_ttbr0_el3_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], TTBR0_EL3\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}


/*
 * ======================================================================================
 * MAIR_EL3, Memory Attribute Indirection Register (EL3)
 * ======================================================================================
 */

static inline void armv8_mair_el3_write(uint64_t val)
{
    __asm volatile("msr MAIR_EL3, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}

static inline uint64_t armv8_mair_el3_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], MAIR_EL3\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}


/*
 * ======================================================================================
 * TCR_EL3, Translation Control Register (EL3)
 * ======================================================================================
 */

static inline void armv8_tcr_el3_write(uint64_t val)
{
    __asm volatile("msr TCR_EL3, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}

static inline uint64_t armv8_tcr_el3_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], TCR_EL3\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
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
 * CLIDR_EL1, Cache Level ID Register
 * ======================================================================================
 */

static inline uint64_t armv8_clidr_el1_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], CLIDR_EL1\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

static inline uint8_t armv8_clidr_el1_get_loc(void)
{
    uint64_t val = armv8_clidr_el1_read();
    return (uint8_t)((val >> 24) & 0x7);
}



/*
 * ======================================================================================
 * CSSELR_EL1, Cache Size Selection Register
 * ======================================================================================
 */

static inline void armv8_csselr_el1_write(uint64_t val)
{
    val = val & 0x1f;
    __asm volatile("msr CSSELR_EL1, %[val]\n"
                   "isb \n"
                   :
                   : [ val ] "r"(val));
}

static inline uint64_t armv8_csselr_el1_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], CSSELR_EL1\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

static inline void armv8_csselr_el3_set_level(uint64_t level)
{
    uint64_t val;
    val = armv8_csselr_el1_read();
    val &= ~(0x7 << 1);
    val |= (((level - 1) & 0x7) << 12);
    armv8_csselr_el1_write(val);
}

/*
 * ======================================================================================
 * CCSIDR_EL1, Current Cache Size ID Register
 * ======================================================================================
 */


static inline uint64_t armv8_cssidr_el1_read(void)
{
    uint64_t val;
    __asm volatile("mrs %[val], CCSIDR_EL1\n"
                   "isb \n"
                   : [ val ] "=r"(val));

    return val;
}

#endif /* ARMV8_REGISTERS_H_ */