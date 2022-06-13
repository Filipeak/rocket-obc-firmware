#include "pico/stdlib.h"
#include <string.h>
#include <stdlib.h>
#include "cryptography.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"

void start();
void initialize();
void loop();

int main()
{
    start();
    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");
 
    char buffer[] = "Hello World!";
    char buffer2[] = "Hello World!";
    char key[] = {0x30, 0x3A};
    char key2[] = {0x48, 0x7D, 0x25};
    char key3[] = {0x21, 0x2F, 0x5B, 0x7B};

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key2, sizeof(key2) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key2, sizeof(key2) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key3, sizeof(key3) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key3, sizeof(key3) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    char buffer3[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    parity_data_t parityData = {0};

    const char EXPECTED_PARITY_ROWS[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c};
    calculateParityRows(buffer3, sizeof(buffer3) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_ROWS, sizeof(EXPECTED_PARITY_ROWS) / sizeof(char)) == 0);

    clearParity(&parityData);

    const char EXPECTED_PARITY_COLUMNS[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    calculateParityColumns(buffer3, sizeof(buffer3) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_COLUMNS, sizeof(EXPECTED_PARITY_COLUMNS) / sizeof(char)) == 0);

    const char EXPECTED_PARITY[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    calculateParity(buffer3, sizeof(buffer3) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY, sizeof(EXPECTED_PARITY) / sizeof(char)) == 0);

    clearParity(&parityData);

    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();
}