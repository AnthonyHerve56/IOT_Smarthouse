/**
 * Author: Vivien Debuchy
 * Simple library for LCD-1602 I2C
 * Based on https://github.com/sterlingbeason/LCD-1602-I2C
 */

#ifndef TEMP_STEAM_SENSOR
#define TEMP_STEAM_SENSOR

struct i2c_dt_spec;



void read_temp_and_steam_sensor();

#endif // TEMP_STEAM_SENSOR