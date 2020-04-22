/*
 * Arm FastModels Hello World Boot Example
 *
 * Printf Support
 * File: printf.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __STDIO_H_
#define __STDIO_H_ 1

#include <types.h>

int printf(const char *format, ...);
int sprintf(char *buffer, const char *format, ...);
int snprintf(char *buffer, size_t count, const char *format, ...);
int vsnprintf(char *buffer, size_t count, const char *format, va_list va);
int vprintf(const char *format, va_list va);
int fctprintf(void (*out)(char character, void *arg), void *arg, const char *format, ...);

#endif /* __STDIO_H_ */