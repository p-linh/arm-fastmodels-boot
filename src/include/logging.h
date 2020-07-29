/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

///< a basic message printing macro
#define MSG(format, ...) printf("[ARMv8]: " format, ## __VA_ARGS__ )

#define WARN(format, ...) printf("[ARMv8]: WARNING " format, ## __VA_ARGS__ )

#define ERROR(format, ...) printf("[ARMv8]: " format, ## __VA_ARGS__ )
