#include "test.h"
#include "uart.h"

/* Force use of DSP instructions */
static int dsp_mac_test(void) {
    int a = 1000;
    int b = 2000;
    int acc = 500;

    /* Should compile to MLA */
    acc += a * b;

    return (acc == 2000500);
}

/* Saturating arithmetic (QADD) */
static int dsp_sat_test(void) {
    int a = 0x7FFFFFF0;
    int b = 0x100;

    int result;

    /* __builtin_arm_qadd → QADD instruction */
    result = __builtin_arm_qadd(a, b);

    return (result == 0x7FFFFFFF);
}

/* SIMD-style dual 16-bit multiply-accumulate */
static int dsp_simd_test(void) {
    int a = 0x00030004;   // two 16-bit values: 3,4
    int b = 0x00050006;   // 5,6

    int result;

    /* SMLAD: (3*5 + 4*6) = 15 + 24 = 39 */
    result = __builtin_arm_smlad(a, b, 0);

    return (result == 39);
}

int test_dsp(void) {
    uart_print("DSP: start\r\n");

    if (!dsp_mac_test()) {
        uart_print("DSP: MAC FAIL\r\n");
        return TEST_FAIL;
    }

    if (!dsp_sat_test()) {
        uart_print("DSP: SAT FAIL\r\n");
        return TEST_FAIL;
    }

    if (!dsp_simd_test()) {
        uart_print("DSP: SIMD FAIL\r\n");
        return TEST_FAIL;
    }

    uart_print("DSP: OK\r\n");
    return TEST_PASS;
}
