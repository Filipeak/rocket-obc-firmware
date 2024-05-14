#ifndef _GEO_UTILS_H
#define _GEO_UTILS_H

/**
 * @brief Calculate the height (up to 11km!) from the barometric formula
 *
 * @param pressure Pressure in Pascals
 * @param temperature Temperature in Celsius
 * @return Height in meters
 */
float height_from_baro_formula(float pressure, float temperature);

#endif