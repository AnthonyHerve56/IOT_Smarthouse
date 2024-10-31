/*
Ce code a été récupéré depuis ce dépot GIT : 
https://github.com/ubieda/zephyr_button_debouncing/tree/master/src
*/



#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "../inc/button.h"

#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)
//Bouton 0
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button0_cb_data;
static button_event_handler_t user0_cb;

//Bouton 1
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios,
							      {0});
static struct gpio_callback button1_cb_data;
static button_event_handler_t user1_cb;


//Button 0
static void cooldown0_expired(struct k_work *work)
{
    ARG_UNUSED(work);

    int val = gpio_pin_get_dt(&button0);
    enum button_evt evt = val ? BUTTON_EVT_RELEASED : BUTTON_EVT_PRESSED;
    if (user0_cb) {
        user0_cb(evt);
    }
}

static K_WORK_DELAYABLE_DEFINE(cooldown0_work, cooldown0_expired);

void button0_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
    k_work_reschedule(&cooldown0_work, K_MSEC(15));
}

int button0_init(button_event_handler_t handler)
{
    int err = -1;

    if (!handler) {
        return -EINVAL;
    }

    user0_cb = handler;

	if (!device_is_ready(button0.port)) {
		return -EIO;
	}

	err = gpio_pin_configure_dt(&button0, GPIO_INPUT);
	if (err) {
        return err;
	}

	err = gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_BOTH);
	if (err) {
		return err;
	}

	gpio_init_callback(&button0_cb_data, button0_pressed, BIT(button0.pin));
	err = gpio_add_callback(button0.port, &button0_cb_data);
    if (err) {
        return err;
    }

    return 0;
}

//Button 1
static void cooldown1_expired(struct k_work *work)
{
    ARG_UNUSED(work);

    int val = gpio_pin_get_dt(&button1);
    enum button_evt evt = val ? BUTTON_EVT_RELEASED : BUTTON_EVT_PRESSED ;
    if (user1_cb) {
        user1_cb(evt);
    }
}

static K_WORK_DELAYABLE_DEFINE(cooldown1_work, cooldown1_expired);

void button1_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
    k_work_reschedule(&cooldown1_work, K_MSEC(15));
}

int button1_init(button_event_handler_t handler)
{
    int err = -1;

    if (!handler) {
        return -EINVAL;
    }

    user1_cb = handler;

	if (!device_is_ready(button1.port)) {
		return -EIO;
	}

	err = gpio_pin_configure_dt(&button1, GPIO_INPUT);
	if (err) {
        return err;
	}

	err = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_BOTH);
	if (err) {
		return err;
	}

	gpio_init_callback(&button1_cb_data, button1_pressed, BIT(button1.pin));
	err = gpio_add_callback(button1.port, &button1_cb_data);
    if (err) {
        return err;
    }

    return 0;
}
