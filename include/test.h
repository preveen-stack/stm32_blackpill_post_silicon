#ifndef TEST_H
#define TEST_H

typedef int (*test_fn_t)(void);

typedef struct {
    const char *name;
    test_fn_t fn;
} test_t;

#define TEST_PASS 0
#define TEST_FAIL 1

#define ASSERT(cond) \
    do { if (!(cond)) return TEST_FAIL; } while (0)

void run_tests(test_t *tests, int count);

#endif
