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
bool flag_thread_2=false;



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
		printk("flag_thread_2 = %d",flag_thread_2);
		if(flag_thread_2)
		{
			printk("Je suis réveille\n");
			write_lcd(&lcd_screen, "                ", LCD_LINE_1);
			write_lcd(&lcd_screen, "                ", LCD_LINE_2);
			write_lcd(&lcd_screen, "Yipi", LCD_LINE_1);
			flag_thread_2=false;
			
		}
		k_thread_suspend(thread2_id);
		
	}

}

/*
Le code des boutons a été récupéré depuis ce dépot GIT : 
https://github.com/ubieda/zephyr_button_debouncing/tree/master/src
*/

static int helper_button_evt_str(enum button_evt evt)
{
	switch (evt) {
	case BUTTON_EVT_PRESSED:
		return 1;
	case BUTTON_EVT_RELEASED:
		return 0;
	default:
		return -1;
	}
}

void button0_event_handler(enum button_evt evt)
{
	
	if(helper_button_evt_str(evt))
	{
		printk("Plop\n");
		flag_thread_2=true;
		k_thread_resume(thread2_id);
	}
		
	
	
}
void button1_event_handler(enum button_evt evt)
{
	printk("Button event: %d\n", helper_button_evt_str(evt));
	
}


int main(void) {
	gpio_pin_configure_dt(&led_yellow_gpio, GPIO_OUTPUT_HIGH);
	init_lcd(&lcd_screen);
	

	int err0 = -1;

	printk("Button Debouncing Sample!\n");

	err0 = button0_init(button0_event_handler);
	if (err0) {
		printk("Button Init failed: %d\n", err0);
		return 0;
	}

	int err1 = -1;

	printk("Button Debouncing Sample!\n");

	err1 = button1_init(button1_event_handler);
	if (err1) {
		printk("Button Init failed: %d\n", err1);
		return 0;
	}







}



K_THREAD_DEFINE(thread1_id,1024,thread1,NULL,NULL,NULL,9,0,0);
K_THREAD_DEFINE(thread2_id,1024,thread2,NULL,NULL,NULL,9,0,0);