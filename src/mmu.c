/*
 * Arm FastModels Hello World Boot
 *
 * Copyright 2020 Reto Achermann
 * SPDX-License-Identifier: GPL-3.0
 */

#include <armv8.h>
#include <mmu.h>
#include <logging.h>
#include <serial.h>


///< symbol of the exception vector start
extern long __pagetables_start;

///< symbol of the exception vector end
extern long __pagetables_end;


/**
 * @brief checks if the mmu is enabled or not
 *
 * @returns TRUE if the mmu is enabled, FALSE otherwise
 */
bool mmu_enabled(void)
{
    switch (armv8_get_current_el()) {
    case 3:
        return armv8_sctlr_el3_get_m();
        break;
    case 2:
    case 1:
    default:
        WARN("checking MMU enabled stated on this el not implemented\n");
        return false;
    }
}


/**
 * @brief writes the current ttbr0
 *
 * @param addr  the base address of the page table
 */
void mmu_write_ttbr0(uint64_t addr)
{
    switch (armv8_get_current_el()) {
    case 3:
        return armv8_ttbr0_el3_write(addr);
        break;
    case 2:
    case 1:
    default:
        WARN("writing to ttbr0 on this el not implemented\n");
    }
}

/**
 * @brief reads the current ttbr0
 *
 * @returns base address of the root level page table
 */
uint64_t mmu_read_ttbr0(void)
{
    switch (armv8_get_current_el()) {
    case 3:
        return armv8_ttbr0_el3_read();
        break;
    case 2:
    case 1:
    default:
        WARN("reading to ttbr0 on this el not implemented\n");
        return (lpaddr_t)-1;
    }
}


/**
 * @brief configures the memory attributes
 *
 * @param the attributes to set
 */
void mmu_configure_memory_attributes(uint64_t attrs)
{
    switch (armv8_get_current_el()) {
    case 3:
        armv8_mair_el3_write(attrs);
        break;
    case 2:
    case 1:
    default:
        WARN("reading to ttbr0 on this el not implemented\n");
    }
}

#define CONFIG_TCR_TTBR0_TRANSLATION_SIZE 16

#define CONFIG_TCR_WRITE_BACK_ALLOCATE_CACHE 0x1

#define CONFIG_TCR_INNER_SHARABLE 0x3

#define CONFIG_TCR_GRANULE_4K 0x0

#define CONFIG_TCR_PHYSICAL_ADDRESS_SIZE_48_BITS 0x5

#define CONFIG_TCR_TOP_BYTE_USED 0b0
#define CONFIG_TCR_TOP_BYTE_IGNORED 0x1

/**
 * @brief configures the translation control register
 */
void mmu_configure_translation_scheme(void)
{
    switch (armv8_get_current_el()) {
    case 3: {
        // 48b user VA
        uint64_t val = CONFIG_TCR_TTBR0_TRANSLATION_SIZE;

        // Inner cacheability attribute for memory associated with translation table walks
        val |= (CONFIG_TCR_WRITE_BACK_ALLOCATE_CACHE << 8);

        // Outer cacheability attribute for memory associated with translation table walks
        val |= (CONFIG_TCR_WRITE_BACK_ALLOCATE_CACHE << 10);

        // Shareability attribute for memory associated with translation table walks
        val |= (CONFIG_TCR_INNER_SHARABLE << 12);

        // Granule size for the TTBR0_EL3.
        val |= (CONFIG_TCR_GRANULE_4K << 14);

        // Physical Address Size.
        val |= (CONFIG_TCR_PHYSICAL_ADDRESS_SIZE_48_BITS << 16);

        /// Top Byte Ignored?
        val |= (CONFIG_TCR_TOP_BYTE_USED << 20);

        isb();
        armv8_tcr_el3_write(val);
    } break;
    case 2:
    case 1:
    default:
        WARN("configuring the TCR on this el not implemented\n");
    }
}


/**
 * @brief invalidates the TLB
 */
void mmu_invalidate_tlb()
{
    switch (armv8_get_current_el()) {
    case 3:
        __asm volatile("tlbi    alle3");
        dmb();
        isb();
        break;
    case 2:
        __asm volatile("tlbi    alle2");
        dmb();
        isb();
        break;
    case 1:
        __asm volatile("tlbi    vmalle1");
        dmb();
        isb();
        break;
    default:
        return;
    }
}


/**
 * @brief configures the MMU
 */
void mmu_configure(void)
{
    if (!mmu_enabled()) {
        MSG("Configuring MMU with TTBR 0x%" PRIx64 "\n", (lpaddr_t)&__pagetables_start);

        /*
         * configure memory attributes
         *   - attr0 = Normal Memory, Inner Write-back non transient  (0xff)
         *   - attr1 = Device-nGnRnE memory                           (0x00)
         */
        mmu_configure_memory_attributes(0x00ff);

        /* configure the translation scheme */
        mmu_configure_translation_scheme();

        /* write the register */
        mmu_write_ttbr0((lpaddr_t)&__pagetables_start);

        MSG("MMU configured and enabled.\n");

    } else {
        MSG("MMU preconfigured (TTBR 0x%" PRIx64 ")\n", mmu_read_ttbr0());
    }
}

/**
 * @brief enables the MMU
 */
void mmu_enable(void)
{
    armv8_sctlr_el3_set_m(1);
}

/**
 * @brief enables the MMU
 */
void mmu_disable(void)
{
    armv8_sctlr_el3_set_m(0);
}

/**
 * @brief configures and enables the MMU
 */
void mmu_configure_and_enable(void)
{
    serial_console_putchar('O');
    serial_console_putchar('\n');
    mmu_configure();
    serial_console_putchar('P');
    serial_console_putchar('\n');
    mmu_enable();
    serial_console_putchar('Q');
    serial_console_putchar('\n');
}
