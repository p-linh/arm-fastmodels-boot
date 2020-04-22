/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __TYPES_H_
#define __TYPES_H_ 1

/* basic type definitions */
typedef signed char int8_t;
typedef unsigned char uint8_t;
#define PRIu8 "u" /* uint8_t */
#define PRIx8 "x" /* uint8_t */

typedef short int16_t;
typedef unsigned short uint16_t;
#define PRIu16 "u" /* uint16_t */
#define PRIx16 "x" /* uint16_t */

typedef int int32_t;
typedef unsigned int uint32_t;
#define PRIu32 "u" /* uint32_t */
#define PRIx32 "x" /* uint32_t */

typedef long int64_t;
typedef unsigned long uint64_t;
#define PRIu64 "lu" /* uint64_t */
#define PRIx64 "lx" /* uint64_t */

typedef uint64_t uintptr_t;
#define PRIuPTR "lu" /* uintptr_t */
#define PRIxPTR "lx" /* uintptr_t */

typedef uint64_t size_t;
typedef int64_t ptrdiff_t;
typedef int64_t intmax_t;


/* special type definitions */
typedef uintptr_t lpaddr_t;
typedef uintptr_t lvaddr_t;
typedef uintptr_t genaddr_t;

/* null constant */
#define NULL ((void *)0)

/* bools */
#define false 0
#define true 1
typedef int bool;

/* va_list type for printf */
typedef __builtin_va_list __va_list; /* internally known to gcc */
typedef __va_list va_list;

#define va_start(ap, last) __builtin_va_start((ap), (last))

#define va_arg(ap, type) __builtin_va_arg((ap), type)

#if __ISO_C_VISIBLE >= 1999
#    define va_copy(dest, src) __builtin_va_copy((dest), (src))
#endif

#define va_end(ap) __builtin_va_end(ap)

#endif /* __TYPES_H_ */