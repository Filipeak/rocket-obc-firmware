#ifndef _SIMPLE_LED_DRIVER_H
#define _SIMPLE_LED_DRIVER_H

#include "hal/gpio_driver.h"
#include <stdbool.h>

/**
 * @brief Initialize LED on pin
 *
 * @param pin Pin to initialize
 * @return Result Code
 */
void led_init(hal_pin_number_t pin);

/**
 * @brief Set LED on or off
 *
 * @param pin Pin to change state for
 * @param state State of pin - on or off
 * @return Result Code
 */
void led_set_state(hal_pin_number_t pin, bool state);

#endif