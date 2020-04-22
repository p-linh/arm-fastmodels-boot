/*
 * Arm FastModels Hello World Boot Example
 *
 * Exception Handlers
 * File: exceptions.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <types.h>
#include <exceptions.h>
#include <stdio.h>

#define VECTORNAMECASE(_var, _x) \
    case _x:                 \
        _var = #_x;          \
        break;

/**
 * @brief handler function for unknown / unhandled exceptions
 *
 * @param epc    the program counter that caused the exception
 * @param spsr   the sps
 * @param esr    the exception syndrome register value
 * @param vector the exception vector
 */
void exceptions_handle_unsupported(uint64_t epc, uint64_t spsr, uint64_t esr, uint64_t vector)
{
    (void)esr;
    (void)spsr;
    (void)epc;
    const char *vecname = NULL;
    char vecname_buf[20];
    snprintf(vecname_buf, sizeof(vecname_buf), "0x%016" PRIx64, vector);

    switch(vector) {
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_UNDEFINED)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SYNC)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_IRQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_FIQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_EL0_SERROR)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SYNC)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_IRQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_FIQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_EL_CURRENT_STACK_CURRENT_SERROR)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_LOWER_EL_SYNC)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_LOWER_EL_IRQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_LOWER_EL_FIQ)
        VECTORNAMECASE(vecname, AARCH64_EVECTOR_LOWER_EL_SERROR)
        VECTORNAMECASE(vecname, AARCH32_EVECTOR_LOWER_EL_SYNC)
        VECTORNAMECASE(vecname, AARCH32_EVECTOR_LOWER_EL_IRQ)
        VECTORNAMECASE(vecname, AARCH32_EVECTOR_LOWER_EL_FIQ)
        VECTORNAMECASE(vecname, AARCH32_EVECTOR_LOWER_EL_SERROR);
        default:
            vecname = "unknown";
    }


    (void)vecname;
}


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
                                    uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t context)
{
    (void)fid;
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    (void)context;
}
