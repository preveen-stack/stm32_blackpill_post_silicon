#include "min_stm32f411.h"
#include "test.h"
#include "uart.h"

static void delay_loop(volatile unsigned int count) {
    while (count--) {
        __asm__("nop");
    }
}

int test_clock_measure(void) {
    uart_print("CLK: measuring...\r\n");

    /* Enable TIM2 */
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Reset timer */
    TIM2_CR1 = 0;
    TIM2_CNT = 0;

    /*
     * Prescaler = 0 → timer runs at full APB1 timer clock
     * On STM32F4:
     * If APB1 prescaler = 1 → TIM clk = PCLK1
     * If APB1 prescaler >1 → TIM clk = 2 × PCLK1
     *
     * Default reset: APB1 prescaler = 1 → TIM2 ≈ SYSCLK
     */

    TIM2_PSC = 0;
    TIM2_CR1 |= TIM_CR1_CEN;

    /* Capture start */
    unsigned int start = TIM2_CNT;

    /* Delay loop (adjust if needed) */
    delay_loop(5000000);

    /* Capture end */
    unsigned int end = TIM2_CNT;

    unsigned int delta = end - start;

    uart_print("CLK ticks: ");

    /* crude print */
    char buf[20];
    int i = 0;

    unsigned int temp = delta;
    if (temp == 0) {
        uart_print("0\r\n");
        return TEST_FAIL;
    }

    /* convert to string */
    char rev[20];
    int j = 0;
    while (temp > 0) {
        rev[j++] = '0' + (temp % 10);
        temp /= 10;
    }
    while (j--) {
        buf[i++] = rev[j];
    }
    buf[i++] = '\r';
    buf[i++] = '\n';
    buf[i] = 0;

    uart_print(buf);

    /*
     * Rough interpretation:
     *
     * If ~16M system → smaller delta
     * If ~84M system → ~5x larger delta
     */

    if (delta > 20000000) {
        uart_print("CLK: looks like ~84MHz\r\n");
    } else {
        uart_print("CLK: looks like ~16MHz\r\n");
    }

    return TEST_PASS;
}
