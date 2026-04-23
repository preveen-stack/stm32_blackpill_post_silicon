#include "min_stm32f411.h"

void pc13_init(void) {
    /* Enable GPIOC clock */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* Set PC13 as output */
    GPIOC_MODER &= ~(3 << (13 * 2));
    GPIOC_MODER |=  (1 << (13 * 2));
}

void pc13_toggle(void) {
    GPIOC_ODR ^= (1 << 13);
}
