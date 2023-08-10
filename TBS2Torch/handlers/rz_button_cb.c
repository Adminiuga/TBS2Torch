/***************************************************************************//**
 * @file
 * @brief rz_button counted button press component
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

#include "af.h"

#include "rz_button.h"

/* @brief Button short duration press callback
 *
 * @note Called for each short press of a button.
 *
 * @param button Button number
 */
WEAK(void rz_button_short_press_cb(uint8_t button)) {
}

/* @brief Button short duration press callback
 *
 * @note Called for multiple short presses of a button
 *
 * @param button Button number
 * @param counter Button was pressed so many times in quick succession
 */
WEAK(void rz_button_counted_short_press_cb(uint8_t button,
                uint8_t counter)) {
}

/* @brief Button medium duration press callback
 *
 * @note Called after repeated or single presses for a button
 *
 * @param button Button number
 */
WEAK(void rz_button_medium_press_cb(uint8_t button)) {
}

/* @brief Button long duration press callback
 *
 * @note Called after repeated or single presses for a button
 *
 * @param button Button number
 */
WEAK(void rz_button_long_press_cb(uint8_t button)) {
}
