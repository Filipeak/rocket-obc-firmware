#include "drivers/barometer/bmp581_driver.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

#define I2C 0
#define SDA 0
#define SCL 1
#define ADDRESS 0x00

int main()
{
    boardInit(5000);

    i2cInitAll(I2C, 400 * 1000);

    BMP581Config bmp581Config;

    bmp581InitI2C(&bmp581Config, I2C, SDA, SCL);
    bmp581SetMode(&bmp581Config, BMP581_MODE_CONTINUOUS);
    bmp581SetODR_OSR(&bmp581Config, BMP581_OSR_1, BMP581_OSR_1, BMP581_ODR_240);

    while (TRUE)
    {
        BMP581Data data;
        bmp581Read(&bmp581Config, &data);

        printf("Temperature: %f C\n", data.temperature);
        printf("Pressure: %f Pa\n", data.pressure);

        sleepMiliseconds(4);
    }

    return 0;
}