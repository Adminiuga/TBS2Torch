/*******************************************************************************
 * @file
 * @brief Power Configuration Server Cluster
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Alexei Chetroi</b>
 *******************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef SL_POWER_CONFIGURATION_SERVER_CONFIG_H
#define SL_POWER_CONFIGURATION_SERVER_CONFIG_H

#define SL_POWER_CONFIGURATION_BATTERY_TYPE_CUSTOM 0
#define SL_POWER_CONFIGURATION_BATTERY_TYPE_CR2032 1
#define SL_POWER_CONFIGURATION_BATTERY_TYPE_18650  2

// <<< Use Configuration Wizard in Context Menu >>>

// <h> Power Configuration Server Cluster configuration

// <o SL_POWER_CONFIGURATION_BATTERY_TYPE> Battery Type
// <i> Default: CR2032
// <i> Indicates the battery type in use.
// <SL_POWER_CONFIGURATION_BATTERY_TYPE_CUSTOM=> CUSTOM
// <SL_POWER_CONFIGURATION_BATTERY_TYPE_CR2023=> CR2032
// <SL_POWER_CONFIGURATION_BATTERY_TYPE_18650=> 18650
#define SL_POWER_CONFIGURATION_BATTERY_TYPE   SL_POWER_CONFIGURATION_BATTERY_TYPE_18650

// </h>

// <<< end of configuration section >>>
#endif  // SL_POWER_CONFIGURATION_SERVER_CONFIG_H
