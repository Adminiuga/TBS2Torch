/***************************************************************************//**
 * @file
 * @brief rz_button_config Config
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

#ifndef RZ_BUTTON_CONFIG_H
#define RZ_BUTTON_CONFIG_H

// <<< Use Configuration Wizard in Context Menu

// <h> Counted Button Press configuration

// <o RZ_BUTTON_PRESS_TIMEOUT_MS> Counted short press timeout
// <100..2000:50>
// <i> Default: 750
// <i> The maximum blink pattern length
#define RZ_BUTTON_PRESS_TIMEOUT_MS 700

// </h> end Counted Button Press config
// <<< end of configuration section >>>

#endif // RZ_BUTTON_CONFIG_H