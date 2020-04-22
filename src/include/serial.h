/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __SERIAL_H_
#define __SERIAL_H_ 1

#include <types.h>
#include <config.h>

/*
 * ===============================================================================================
 * Initialization of Serial Ports
 * ===============================================================================================
 */


/**
 * @brief initializes the serial port in physical base address
 *
 * @param port      the serial port to be initialized
 * @param pbase     the physical base address of the port
 */
void serial_early_init(unsigned int port, lpaddr_t pbase);


/**
 * @brief initializes the serial port with virtual base address
 *
 * @param port      the serial port to be initialized
 * @param vbase     the physical base address of the port
 */
void serial_init(unsigned int port, lvaddr_t vbase);


/*
 * ===============================================================================================
 * Blocking Input and Output
 * ===============================================================================================
 */


/**
 * @brief writes a character to the serial port (blocking)
 *
 * @param port  the port to be used
 * @param c     the character to be written
 */
void serial_putchar(unsigned port, char c);


/**
 * @brief reads a character from the serial port (blocking)
 *
 * @param port  the serial port to be uased
 *
 * @returns the read character
 */
char serial_getchar(unsigned port);


/*
 * ===============================================================================================
 * Serial Console
 * ===============================================================================================
 */


/**
 * @brief initializes the serial console port in physical base address
 *
 * @param port      the serial port to be initialized
 * @param pbase     the physical base address of the port
 */
static inline void serial_console_early_init(lpaddr_t pbase)
{
    serial_early_init(CONFIG_SERIAL_CONSOLE_PORT, pbase);
}


/**
 * @brief initializes the serial console port with virtual base address
 *
 * @param port      the serial port to be initialized
 * @param vbase     the physical base address of the port
 */
static inline void serial_console_init(lvaddr_t vbase)
{
    serial_init(CONFIG_SERIAL_CONSOLE_PORT, vbase);
}


/**
 * @brief writes a character to the serial console port (blocking), replacing LF with CRLF
 *
 * @param port  the port to be used
 * @param c     the character to be written
 */
static inline void serial_console_putchar(char c)
{
    if (c == '\n') {
        serial_putchar(CONFIG_SERIAL_CONSOLE_PORT, '\r');
    }
    serial_putchar(CONFIG_SERIAL_CONSOLE_PORT, c);
}


/**
 * @brief reads a character from the serial console port (blocking)
 *
 * @param port  the serial port to be uased
 *
 * @returns the read character
 */
static inline char serial_console_getchar(void)
{
    return serial_getchar(CONFIG_SERIAL_CONSOLE_PORT);
}


/*
 * ===============================================================================================
 * Debug Serial
 * ===============================================================================================
 */


/**
 * @brief initializes the serial debug port in physical base address
 *
 * @param port      the serial port to be initialized
 * @param pbase     the physical base address of the port
 */
static inline void serial_debug_early_init(lpaddr_t pbase)
{
    serial_early_init(CONFIG_SERIAL_DEBUG_PORT, pbase);
}


/**
 * @brief initializes the serial debug port with virtual base address
 *
 * @param port      the serial port to be initialized
 * @param vbase     the physical base address of the port
 */
static inline void serial_debug_init(lvaddr_t vbase)
{
    serial_init(CONFIG_SERIAL_DEBUG_PORT, vbase);
}


/**
 * @brief writes a character to the serial debug port (blocking), replacing LF with CRLF
 *
 * @param port  the port to be used
 * @param c     the character to be written
 */
static inline void serial_debug_putchar(char c)
{
    if (c == '\n') {
        serial_putchar(CONFIG_SERIAL_DEBUG_PORT, '\r');
    }
    serial_putchar(CONFIG_SERIAL_DEBUG_PORT, c);
}


/**
 * @brief reads a character from the serial debug port (blocking)
 *
 * @param port  the serial port to be uased
 *
 * @returns the read character
 */
static inline char serial_debug_getchar(void)
{
    return serial_getchar(CONFIG_SERIAL_DEBUG_PORT);
}

#endif /* __SERIAL_H_ */