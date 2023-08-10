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

#include "rz_button.h"

#include "af.h"
#include "sl_simple_button_instances.h"
#include "app_button_press.h"

typedef struct {
    uint8_t counter;
    uint32_t timestamp;
} buttonState_t;

static buttonState_t buttonState[SL_SIMPLE_BUTTON_COUNT];
static bool isInitialized = false;

// local events
static sl_zigbee_event_t button_press_timeout_event;

// implemented callback declarations
void app_button_press_cb(uint8_t button, uint8_t duration);

// local functions declarations
static void buttonPressTimeoutEventHandler(sl_zigbee_event_t *event);
static void buttonHandlersInit(void);
static void processShortPress(uint8_t button);

/********************************
 * Implemented callbacks
 ********************************/

/** @brief callback from app_button_press callback
 *
 * @param button
 * @param duration
 ********************************/
void app_button_press_cb(uint8_t button, uint8_t duration) {
    if (!isInitialized)
        buttonHandlersInit();

    switch (duration) {
    case APP_BUTTON_PRESS_DURATION_SHORT:
        processShortPress(button);
        break;

    case APP_BUTTON_PRESS_DURATION_MEDIUM:
        rz_button_medium_press_cb(button);
        break;

    case APP_BUTTON_PRESS_DURATION_LONG:
        rz_button_long_press_cb(button);
        break;
    }
}

/********************************
 * Local function implementations
 ********************************/
static void buttonHandlersInit(void) {
    if (isInitialized)
        return;

    sl_zigbee_event_init(&button_press_timeout_event, buttonPressTimeoutEventHandler);
    for (uint8_t i = 0; i < SL_SIMPLE_BUTTON_COUNT; i++) {
        buttonState[i].counter = 0;
        buttonState[i].timestamp = 0;
    }
    isInitialized = true;
}

static void processShortPress(uint8_t button) {
    rz_button_short_press_cb(button);
    if (button > SL_SIMPLE_BUTTON_COUNT) {
        sl_zigbee_app_debug_println(
                "handlers/buttons: unexpected %d button number, max supported buttons: %d",
                button, SL_SIMPLE_BUTTON_COUNT);
        return;
    }

    buttonState_t *state = &buttonState[button];
    state->counter++;
    // max 255 presses. CallBack immediately without waiting for a timeout
    if (0xff == state->counter) {
        rz_button_counted_short_press_cb(button, state->counter);
        state->counter = 0;
        state->timestamp = 0;
        return;
    }

    state->timestamp = sl_sleeptimer_get_tick_count();
    sl_zigbee_event_set_delay_ms(&button_press_timeout_event, handlerButtonPressTimeoutMs);
}
;

//---------------
// Event handlers

static void buttonPressTimeoutEventHandler(sl_zigbee_event_t *event) {
    sl_zigbee_event_set_inactive(&button_press_timeout_event);

    bool reschedule = false;
    uint32_t tdiff;

    for (uint8_t i = 0; i < SL_SIMPLE_BUTTON_COUNT; i++) {
        if (!buttonState[i].counter)
            continue;

        // current button is actively counting. are we in timeout for it yet?
        tdiff = sl_sleeptimer_get_tick_count() - buttonState[i].timestamp;

        if (tdiff >= sl_sleeptimer_ms_to_tick(handlerButtonPressTimeoutMs)) {
            // press counting timeout -> trigger callback
            rz_button_counted_short_press_cb(i, buttonState[i].counter);
            buttonState[i].counter = 0;
            buttonState[i].timestamp = 0;
        } else {
            reschedule = true;
        }
    };

    if (reschedule)
        sl_zigbee_event_set_delay_ms(&button_press_timeout_event, handlerButtonPressTimeoutMs);
}
