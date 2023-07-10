/*
 * color_cbs.c
 *
 *  Created on: May 5, 2023
 *      Author: lex
 */

#include "math.h"
#include "af.h"

#include "../hw/drivers.h"




/** @brief Compute Pwm from HSV
 *
 * This function is called from the color server when it is time for the PWMs to
 * be driven with a new value from the HSV values.
 *
 * @param endpoint The identifying endpoint Ver.: always
 */
void emberAfPluginColorControlServerComputePwmFromHsvCallback(uint8_t endpoint)
{
}

/** @brief Compute Pwm from HSV
 *
 * This function is called from the color server when it is time for the PWMs to
 * be driven with a new value from the color X and color Y values.
 *
 * @param endpoint The identifying endpoint Ver.: always
 */
void emberAfPluginColorControlServerComputePwmFromXyCallback(uint8_t endpoint)
{
  uint16_t color_x, color_y;
  uint8_t level;
  if (emberAfReadServerAttribute(endpoint,
                                 ZCL_COLOR_CONTROL_CLUSTER_ID,
                                 ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID,
                                 (void *) &color_x,
                                 sizeof(color_x))
        != EMBER_ZCL_STATUS_SUCCESS) {
      return;
  }
  if (emberAfReadServerAttribute(endpoint,
                                 ZCL_COLOR_CONTROL_CLUSTER_ID,
                                 ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID,
                                 (void *) &color_y,
                                 sizeof(color_y))
        != EMBER_ZCL_STATUS_SUCCESS) {
      return;
  }
  if (emberAfReadServerAttribute(endpoint,
                                 ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                 ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                 (void *) &level,
                                 sizeof(level))
        != EMBER_ZCL_STATUS_SUCCESS) {
      return;
  }
  sl_zigbee_app_debug_println("Current x,y is (0x%x, 0x%x), brightness: %d", color_x, color_y, level);

  double x = color_x / 65535.0f;
  double y = color_y / 65535.0f;
  double z = 1.0 - x - y;

  double Y = 1.0f; // The reference white point luminance
  sl_zigbee_app_debug_println("Current x,y is (%.5f, %.5x), luminance: %f", x, y, Y);
  double X = (Y / y) * x;
  double Z = (Y / y) * z;

  double r_linear = X * 1.656492f - Y * 0.354851f - Z * 0.255038f;
  double g_linear = -X * 0.707196f + Y * 1.655397f + Z * 0.036152f;
  double b_linear = X * 0.051713f - Y * 0.121364f + Z * 1.011530f;

  // Apply gamma correction to convert from linear to sRGB color space
  double gamma = 2.4;
  double a = 0.055;
  double r = (r_linear <= 0.0031308f) ? 12.92f*r_linear : (1.0f+a)*pow(r_linear, 1.0/gamma) - a;
  double g = (g_linear <= 0.0031308f) ? 12.92f*g_linear : (1.0f+a)*pow(g_linear, 1.0/gamma) - a;
  double b = (b_linear <= 0.0031308f) ? 12.92f*b_linear : (1.0f+a)*pow(b_linear, 1.0/gamma) - a;

  if (r < 0) {
      r = 0;
  } else if (r > 1) {
      r = 1;
  }
  if (g < 0) {
      g = 0;
  } else if (g > 1) {
      g = 1;
  }
  if (b < 0) {
      b = 0;
  } else if (b > 1) {
      b = 1;
  }

  float flevel = 1.0 * level;

  // Normalize to the range 0-255 and round to the nearest integer
  //sl_zigbee_app_debug_print("Calculated R:%d G:%d B:%d", (uint16_t)round(r*255.0), (uint16_t)round(g*255.0), (uint16_t)round(b*255.0));
  sl_zigbee_app_debug_println("Calculated R:%.5f G:%.5f B:%.5f", r, g, b);
  sl_zigbee_app_debug_println("Calculated R:0x%x, G:0x%x, B:0x%x", (uint16_t)round(r*flevel), (uint16_t)round(g*flevel), (uint16_t)round(b*flevel));
  hal_rgb_led_set_rgbcolor((uint16_t)round(r*flevel), (uint16_t)round(g*flevel), (uint16_t)round(b*flevel));

}

/** @brief Compute Pwm from HSV
 *
 * This function is called from the color server when it is time for the PWMs to
 * be driven with a new value from the color temperature.
 *
 * @param endpoint The identifying endpoint Ver.: always
 */
void emberAfPluginColorControlServerComputePwmFromTempCallback(uint8_t endpoint)
{
}
