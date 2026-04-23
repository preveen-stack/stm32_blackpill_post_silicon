#include "min_stm32f411.h"

void uart_init(void) {
    /* Enable clocks */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN;

    /* PA2 → Alternate Function (AF7 for USART2) */
    GPIOA_MODER &= ~(3 << (2 * 2));
    GPIOA_MODER |=  (2 << (2 * 2));

    /* Set AF7 (USART2) */
    GPIOA_AFRL &= ~(0xF << (4 * 2));
    GPIOA_AFRL |=  (7 << (4 * 2));

    /* Baud rate (9600 @ 16 MHz) */
    USART2_BRR = 0x0683;

    /* Enable UART */
    USART2_CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_print(const char *s) {
    while (*s) {
        while (!(USART2_SR & USART_SR_TXE));
        USART2_DR = *s++;
    }
}
