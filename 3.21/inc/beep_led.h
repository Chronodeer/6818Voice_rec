#ifndef __BEEP_LED_H
#define __BEEP_LED_H

int beep_init(void);
int led_init(void);
void beep_led_on(void);
void beep_led_off(void);
void beep_exit(void);
void app_beep_led(void);
#endif