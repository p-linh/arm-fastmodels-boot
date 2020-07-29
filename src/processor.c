/*
 * Arm FastModels Hello World Boot Example
 *
 * Bootloader Main File
 * File: boot.c
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <processor.h>


/**
 * @brief returns the processor implementer string
 */
const char *armv8_processor_implementer_string(void)
{
    switch (armv8_midr_el1_get_implementer()) {
    case 0x41:
        return "ARM";
    case 0x42:
        return "Broadcom";
    case 0x43:
        return "Cavium";
    case 0x44:
        return "Digital Equipment Corporation";
    case 0x49:
        return "Infineon";
    case 0x4D:
        return "Motorola/Freescale";
    case 0x4E:
        return "NVIDIA";
    case 0x50:
        return "Applied Micro";
    case 0x51:
        return "Qualcomm";
    case 0x56:
        return "Marvell";
    case 0x69:
        return "Intel Corporation";
    default:
        return "Unknown impementer";
    }
}


/**
 * @brief returns the processor model string
 */
const char *armv8_processor_model_string(void)
{
    if (armv8_midr_el1_get_implementer() == 0x41) {
        /* ARM Processors */
        switch (armv8_midr_el1_get_partnum()) {
        case 0xD03:
            return "Cortex-A53";
        case 0xD04:
            return "Cortex-A35";
        case 0xD07:
            return "Cortex-A57";
        case 0xD09:
            return "Cortex-A73";
        case 0xD0A:
            return "Cortex-A75";
        default:
            break;
        }
    }
    return "Unknown processor";
}


/**
 * @brief returns the processor model string
 */
unsigned armv8_processor_revision_number(void)
{
    return armv8_midr_el1_get_revision();
}
