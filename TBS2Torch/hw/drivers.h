/*
 * hal.h
 *
 *  Created on: May 2, 2023
 *      Author: lex
 */

#ifndef HAL_HAL_H_
#define HAL_HAL_H_

#include "hw-config.h"

void hal_init(void);

// RGB LED specifics
void hal_rgb_leds_enable();
void hal_rgb_leds_disable();
void hal_rgb_led_enable(int ledNumber);
void hal_rgb_led_disable(int ledNumber);
void hal_rgb_led_turnon();
void hal_rgb_led_turnoff();
void hal_rgb_led_set_rgbcolor(uint16_t red, uint16_t green, uint16_t blue);
void hal_rgb_led_set_brightness(uint8_t brightness);
uint8_t hal_rgb_led_get_brightness();

#endif /* HAL_HAL_H_ */
