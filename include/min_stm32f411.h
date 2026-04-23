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

/* USART1 (on APB2) */
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)

#define RCC_APB2ENR     (*(volatile unsigned int *)(RCC_BASE + 0x44))
#define RCC_APB2ENR_USART1EN (1 << 4)

#define USART1_BASE     (APB2PERIPH_BASE + 0x1000UL)
#define USART1_SR       (*(volatile unsigned int *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile unsigned int *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile unsigned int *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile unsigned int *)(USART1_BASE + 0x0C))

/* USART2 */
#define USART2_BASE     (APB1PERIPH_BASE + 0x4400UL)
#define USART2_SR       (*(volatile unsigned int *)(USART2_BASE + 0x00))
#define USART2_DR       (*(volatile unsigned int *)(USART2_BASE + 0x04))
#define USART2_BRR      (*(volatile unsigned int *)(USART2_BASE + 0x08))
#define USART2_CR1      (*(volatile unsigned int *)(USART2_BASE + 0x0C))

/* GPIOC base */
#define GPIOC_BASE      (AHB1PERIPH_BASE + 0x0800UL)

#define GPIOC_MODER     (*(volatile unsigned int *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR       (*(volatile unsigned int *)(GPIOC_BASE + 0x14))

/* Enable bit */
#define RCC_AHB1ENR_GPIOCEN (1 << 2)

/* Bit definitions */
#define RCC_AHB1ENR_GPIOAEN (1 << 0)
#define RCC_APB1ENR_USART2EN (1 << 17)

#define USART_SR_TXE    (1 << 7)
#define USART_CR1_TE    (1 << 3)
#define USART_CR1_UE    (1 << 13)

#define GPIOA_OSPEEDR (*(volatile unsigned int *)(GPIOA_BASE + 0x08))
#define GPIOA_OTYPER  (*(volatile unsigned int *)(GPIOA_BASE + 0x04))
#define GPIOA_PUPDR   (*(volatile unsigned int *)(GPIOA_BASE + 0x0C))
#endif
