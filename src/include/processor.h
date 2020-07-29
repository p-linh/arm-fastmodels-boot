/*
 * Arm FastModels Hello World Boot Example
 *
 * Bootloader Main File
 * File: boot.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef ARMV8_PROCESSOR_H_
#define ARMV8_PROCESSOR_H_ 1

#include <armv8.h>

/**
 * @brief returns the processor implementer string
 */
const char *armv8_processor_implementer_string(void);


/**
 * @brief returns the processor model string
 */
const char *armv8_processor_model_string(void);


/**
 * @brief returns the processor model string
 */
unsigned armv8_processor_revision_number(void);

#endif /* ARMV8_PROCESSOR_H_ */