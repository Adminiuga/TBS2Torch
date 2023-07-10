/*
 * buttons-cbs.c
 *
 *  Created on: May 26, 2023
 *      Author: achetroi
 */

#include "af.h"

#include "buttons.h"

/** @brief Button short duration press callback
 *
 * Called for each short press of a button
 *
 * @param button Button number
 */
WEAK(void handlerButtonShortPressCallback(uint8_t button))
{
}

/** @brief Button short duration press callback
 *
 * Called for multiple short presses of a button
 *
 * @param button Button number
 * @param counter Button was pressed so many times in quick succession
 */
WEAK(void handlerButtonShortPressMultipleCallback(uint8_t button,
                                                  uint8_t counter))
{
}

/** @brief Button medium duration press callback
 *
 * Called after repeated or single presses for a button
 *
 * @param button Button number
 */
WEAK(void handlerButtonMediumPressCallback(uint8_t button))
{
}

/** @brief Button long duration press callback
 *
 * Called after repeated or single presses for a button
 *
 * @param button Button number
 */
WEAK(void handlerButtonLongPressCallback(uint8_t button))
{
}
