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
#include <zephyr/drivers/adc.h>

//Define
#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_NODE DT_ALIAS(lcd)
#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

//Main code
const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen = I2C_DT_SPEC_GET(LCD_NODE);
const struct device *const dht11 = DEVICE_DT_GET_ONE(aosong_dht); //Temperature
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

int main(void) {
	gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
	init_lcd(&lcd_screen);
	int ret;
	struct sensor_value temp_value;

	if (!device_is_ready(dht11)) {
		printk("sensor: device not ready.\n");
		return 0;
	}

	printk("Polling magnetometer data from %s.\n", dht11->name);

	while (1) {
		ret = sensor_sample_fetch(dht11);
		if (ret) {
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return 0;
		}
		ret = sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP, &temp_value);
		printf("ambiant temp = ( %f )\n",
		       sensor_value_to_double(&temp_value));

		k_sleep(K_MSEC(10000));
	}
	return 0;
}

