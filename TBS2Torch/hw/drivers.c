/*
 * hal.c
 *
 *  Created on: May 2, 2023
 *      Author: lex
 */

#include "em_gpio.h"

#include "drivers.h"

#include "af.h"
#include "sl_zigbee_debug_print.h"
#include "sl_simple_rgb_pwm_led.h"

#define MAX(a, b) (a > b) ? a : b

extern sl_led_rgb_pwm_t sl_simple_rgb_pwm_led_rgb_led0;

static uint8_t targetLevel = 254;

/***
 * Static declarations
 */
static void hal_init_rgb_leds();

/*
 * Public implementations
 */
void hal_init(void)
{
 hal_init_rgb_leds();
}

void hal_rgb_leds_enable()
{
  GPIO_PinOutSet(RGB_LED_ENABLE_PORT, RGB_LED_ENABLE_LOC);
}

void hal_rgb_leds_disable()
{
  GPIO_PinOutClear(RGB_LED_ENABLE_PORT, RGB_LED_ENABLE_LOC);
}

/*
 * Enable Specific RGB Led
 */
void hal_rgb_led_enable(int ledNumber)
{
  switch (ledNumber)
  {
    case 0:
      GPIO_PinOutSet(RGB_LED_COMS_PORT, RGB_LED_COM0_LOC);
      break;

    case 1:
      GPIO_PinOutSet(RGB_LED_COMS_PORT, RGB_LED_COM1_LOC);
      break;

    case 2:
      GPIO_PinOutSet(RGB_LED_COMS_PORT, RGB_LED_COM2_LOC);
      break;

    case 3:
      GPIO_PinOutSet(RGB_LED_COMS_PORT, RGB_LED_COM3_LOC);
      break;
  }
}


/*
 * Disable Specific RGB Led
 */
void hal_rgb_led_disable(int ledNumber)
{
  switch (ledNumber)
  {
    case 0:
      GPIO_PinOutClear(RGB_LED_COMS_PORT, RGB_LED_COM0_LOC);
      break;

    case 1:
      GPIO_PinOutClear(RGB_LED_COMS_PORT, RGB_LED_COM1_LOC);
      break;

    case 2:
      GPIO_PinOutClear(RGB_LED_COMS_PORT, RGB_LED_COM2_LOC);
      break;

    case 3:
      GPIO_PinOutClear(RGB_LED_COMS_PORT, RGB_LED_COM3_LOC);
      break;
  }
}

void hal_rgb_led_turnonoff(bool turnon)
{
  if ( turnon ) {
    hal_rgb_leds_enable();
    sl_simple_rgb_pwm_led_turn_on(sl_simple_rgb_pwm_led_rgb_led0.led_common.context);
  } else {
    sl_simple_rgb_pwm_led_turn_off(sl_simple_rgb_pwm_led_rgb_led0.led_common.context);
    hal_rgb_leds_disable();
  }
}

/*
 * Turn LEDs on
 */
void hal_rgb_led_turnon()
{
  hal_rgb_led_turnonoff( true );
}


/*
 * Turn LEDs off
 */
void hal_rgb_led_turnoff()
{
  hal_rgb_led_turnonoff( false );
}


/*
 * Set RGB Led color
 */
void hal_rgb_led_set_rgbcolor(uint16_t red, uint16_t green, uint16_t blue)
{
  sl_led_set_rgb_color(&sl_simple_rgb_pwm_led_rgb_led0, red, green, blue);
}


/*
 * Set RGB Led brightness
 */
void hal_rgb_led_set_brightness(uint8_t level)
{
  uint16_t red, green, blue;
  sl_led_get_rgb_color(&sl_simple_rgb_pwm_led_rgb_led0, &red, &green, &blue);
  red = MAX(red, 1);
  green = MAX(green, 1);
  blue = MAX(blue, 1);

  red = red * level / targetLevel;
  green = green * level / targetLevel;
  blue = blue * level / targetLevel;

  sl_led_set_rgb_color(&sl_simple_rgb_pwm_led_rgb_led0, red, green, blue);
  if ( SL_LED_CURRENT_STATE_OFF == sl_led_get_state((const sl_led_t *) &sl_simple_rgb_pwm_led_rgb_led0) )
  {
    hal_rgb_led_turnoff();
  }
  targetLevel = MAX(level, 1);
}


/*
 * Local implementations
 */

static void hal_init_rgb_leds()
{
  GPIO_PinModeSet(RGB_LED_ENABLE_PORT, RGB_LED_ENABLE_LOC, gpioModePushPull, 0);
  GPIO_PinModeSet(RGB_LED_COMS_PORT, RGB_LED_COM0_LOC, gpioModePushPull, 0);
  GPIO_PinModeSet(RGB_LED_COMS_PORT, RGB_LED_COM1_LOC, gpioModePushPull, 0);
  GPIO_PinModeSet(RGB_LED_COMS_PORT, RGB_LED_COM2_LOC, gpioModePushPull, 0);
  GPIO_PinModeSet(RGB_LED_COMS_PORT, RGB_LED_COM3_LOC, gpioModePushPull, 0);
}
