#include "clock.h"
#include "uart.h"
#include "pc13.h"
#include "timer.h"
#include "test.h"
#include "min_stm32f411.h" 
#include <stdio.h>

// Forward declarations
int test_sram(void);
int test_hse(void);
int test_timer(void);
int test_clock_measure(void);
int test_dsp(void);

test_t tests[] = {
    {"DSP Test", test_dsp},
    {"Clock Measure", test_clock_measure},
    {"Timer Test", test_timer},
    {"SRAM Test", test_sram},
    // {"HSE Test", test_hse}
};

#define RCC_BDCR (*(volatile unsigned int *)(RCC_BASE + 0x70))

#define RCC_BDCR_LSEON   (1 << 0)
#define RCC_BDCR_LSERDY  (1 << 1)

void test_lse(void) {
    uart_print("LSE: start\r\n");

    /* Enable write access to backup domain */
    RCC_APB1ENR |= (1 << 28);   // PWREN
    (*(volatile unsigned int *)0x40007000) |= (1 << 8); // PWR_CR DBP

    /* Enable LSE */
    RCC_BDCR |= RCC_BDCR_LSEON;

    /* Wait with timeout */
    int timeout = 1000000;
    while (!(RCC_BDCR & RCC_BDCR_LSERDY) && timeout--) {
        // wait
    }

    if (RCC_BDCR & RCC_BDCR_LSERDY) {
        uart_print("LSE: OK\r\n");
    } else {
        uart_print("LSE: FAIL\r\n");
    }
}

#define RCC_BDCR   (*(volatile unsigned int *)(RCC_BASE + 0x70))
#define RCC_APB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x40))

#define PWR_CR     (*(volatile unsigned int *)0x40007000)

#define RTC_BASE   0x40002800
#define RTC_TR     (*(volatile unsigned int *)(RTC_BASE + 0x00))
#define RTC_DR     (*(volatile unsigned int *)(RTC_BASE + 0x04))
#define RTC_ISR    (*(volatile unsigned int *)(RTC_BASE + 0x0C))
#define RTC_PRER   (*(volatile unsigned int *)(RTC_BASE + 0x10))
#define RTC_WPR    (*(volatile unsigned int *)(RTC_BASE + 0x24))

#define RCC_BDCR_LSEON   (1 << 0)
#define RCC_BDCR_LSERDY  (1 << 1)
#define RCC_BDCR_RTCEN   (1 << 15)
#define RCC_BDCR_RTCSEL_LSE (1 << 8)

/* Enable RTC with LSE */
void rtc_init_lse(void) {
    uart_print("RTC: init\r\n");

    /* Enable PWR clock + backup access */
    RCC_APB1ENR |= (1 << 28);
    PWR_CR |= (1 << 8);  // DBP

    /* Enable LSE if not already */
    RCC_BDCR |= RCC_BDCR_LSEON;
    while (!(RCC_BDCR & RCC_BDCR_LSERDY));

    /* Select LSE as RTC clock */
    RCC_BDCR &= ~(3 << 8);
    RCC_BDCR |= RCC_BDCR_RTCSEL_LSE;

    /* Enable RTC */
    RCC_BDCR |= RCC_BDCR_RTCEN;

    /* Disable write protection */
    RTC_WPR = 0xCA;
    RTC_WPR = 0x53;

    /* Enter init mode */
    RTC_ISR |= (1 << 7);  // INIT
    while (!(RTC_ISR & (1 << 6))); // INITF

    /*
     * LSE = 32768 Hz
     * Want 1 Hz:
     * async = 127
     * sync  = 255
     *
     * 32768 / ((127+1)*(255+1)) = 1 Hz
     */
    RTC_PRER = (127 << 16) | 255;

    /* Exit init mode */
    RTC_ISR &= ~(1 << 7);

    /* Enable write protection */
    RTC_WPR = 0xFF;

    uart_print("RTC: ready\r\n");
}

unsigned int rtc_get_seconds(void) {
    unsigned int tr = RTC_TR;

    unsigned int sec =
        ((tr >> 4) & 0x7) * 10 +
        (tr & 0xF);

    return sec;
}

int main1(void) {
    uart_init();
    pc13_init();

    while (1) {
        pc13_toggle();
        uart_print("TICK\r\n");

        for (volatile int i = 0; i < 1000000; i++);
    }
}

void delay_seconds(unsigned int sec) {
    unsigned int start = rtc_get_seconds();
    unsigned int elapsed = 0;

    while (elapsed < sec) {
        unsigned int now = rtc_get_seconds();

        if (now != start) {
            start = now;
            elapsed++;
        }
    }
}

int main(void) {
    clock_init_pll_hsi();

    pc13_init();
    uart_init();

    tim2_init();
    
    int sc = get_sysclk();
    int a = sc + 1;

    test_lse();
    rtc_init_lse();

    systick_init();
    //__asm__("cpsie i");

    dwt_init();

    

    //uart_print("\r\n=== POST SILICON TEST START ===\r\n");

    // run_tests(tests, sizeof(tests)/sizeof(test_t));

    // uart_print("=== TEST END ===\r\n");

    while (1) {
       pc13_toggle();
       uart_print("rtc seconds: ");
       int s = rtc_get_seconds();
       int s1 = s % 10;
       int s2 = s / 10;
       char a[3] = {};
       a[1] = 48 + s1; a[0] = 48 + s2;
       uart_print(a);
       uart_print("\r\n");
       //delay_ms_systick(1000);
       delay_seconds(1);
       uart_print("sysclock: ");
       uart2_write_uint(measure_sysclk_hz(4));
       uart_print("Hz\n");
    } 
}
