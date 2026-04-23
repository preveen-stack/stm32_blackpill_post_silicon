#ifndef MIN_STM32F411_H
#define MIN_STM32F411_H

#define PERIPH_BASE     0x40000000UL
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE (PERIPH_BASE + 0x00000000UL)

/* RCC */
#define RCC_BASE        (AHB1PERIPH_BASE + 0x3800UL)
#define RCC_AHB1ENR     (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile unsigned int *)(RCC_BASE + 0x40))

/* GPIOA */
#define GPIOA_BASE      (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOA_MODER     (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL      (*(volatile unsigned int *)(GPIOA_BASE + 0x20))

/* USART2 */
#define USART2_BASE     (APB1PERIPH_BASE + 0x4400UL)
#define USART2_SR       (*(volatile unsigned int *)(USART2_BASE + 0x00))
#define USART2_DR       (*(volatile unsigned int *)(USART2_BASE + 0x04))
#define USART2_BRR      (*(volatile unsigned int *)(USART2_BASE + 0x08))
#define USART2_CR1      (*(volatile unsigned int *)(USART2_BASE + 0x0C))

/* Bit definitions */
#define RCC_AHB1ENR_GPIOAEN (1 << 0)
#define RCC_APB1ENR_USART2EN (1 << 17)

#define USART_SR_TXE    (1 << 7)
#define USART_CR1_TE    (1 << 3)
#define USART_CR1_UE    (1 << 13)

#endif
