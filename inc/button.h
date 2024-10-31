/*
Ce code a été récupéré depuis ce dépot GIT : 
https://github.com/ubieda/zephyr_button_debouncing/tree/master/src
*/

#ifndef _BUTTON_H_
#define _BUTTON_H_

enum button_evt {
    BUTTON_EVT_PRESSED,
    BUTTON_EVT_RELEASED
};

typedef void (*button_event_handler_t)(enum button_evt evt);

int button0_init(button_event_handler_t handler);
int button1_init(button_event_handler_t handler);

#endif /* _BUTTON_H_ */
