#include "uart.h"
#include "test.h"

// Forward declarations
int test_sram(void);

test_t tests[] = {
    {"SRAM Test", test_sram},
};

int main(void) {
    uart_init();

    uart_print("\r\n=== POST SILICON TEST START ===\r\n");

    run_tests(tests, sizeof(tests)/sizeof(test_t));

    uart_print("=== TEST END ===\r\n");

    while (1);
}
