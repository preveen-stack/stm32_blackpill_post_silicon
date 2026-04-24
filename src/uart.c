#include "min_stm32f411.h"

/* Add this in header if missing */
#define GPIOA_AFRH (*(volatile unsigned int *)(GPIOA_BASE + 0x24))

void uart_init(void) {
    RCC_AHB1ENR |= (1 << 0);   // GPIOA
    RCC_APB2ENR |= (1 << 4);   // USART1

    /* PA9, PA10 AF mode */
    GPIOA_MODER &= ~(3 << (9*2));
    GPIOA_MODER |=  (2 << (9*2));

    GPIOA_MODER &= ~(3 << (10*2));
    GPIOA_MODER |=  (2 << (10*2));

    /* AFRH */
    GPIOA_AFRH &= ~(0xFF << 4);
    GPIOA_AFRH |=  (0x77 << 4);  // AF7 for both

    /* Speed high */
    GPIOA_OSPEEDR |= (3 << (9*2)) | (3 << (10*2));

    /* USART */
    // USART1_BRR = 0x0683; // 9600 @ 16 MHz
    // USART1_BRR = 0x008B; // 115200 @ 16 MHz
    USART1_BRR = 0x0045; // 230400
    // USART1_BRR = 0x0011; // 921600 @ 16 MHz
    USART1_BRR = 0x02D9; // 115200 @ 84MHz
    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE
}

void uart_print(const char *s) {
    while (*s) {
        while (!(USART1_SR & USART_SR_TXE));
        USART1_DR = *s++;
    }
}

void uart2_write_char(char c) {
    while (!(USART1_SR & USART_SR_TXE));
    USART1_DR = c;
}

void uart2_write_str(const char *s) {
    while (*s) uart2_write_char(*s++);
}

void uart2_write_uint(uint32_t v) {
    char buf[16];
    int i = 0;

    if (v == 0) {
        uart2_write_char('0');
        return;
    }

    while (v > 0) {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }

    while (i--) uart2_write_char(buf[i]);
}
