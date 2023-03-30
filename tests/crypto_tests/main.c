#include "pico/stdlib.h"
#include "test_framework.h"
#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"
#include "test_cryptography.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_LOG_CORE_INFO("Running tests...");

    MY_TESTS_BEGIN();
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 3);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 4);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 5);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 6);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 7);
    MY_TESTS_END();

    MY_LOG_CORE_INFO("Tests finished!");

    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}