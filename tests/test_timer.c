#include "min_stm32f411.h"
#include "test.h"
#include "uart.h"

int test_timer(void) {
    uart_print("TIMER: start\r\n");

    /* Enable TIM2 clock */
    RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Reset config */
    TIM2_CR1 = 0;
    TIM2_SR  = 0;

    /*
     * Assuming 16 MHz clock (HSI):
     * PSC = 16000-1 → 1 kHz (1 ms tick)
     * ARR = 1000-1  → 1 second overflow
     */

    TIM2_PSC = 16000 - 1;
    TIM2_ARR = 1000 - 1;

    /* Start timer */
    TIM2_CR1 |= TIM_CR1_CEN;

    /* Wait for update event */
    for (volatile int i = 0; i < 30000000; i++) {
        if (TIM2_SR & TIM_SR_UIF) {
            uart_print("TIMER: OK\r\n");

            /* Clear flag */
            TIM2_SR &= ~TIM_SR_UIF;

            return TEST_PASS;
        }
    }

    uart_print("TIMER: TIMEOUT\r\n");
    return TEST_FAIL;
}
