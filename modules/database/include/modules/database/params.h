#ifndef _PARAMS_H
#define _PARAMS_H

#include "hal/gpio_driver.h"
#include "hal/time_tracker.h"
#include <stdint.h>
#include <stdbool.h>

#define DB_PARAMS_MAGIC 0x9B /** Magic byte for params */

/**
 * @brief Params configuration structure
 */
typedef struct db_params
{
    uint16_t mainParachuteHeight;
    uint16_t launcherHeight;
    uint16_t parachuteErrorSpeed;
    msec_t ignSecondDelay;
} db_params_t;

/**
 * @brief Params frame structure
 */
typedef struct db_params_frame
{
    uint8_t magic;
    db_params_t params;
} db_params_frame_t;

/**
 * @brief Get params
 *
 * @param params Params pointer
 * @return true if success
 */
bool params_get(db_params_t *params);

/**
 * @brief Save params
 *
 * @param params Params pointer
 * @return true if success
 */
bool params_save(db_params_t *params);

#endif