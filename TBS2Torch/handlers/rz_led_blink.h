/*******************************************************************************
 * @file
 * @brief API to blink simple led instances
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

#ifndef RZ_LED_BLINK_H_
#define RZ_LED_BLINK_H_

/** @brief Initialize LED Blink control
 *
 * @note This function will initialize led blinker
 */
void rz_led_blink_init(void);

/** @brief Turn the Activity LED off, for multiple LED control
 *
 * @note This function will turn the activity LED off.  If a value of 0 is
 * passed in, it will keep the LED off indefinitely.  Otherwise, it will turn
 * the LED back on after the specified amount of time has passed.
 *
 * @param time  The amount of time (in mS) to turn the LED off.  If 0, the LED
 * will remain off indefinitely.
 * @param ledIndex The led number to be turned off.
 */
void rz_led_blink_blink_led_off(uint32_t timeMs, uint8_t ledIndex);

/** @brief Turn the Activity LED on, for multiple LED control
 *
 * @note This function will turn the activity LED on.  If a value of 0 is
 * passed in, it will keep the LED on indefinitely.  Otherwise, it will turn
 * the LED back off after the specified amount of time has passed.
 *
 * @param time  The amount of time (in mS) to turn the LED on.  If 0, the LED
 * will remain on indefinitely.
 * @param ledIndex The led to be turned on.
 */
void rz_led_blink_blink_led_on(uint32_t timeMs, uint8_t ledIndex);

/** @brief Set the activity LED to blink a given number of times, for multiple
 * LED control
 *
 * @note This function will cause the activity LED to blink a user provided
 * number of times at a user provided rate.
 *
 * @param count  The number of times to blink the LED
 * @param blinkTimeMs  The amount of time (in ms) the LED should be on and off
 * @param ledIndex The led number to blink
 */
void rz_led_blink_counted(uint8_t count, uint16_t blinkTimeMs, uint8_t ledIndex);

/** @brief Cause the activity LED to blink in a specified pattern, for multiple
 * LED control
 *
 * @note This function will cause the activity LED to blink a user specified
 * pattern.
 *
 * @param count  The number of times to cycle through the pattern
 * @param length  The number entries in the pattern array
 * @param pattern  An array of on and off times (in mS) that describes the
 * blink pattern.  The first integer must describe an on time (can be 0), but
 * the last integer is not required to be an off time.  Maximum pattern length
 * is 20 unsigned 16 bit entries.
 * @param led The led number to blink.
 */
void rz_led_blink_pattern(uint8_t count, uint8_t length, uint16_t *pattern, uint8_t ledIndex);
#endif /* RZ_LED_BLINK_H_ */
