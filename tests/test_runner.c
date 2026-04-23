 #include "test.h"
#include "uart.h"

void run_tests(test_t *tests, int count) {
    for (int i = 0; i < count; i++) {
        uart_print("RUN: ");
        uart_print(tests[i].name);
        uart_print("\r\n");

        int result = tests[i].fn();

        if (result == TEST_PASS) {
            uart_print("PASS\r\n");
        } else {
            uart_print("FAIL\r\n");
        }
    }
}
