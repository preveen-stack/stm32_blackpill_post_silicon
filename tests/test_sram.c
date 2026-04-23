#include "test.h"

int test_sram(void) {
    volatile unsigned int *ptr = (unsigned int *)0x20000000;

    for (int i = 0; i < 256; i++)
        ptr[i] = 0xAAAAAAAA;

    for (int i = 0; i < 256; i++)
        ASSERT(ptr[i] == 0xAAAAAAAA);

    for (int i = 0; i < 256; i++)
        ptr[i] = 0x55555555;

    for (int i = 0; i < 256; i++)
        ASSERT(ptr[i] == 0x55555555);

    return TEST_PASS;
}
