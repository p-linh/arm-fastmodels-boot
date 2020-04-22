/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */


#ifndef __CONFIG_H_
#define __CONFIG_H_

///< the MPID of the BSP processor
#define CONFIG_BSP_MIPD 0


#define CONFIG_SERIAL_BASE 0x1234;


///< this is the serial console port
#define CONFIG_SERIAL_CONSOLE_PORT 0

///< this is the serial debug port
#define CONFIG_SERIAL_DEBUG_PORT 1

///< this is the number of serial ports
#define CONFIG_SERIAL_NUM_PORTS 2


#endif /* __CONFIG_H_ */