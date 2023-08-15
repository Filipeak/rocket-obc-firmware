#pragma once

#include <obc/api.h>

/**
 * @brief Exponential filter structure
 */
typedef struct ExpFilterData
{
    FLOAT a;
    FLOAT v;
    BOOL vInit;
} ExpFilterData;

/**
 * @brief Initialize exponential filter (smoothing)
 *
 * @param data Filter data pointer
 * @param alpha Alpha value
 */
VOID filterExpInit(ExpFilterData *data, FLOAT a);

/**
 * @brief Exponential filter (smoothing)
 *
 * @param data Filter data pointer
 * @param v Value to filter and the output value
 */
VOID filterExpUpdate(ExpFilterData *data, FLOAT *v);