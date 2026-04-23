#include "min_stm32f411.h"

/* ================= RCC / FLASH ================= */
#define RCC_CFGR     (*(volatile unsigned int *)(RCC_BASE + 0x08))
#define RCC_PLLCFGR  (*(volatile unsigned int *)(RCC_BASE + 0x04))

/* ================= TIM2 ================= */
#define TIM2_CNT (*(volatile unsigned int *)(TIM2_BASE + 0x24))

/* ================= Helpers ================= */

/* Decode APB1 prescaler */
static unsigned int get_apb1_prescaler(void) {
    unsigned int ppre1 = (RCC_CFGR >> 10) & 0x7;

    switch (ppre1) {
        case 0: return 1;
        case 4: return 2;
        case 5: return 4;
        case 6: return 8;
        case 7: return 16;
        default: return 1;
    }
}

/* Decode AHB prescaler */
static unsigned int get_ahb_prescaler(void) {
    unsigned int hpre = (RCC_CFGR >> 4) & 0xF;

    switch (hpre) {
        case 0: return 1;
        case 8: return 2;
        case 9: return 4;
        case 10: return 8;
        case 11: return 16;
        case 12: return 64;
        case 13: return 128;
        case 14: return 256;
        case 15: return 512;
        default: return 1;
    }
}

/* ================= SYSCLK detection ================= */

unsigned int get_sysclk(void) {
    unsigned int sws = (RCC_CFGR >> 2) & 0x3;

    /* 00 = HSI */
    if (sws == 0) {
        return 16000000;
    }

    /* 01 = HSE (assume 25 MHz board) */
    if (sws == 1) {
        return 25000000;   // change to 8000000 if your board is 8 MHz
    }

    /* 10 = PLL */
    if (sws == 2) {
        unsigned int pllm = (RCC_PLLCFGR >> 0) & 0x3F;
        unsigned int plln = (RCC_PLLCFGR >> 6) & 0x1FF;
        unsigned int pllp = (((RCC_PLLCFGR >> 16) & 0x3) + 1) * 2;
        unsigned int pllsrc = (RCC_PLLCFGR >> 22) & 0x1;

        unsigned int vco_in;

        if (pllsrc == 0) {
            vco_in = 16000000 / pllm;  // HSI
        } else {
            vco_in = 25000000 / pllm;  // HSE (assumed)
        }

        unsigned int vco_out = vco_in * plln;

        return vco_out / pllp;
    }

    return 16000000; // fallback
}

/* ================= Timer clock ================= */

unsigned int get_tim2_clock(void) {
    unsigned int sysclk = get_sysclk();
    unsigned int ahb = get_ahb_prescaler();
    unsigned int apb1 = get_apb1_prescaler();

    unsigned int hclk = sysclk / ahb;
    unsigned int pclk1 = hclk / apb1;

    /* Timer clock doubling rule */
    if (apb1 == 1)
        return pclk1;
    else
        return pclk1 * 2;
}

/* ================= TIM2 driver ================= */

void tim2_init(void) {
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    unsigned int tim_clk = get_tim2_clock();

    /* Configure for 1 kHz tick (1 ms) */
    unsigned int psc = (tim_clk / 1000) - 1;

    TIM2_CR1 = 0;
    TIM2_PSC = psc;
    TIM2_ARR = 0xFFFFFFFF;

    TIM2_CR1 |= TIM_CR1_CEN;
}

/* ================= Delay ================= */

void delay_ms(unsigned int ms) {
    unsigned int start = TIM2_CNT;

    while ((TIM2_CNT - start) < ms);
}
