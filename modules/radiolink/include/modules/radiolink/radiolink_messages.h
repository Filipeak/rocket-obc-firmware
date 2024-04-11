#ifndef _RADIOLINK_MESSAGES_H
#define _RADIOLINK_MESSAGES_H

/**
 * REF: https://mavlink.io/en/messages/common.html
 */

#include "modules/maths/vector.h"

/**
 * @brief List of available RadioLink messsages types
 */
typedef enum radiolink_message_type
{
    RADIOLINK_MESSAGE_SENSORS = 0,
} radiolink_message_type_t;

/**
 * @brief RadioLink Sensor Frame
 */
typedef struct radiolink_sensor_frame
{
    vec3_t pos;
    vec3_t gyro;
    float lat;
    float lon;
    float alt;
    float velocity;
    float batteryVoltage;
    uint8_t batteryPercentage;
    float pressure;
    float temperature;
} radiolink_sensor_frame_t;

#endif