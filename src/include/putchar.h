/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __PUTCHAR_H_
#define __PUTCHAR_H_ 1

#include <serial.h>

static inline void putchar(char character)
{
    serial_console_putchar(character);
}

#endif /* __PUTCHAR_H_ */