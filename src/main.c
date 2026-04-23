#include "uart.h"
#include "pc13.h"
#include "test.h"

// Forward declarations
int test_sram(void);

test_t tests[] = {
    {"SRAM Test", test_sram},
};
int main1(void) {
    uart_init();
    pc13_init();

    while (1) {
        pc13_toggle();
        uart_print("TICK\r\n");

        for (volatile int i = 0; i < 1000000; i++);
    }
}

int main(void) {
    pc13_init();
    uart_init();

    uart_print("\r\n=== POST SILICON TEST START ===\r\n");

    run_tests(tests, sizeof(tests)/sizeof(test_t));

    uart_print("=== TEST END ===\r\n");

    while (1) pc13_toggle();
}
