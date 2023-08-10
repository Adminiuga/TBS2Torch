/***************************************************************************//**
 * @file
 * @brief rz_led_blink_config Config
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Alexei Chetroi</b>
 *******************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef RZ_LED_BLINK_CONFIG_H
#define RZ_LED_BLINK_CONFIG_H

// <<< Use Configuration Wizard in Context Menu

// <h> Led Blinker configuration

// <o RZ_LED_BLINK_MAX_PATTERN_LEN> Maximim blink pattern length
// <1..40:1>
// <i> Default: 20
// <i> The maximum blink pattern length
#define RZ_LED_BLINK_MAX_PATTERN_LEN 20

// <o RZ_LED_BLINK_MAX_LED_INSTANCES> Maximum number of led instances
// <1..5:1>
// <i> Default: 2
// <i> The number of simple led instances to support
#define RZ_LED_BLINK_MAX_LED_INSTANCES 2

// </h> end Led Blinker config
// <<< end of configuration section >>>

#endif // RZ_LED_BLINK_CONFIG_H
