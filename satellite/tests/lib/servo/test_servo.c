#include "test_servo.h"
#include "pinout.h"
#include "myhardware/servo.h"

MY_TEST_INIT_FUNC(SERVO_TEST_NAME)
{
    ServoData data = {0};

    MY_ASSERT(FUNCSUCCESS(servoInit(MG995_PIN, &data)));

    MY_TEST_END();
}