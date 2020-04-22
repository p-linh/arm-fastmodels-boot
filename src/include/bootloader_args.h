/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef __BOOTLOADER_ARGS_H_
#define __BOOTLOADER_ARGS_H_ 1

#include <offsets.h>
#include <types.h>
#include <assert.h>
#include <config.h>


///< this is the size of the bootloader arguemtns
#define BOOTLOADER_ARGS_SIZE (2UL * sizeof(uint64_t))

///< this is the bootloader arguments magic value
#define BOOTLOADER_ARGS_MAGIC 0x3001300130013001UL

/**
 *  @brief This is the arguments struct for the bootloader
 */
struct bootloader_args {
    ///< the magic value
    uint64_t magic;

    ///< the processor to be started
    uint64_t mpid;

    ///< the base addresses of the serials
    lpaddr_t serial[CONFIG_SERIAL_NUM_PORTS];
};

STATIC_ASSERT(sizeof(struct bootloader_args) == BOOTLOADER_ARGS_SIZE);


#define BOOTLOADER_ARGS_INIT(_mpid, _serial)                                                      \
    (struct bootloader_args)                                                                      \
    {                                                                                             \
        .magic = 0, .mpid = _mpid, .serial = { _serial, _serial }                                 \
    }

#endif /* __BOOTLOADER_ARGS_H_ */