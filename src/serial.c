/*
 * Arm FastModels Hello World Boot Example
 *
 * Serial Driver
 * File: serial.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <config.h>
#include <types.h>
#include <serial.h>
#include <assert.h>

/*
 * ###############################################################################################
 * pl011 uart serial driver
 * ###############################################################################################
 */

///< device base addressses
static genaddr_t uart_base[CONFIG_SERIAL_NUM_PORTS];


/*
 * ===============================================================================================
 * Register Access
 * ===============================================================================================
 */

/* registers offsets */
#define PL011_UART_REG_DATA 0x0
#define PL011_UART_REG_FLAG 0x18
#define PL011_UART_REG_CTRL 0x30
#define PL011_UART_REG_IMSC 0x38

/* field offsets in the control register */
#define PL011_UART_CTRL_ENABLE 0x1
#define PL011_UART_CTRL_TX_ENABLE 0x100
#define PL011_UART_CTRL_RX_ENABLE 0x200


static inline void pl011_uart_disable(genaddr_t base)
{
    volatile uint32_t *cr = (volatile uint32_t *)(base + PL011_UART_REG_CTRL);
    *cr = 0x0;
}

static inline void pl011_uart_enable(genaddr_t base)
{
    volatile uint32_t *cr = (volatile uint32_t *)(base + PL011_UART_REG_CTRL);
    *cr = *cr | PL011_UART_CTRL_ENABLE;
}

static inline void pl011_uart_tx_enable(genaddr_t base)
{
    volatile uint32_t *cr = (volatile uint32_t *)(base + PL011_UART_REG_CTRL);
    *cr = *cr | PL011_UART_CTRL_TX_ENABLE;
}

static inline void pl011_uart_rx_enable(genaddr_t base)
{
    volatile uint32_t *cr = (volatile uint32_t *)(base + PL011_UART_REG_CTRL);
    *cr = *cr | PL011_UART_CTRL_RX_ENABLE;
}

static inline void pl011_uart_mask_interrupts(genaddr_t base)
{
    volatile uint32_t *imsc = (volatile uint32_t *)(base + PL011_UART_REG_IMSC);
    *imsc = 0;
}


/*
 * ===============================================================================================
 * Initialization of Serial Ports
 * ===============================================================================================
 */


/**
 * @brief initializes the pl011 uart hardware
 *
 * @param base  the base address of the hardware
 */
static void pl011_uart_init(genaddr_t base)
{
    // disable uart before reconfiguring
    pl011_uart_disable(base);

    // disable all interrupts
    pl011_uart_mask_interrupts(base);

    // don't enable receiving for now
    // pl011_uart_rx_enable();

    pl011_uart_tx_enable(base);
    pl011_uart_enable(base);
}


/**
 * @brief initializes the serial port in physical base address
 *
 * @param port      the serial port to be initialized
 * @param pbase     the physical base address of the port
 */
void serial_early_init(unsigned int port, lpaddr_t pbase)
{
    assert(port < CONFIG_SERIAL_NUM_PORTS);
    uart_base[port] = (genaddr_t)pbase;

    // disable interrupts
    pl011_uart_mask_interrupts(uart_base[port]);

    // make sure uart is transmitting
    pl011_uart_tx_enable(uart_base[port]);
    pl011_uart_enable(uart_base[port]);
}


/**
 * @brief initializes the serial port with virtual base address
 *
 * @param port      the serial port to be initialized
 * @param vbase     the physical base address of the port
 */
void serial_init(unsigned int port, lvaddr_t vbase)
{
    assert(port < CONFIG_SERIAL_NUM_PORTS);
    uart_base[port] = (genaddr_t)vbase;

    pl011_uart_init(uart_base[port]);
}


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
void serial_putchar(unsigned port, char c)
{
    assert(port < CONFIG_SERIAL_NUM_PORTS);

    /* wait until we can transmit more data */
    volatile uint32_t *fr = (volatile uint32_t *)(uart_base[port] + PL011_UART_REG_FLAG);
    while (((*fr >> 5) & 0x1) == 1)
        ;

    /* write data */
    volatile uint32_t *dr = (volatile uint32_t *)(uart_base[port] + PL011_UART_REG_DATA);
    *dr = (uint32_t)c;
}


/**
 * @brief reads a character from the serial port (blocking)
 *
 * @param port  the serial port to be uased
 *
 * @returns the read character
 */
char serial_getchar(unsigned port)
{
    assert(port < CONFIG_SERIAL_NUM_PORTS);

    /* wait until there is data ready */
    volatile uint32_t *fr = (volatile uint32_t *)(uart_base[port] + PL011_UART_REG_FLAG);
    while (((*fr >> 4) & 0x1) == 1)
        ;

    /* read the data register */
    volatile uint32_t *dr = (volatile uint32_t *)(uart_base[port] + PL011_UART_REG_DATA);
    return (char)(*dr & 0xff);
}
