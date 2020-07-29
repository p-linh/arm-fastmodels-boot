/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */


/**
 * @brief cleans the instruction cache
 */
void cachectrl_clean_icache();

/**
 * @brief invalidates the instruction cache
 */
void cachectrl_invalidate_icache();

/**
 * @brief enables the instruction cache
 */
void cachectrl_enable_icache();

/**
 * @brief disable the instruction cache
 */
void cachectrl_disable_icache();


/**
 * @brief invalidates the data cache
 */
void cachectrl_invalidate_dcache();

/**
 * @brief cleans the data cache
 */
void cachectrl_clean_dcache();

/**
 * @brief enables the data cache
 */
void cachectrl_enable_dcache();

/**
 * @brief disables the data cache
 */
void cachectrl_disable_dcache();


/**
 * @brief enables the data and instruction caches
 */
void cachectrl_enable_all_caches();

/**
 * @brief disables the data and instruction caches
 */
void cachectrl_disable_all_caches();
