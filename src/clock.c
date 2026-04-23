#include "min_stm32f411.h"

void clock_init_pll(void) {
    /* 1. Enable HSE */
    RCC_CR &= ~(1 << 18);   // HSEBYP = 0
    RCC_CR |=  (1 << 16);   // HSEON

    /* Wait for HSE */
    while (!(RCC_CR & (1 << 17)));

    /* 2. Configure PLL */
    /*
     * PLLM = 25
     * PLLN = 336
     * PLLP = 4 (encoded as 01)
     * PLLSRC = HSE
     */

    RCC_PLLCFGR =
        (25 << 0)  |        // PLLM
        (336 << 6) |        // PLLN
        (1 << 16)  |        // PLLP = /4
        (1 << 22);          // PLLSRC = HSE

    /* 3. Enable PLL */
    RCC_CR |= (1 << 24);
    while (!(RCC_CR & (1 << 25)));

    /* Set Flash latency = 2 wait states */
    FLASH_ACR |= (2 << 0);
    
    /* 4. Switch system clock to PLL */
    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |=  (2 << 0);

    /* Wait until switched */
    while ((RCC_CFGR & (3 << 2)) != (2 << 2));
}

#include "min_stm32f411.h"

#define FLASH_ACR (*(volatile unsigned int *)0x40023C00)

void clock_init_pll_hsi(void) {
    /* 1. Ensure HSI is ON */
    RCC_CR |= (1 << 0);  // HSION
    while (!(RCC_CR & (1 << 1)));  // HSIRDY

    /* 2. Set Flash latency (84 MHz → 2 WS) */
    FLASH_ACR |= (2 << 0);

    /* 3. Configure PLL (HSI source) */
    RCC_PLLCFGR =
        (16 << 0)  |     // PLLM = 16
        (336 << 6) |     // PLLN = 336
        (1 << 16)  |     // PLLP = /4
        (0 << 22);       // PLLSRC = HSI

    /* 4. Enable PLL */
    RCC_CR |= (1 << 24);
    while (!(RCC_CR & (1 << 25)));

    /* 5. Switch SYSCLK → PLL */
    RCC_CFGR &= ~(3 << 0);
    RCC_CFGR |=  (2 << 0);

    while ((RCC_CFGR & (3 << 2)) != (2 << 2));
}

