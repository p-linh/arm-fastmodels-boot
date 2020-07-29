/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <types.h>

/**
 * @brief checks if the mmu is enabled or not
 *
 * @returns TRUE if the mmu is enabled, FALSE otherwise
 */
bool mmu_enabled(void);

/**
 * @brief configures and enables the MMU
 */
void mmu_configure_and_enable(void);

/**
 * @brief configures the MMU
 */
void mmu_configure(void);

/**
 * @brief enables the MMU
 */
void mmu_enable(void);

/**
 * @brief enables the MMU
 */
void mmu_disable(void);

/**
 * @brief writes the current ttbr0
 *
 * @param addr  the base address of the page table
 */
void mmu_write_ttbr0(uint64_t addr);


/**
 * @brief reads the current ttbr0
 *
 * @returns base address of the root level page table
 */
uint64_t mmu_read_ttbr0(void);


/**
 * @brief configures the memory attributes
 *
 * @param the attributes to set
 */
void mmu_configure_memory_attributes(uint64_t attrs);


/**
 * @brief configures the translation control register
 */
void mmu_configure_translation_scheme(void);


/**
 * @brief invalidates the TLB
 */
void mmu_invalidate_tlb();