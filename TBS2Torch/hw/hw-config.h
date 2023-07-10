/*
 * hal.h
 *
 *  Created on: May 2, 2023
 *      Author: lex
 */

#ifndef HAL_HAL_CONFIG_H_
#define HAL_HAL_CONFIG_H_

#include "em_gpio.h"

#define RGB_LED_ENABLE_PORT gpioPortJ
#define RGB_LED_ENABLE_LOC 14

#define RGB_LED_COMS_PORT gpioPortI
#define RGB_LED_COM0_LOC 0
#define RGB_LED_COM1_LOC 1
#define RGB_LED_COM2_LOC 2
#define RGB_LED_COM3_LOC 3

#endif /* HAL_HAL_CONFIG_H_ */
