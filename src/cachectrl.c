/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <logging.h>
#include <cachectrl.h>
#include <armv8.h>

/**
 * @brief cleans the instruction cache
 */
void cachectrl_clean_icache()
{ /* no-op */
}

/**
 * @brief invalidates the instruction cache
 */
void cachectrl_invalidate_icache()
{
    __asm volatile("ic      iallu \n");
    dmb();
    isb();
}

/**
 * @brief enables the instruction cache
 */
void cachectrl_enable_icache()
{
    cachectrl_invalidate_icache();
    armv8_sctlr_el3_set_i(1);
}

/**
 * @brief disable the instruction cache
 */
void cachectrl_disable_icache()
{
    armv8_sctlr_el3_set_i(0);
}

static inline int clz(uint64_t x)
{
    int r;
    __asm volatile("clz %[r], %[x]" : [ r ] "=r"(r) : [ x ] "r"(x));
    return r;
}


/* calculates the log2 of a number */
static inline int log2i(uint64_t x)
{
    return 64 - clz(x - 1);
}


static void do_clean_or_invalidate(bool clean)
{
    uint64_t clidr = armv8_clidr_el1_read();
    uint8_t loc = armv8_clidr_el1_get_loc();

    /* clear the cache selector */
    armv8_csselr_el1_write(0);

    /* Invalidate all data caches up to the point of coherence.
       NOTE: level must start at 1.
     */
    for (uint8_t level = 1; level <= loc; level++) {
        uint8_t type = clidr & 0x7;  // each Ctype field is 3 bits

        /// shift to the next cache type.
        clidr = clidr >> 3;

        if (type == 0 || type == 1) {
            /// there was no data cache here, continue with next level
            continue;
        }

        /* set the level we are targeting now */
        armv8_csselr_el3_set_level(level);


        /* Read the data cache size & associativity. */
        uint64_t csidr = armv8_cssidr_el1_read();

        uint32_t sets = (uint32_t)((csidr >> 32) & 0xffffff) + 1;
        uint32_t assoc = (uint32_t)((csidr >> 3) & 0xfffff) + 1;
        uint32_t linesizebits = (uint32_t)((csidr)&0x7) + 4;


        /* Calculate the field offsets for the invalidate operation. */
        int setbits = log2i(sets);
        int assocbits = log2i(assoc);

        for (int w = 0; w < assoc; w++) {
            for (int s = 0; s < sets; s++) {
                // set which level to operate on, bits [3:1]
                uint64_t op = ((level - 1) & 0x7) << 1;

                // SetWay, bits [31:4]
                /*
                    Contains two fields:

                        Way, bits[31:32-A], the number of the way to operate on.
                        Set, bits[B-1:L], the number of the set to operate on.
                        Bits[L-1:4] are RES0.

                        A = Log2(ASSOCIATIVITY),
                        L = Log2(LINELEN),
                        B = (L + S),
                        S = Log2(NSETS).

                        ASSOCIATIVITY, LINELEN (line length, in bytes), and NSETS (number of sets)
                        have their usual meanings and are the values for the cache level being
                        operated on. The values of A and S are rounded up to the next integer.

                */

                // setting the way bits
                op |= (w << (32 - assocbits));

                // setting the set bits
                op |= (s << (linesizebits));

                if (clean) {
                    __asm volatile("dc csw, %[op]" : : [ op ] "r"(op));
                } else {
                    __asm volatile("dc isw, %[op]" : : [ op ] "r"(op));
                }
            }
        }
    }

    dmb();
    isb();
}


/**
 * @brief invalidates the data cache
 */
void cachectrl_invalidate_dcache()
{
    do_clean_or_invalidate(false);
}

/**
 * @brief cleans the data cache
 */
void cachectrl_clean_dcache()
{
    do_clean_or_invalidate(true);
}

/**
 * @brief enables the data cache
 */
void cachectrl_enable_dcache()
{
    cachectrl_invalidate_dcache();
    armv8_sctlr_el3_set_c(1);
}

/**
 * @brief disables the data cache
 */
void cachectrl_disable_dcache()
{
    armv8_sctlr_el3_set_c(0);
}


/**
 * @brief enables the data and instruction caches
 */
void cachectrl_enable_all_caches()
{
    cachectrl_enable_dcache();
    cachectrl_enable_icache();
}

/**
 * @brief disables the data and instruction caches
 */
void cachectrl_disable_all_caches()
{
    cachectrl_disable_dcache();
    cachectrl_disable_icache();
}
