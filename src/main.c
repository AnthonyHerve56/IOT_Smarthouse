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
#include "../inc/temp_sensor_steam.h"
#include "../inc/button.h"
//Define
#define LED_YELLOW_NODE DT_ALIAS(led_yellow)
#define LCD_NODE DT_ALIAS(lcd)
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)
const k_tid_t thread2_id;

//Bouton 0
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button0_cb_data;

//Bouton 1
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});
static struct gpio_callback button1_cb_data;

//Main code
const struct gpio_dt_spec led_yellow_gpio = GPIO_DT_SPEC_GET_OR(LED_YELLOW_NODE, gpios, {0});
const struct i2c_dt_spec lcd_screen = I2C_DT_SPEC_GET(LCD_NODE);
void thread1(){ // Lecture de la température et de Steam toutes les 10 sec
	while(1){
		read_temp_and_steam_sensor();
		k_sleep(K_MSEC(10000));
	}

}
void thread2(){ // Lecture de la température et de Steam toutes les 10 sec
		while(1){
		printk("A la sieste");
		k_thread_suspend(thread2_id);
	}

}

void button0_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Gauche");

}
void button1_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Droite");
	k_thread_resume(thread2_id);
}
int main(void) {
	gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
	init_lcd(&lcd_screen);
	
	//Interrupt bouton 0

	int ret0 = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	if (ret0 != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret0, button0.port->name, button0.pin);
		return 0;
	}

	ret0 = gpio_pin_interrupt_configure_dt(&button0,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret0 != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret0, button0.port->name, button0.pin);
		return 0;
	}

	gpio_init_callback(&button0_cb_data, button0_pressed, BIT(button0.pin));
	gpio_add_callback(button0.port, &button0_cb_data);

	//Interrupt bouton 1

	int ret1 = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	if (ret1 != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret1, button1.port->name, button1.pin);
		return 0;
	}

	ret1 = gpio_pin_interrupt_configure_dt(&button1,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret1 != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret1, button1.port->name, button1.pin);
		return 0;
	}

	gpio_init_callback(&button1_cb_data, button1_pressed, BIT(button1.pin));
	gpio_add_callback(button1.port, &button1_cb_data);






}



K_THREAD_DEFINE(thread1_id,1024,thread1,NULL,NULL,NULL,9,0,0);
K_THREAD_DEFINE(thread2_id,1024,thread2,NULL,NULL,NULL,9,0,0);