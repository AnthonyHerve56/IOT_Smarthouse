/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Include
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "../inc/lcd_screen_i2c.h"
#include <zephyr/drivers/i2c.h>

//Define
#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_NODE DT_ALIAS(lcd)

//Main code
const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen = I2C_DT_SPEC_GET(LCD_NODE);
const struct device *const dht11 = DEVICE_DT_GET_ONE(aosong_dht); //Temperature
int main(void) {
	gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
	init_lcd(&lcd_screen);

	//int ret = sensor_sample_fetch(dht11);
	//printk("%d",ret);
}

