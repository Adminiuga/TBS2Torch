/*
 * buttons.h
 *
 *  Created on: May 26, 2023
 *      Author: achetroi
 */

#ifndef HANDLERS_BUTTONS_H_
#define HANDLERS_BUTTONS_H_

#ifndef handlerButtonPressTimeoutMs
#define handlerButtonPressTimeoutMs 200
#endif

/** @brief Button short duration press callback
 *
 * Called for each short press of a button
 *
 * @param button Button number
 */
void handlerButtonShortPressCallback(uint8_t button);

/** @brief Button short duration press callback
 *
 * Called for multiple short presses of a button
 *
 * @param button Button number
 * @param counter Button was pressed so many times in quick succession
 */
void handlerButtonShortPressMultipleCallback(uint8_t button,
                                             uint8_t counter);

/** @brief Button medium duration press callback
 *
 * Called after repeated or single presses for a button
 *
 * @param button Button number
 */
void handlerButtonMediumPressCallback(uint8_t button);

/** @brief Button long duration press callback
 *
 * Called after repeated or single presses for a button
 *
 * @param button Button number
 */
void handlerButtonLongPressCallback(uint8_t button);


#endif /* HANDLERS_BUTTONS_H_ */
