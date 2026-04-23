#include "min_stm32f411.h"
#include "test.h"
#include "uart.h"

int test_hse(void) {
    uart_print("HSE: start\r\n");

    RCC_CR &= ~RCC_CR_HSEBYP;
    RCC_CR |= RCC_CR_HSEON;

    for (volatile int i = 0; i < 5000000; i++) {
        if (RCC_CR & RCC_CR_HSERDY) {
            uart_print("HSE: READY\r\n");
            return TEST_PASS;
        }
    }

    uart_print("HSE: TIMEOUT\r\n");
    return TEST_FAIL;
}
