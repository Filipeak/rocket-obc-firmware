#ifndef _GPS_DEFINES_H
#define _GPS_DEFINES_H

#include <stdbool.h>
#include <stddef.h>

#define GPS_NMEA_SENTENCE_LENGTH 83 /** NMEA Sentence Length */

/**
 * @brief Structure for holding gps nmea sentence
 */
typedef struct gps_nmea_sentence
{
    char data[GPS_NMEA_SENTENCE_LENGTH]; /** Sentence string with null terminator */
    size_t currentIndex;                 /** Current index of data */
    bool isFinished;                     /** Is sentence finished */
} gps_nmea_sentence_t;

#endif