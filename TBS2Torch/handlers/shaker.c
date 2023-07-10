/***************************************************************************//**
 * @file
 * @brief Inertial Measurement Unit sensor
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include <stdlib.h>

#include "af.h"
#include "sl_board_control.h"
#include "sl_imu.h"

#include "shaker.h"




// --------------------------------------------------------------------
// local state
typedef struct {
  int16_t   x, y, z;
} imu_acc_t;

typedef struct {
  HandlerShakerShakingState_t current;
  imu_acc_t                   acc;
  bool                        isInitialized;
  uint32_t                    eventTick;
} state_t;


// --------------------------------------------------------------------
// local variables
static sl_zigbee_event_t imu_measurement_event;
static state_t state = {SHAKING_IDLE, {0, 0, 0}, false, 0};


// --------------------------------------------------------------------
// local macros
#define IMU_SAMPLE_RATE           10.0f /* Hz */
#define EVT_MEASURE               (&imu_measurement_event)
#define MEASUREMENT_INIT_DELAY_MS 2 * 1000
#define MEASUREMENT_DELAY_MS      100
#define ACC_THRESHOLD             150
#define TRIGGER_COOLDOWN_MS       600
#define STILL_SHAKING_REEVENT_MS  1000
#define LOG_TRANSITION(x) sl_zigbee_app_debug_println(\
        "IMU handler: line %d: Transition from %d to %d", \
        __LINE__, state.current, x)

// --------------------------------------------------------------------
// Forward declarations
static void imuMeasurementEventHandler(sl_zigbee_event_t *event);
static void processNewAccData(int16_t acc[3]);
// IMU driver
static sl_status_t sl_sensor_imu_enable(bool enable);
static sl_status_t sl_sensor_imu_get(int16_t avec[3]);


// --------------------------------------------------------------------
// CallBacks
WEAK(void handlerShakerShakingStart(void) {});
WEAK(void handlerShakerStillShaking(uint32_t durationMs) {});
WEAK(void handlerShakerShakingStop(uint32_t durationMs) {});

// --------------------------------------------------------------------
// Public function implementations
void handlerShakerInit(void)
{
  (void)sl_board_enable_sensor(SL_BOARD_SENSOR_IMU);
  sl_status_t sc = sl_sensor_imu_enable(true);
  sl_zigbee_app_debug_println("Initialized IMU: 0x%02x", sc);

  sl_zigbee_event_init(EVT_MEASURE, imuMeasurementEventHandler);
  sl_zigbee_event_set_delay_ms(EVT_MEASURE, MEASUREMENT_INIT_DELAY_MS);
}


// --------------------------------------------------------------------
// Local event handlers
static void imuMeasurementEventHandler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(EVT_MEASURE);

  int16_t avec[3];

  sl_status_t sc = sl_sensor_imu_get(avec);
  if ( sc == SL_STATUS_OK ) {
      //sl_zigbee_app_debug_println("IMU reading avec: %05d / %05d / %05d ",
      //                            avec[0], avec[1], avec[2]);
      processNewAccData(avec);
  } else {
      sl_zigbee_app_debug_println("Couldn't read IMU: 0x%02x", sc);
  }

  sl_zigbee_event_set_delay_ms(EVT_MEASURE, MEASUREMENT_DELAY_MS);
}


static void processNewAccData(int16_t acc[3]) {
  if ( !state.isInitialized) {
      state.acc.x = acc[0];
      state.acc.y = acc[1];
      state.acc.z = acc[2];
      state.isInitialized = true;
      return;
  }

  static uint32_t continiousShaking = 0;
  uint32_t tdiff = sl_sleeptimer_tick_to_ms(sl_sleeptimer_get_tick_count()
                                            - state.eventTick);
  bool xIsShaking, yIsShaking, zIsShaking, isShaking;
  xIsShaking = yIsShaking = zIsShaking = isShaking = false;

  if ( abs(acc[0] - state.acc.x) > ACC_THRESHOLD) xIsShaking = true;
  if ( abs(acc[1] - state.acc.y) > ACC_THRESHOLD) yIsShaking = true;
  if ( abs(acc[2] - state.acc.z) > ACC_THRESHOLD) zIsShaking = true;

  state.acc.x = acc[0];
  state.acc.y = acc[1];
  state.acc.z = acc[2];

  if ((xIsShaking && yIsShaking)
     || (xIsShaking && zIsShaking)
     || (yIsShaking && zIsShaking)) {
      // Shake is detected
      isShaking = true;
  }

  switch ( state.current ) {
    case SHAKING_IDLE:
      if ( isShaking && tdiff >= (TRIGGER_COOLDOWN_MS << 1)) {
        LOG_TRANSITION(SHAKING_START);
        state.current   = SHAKING_START;
        state.eventTick = sl_sleeptimer_get_tick_count();
        sl_zigbee_app_debug_println("Shaking started, was idle for %dms", tdiff);
        handlerShakerShakingStart();
      }
      break;

    case SHAKING_START:
      if ( isShaking ) {
        // trigger continuous shaking if shaking more than X ms
        if ( tdiff >= STILL_SHAKING_REEVENT_MS ) {
          LOG_TRANSITION(SHAKING_STILL_SHAKING);
          state.current   = SHAKING_STILL_SHAKING;
          continiousShaking = tdiff;
          state.eventTick = sl_sleeptimer_get_tick_count();
          sl_zigbee_app_debug_println("Still Shaking for %dms", tdiff);
          handlerShakerStillShaking(tdiff);
        }
      } else if (!isShaking && tdiff >= (TRIGGER_COOLDOWN_MS >> 1)) {
          LOG_TRANSITION(SHAKING_STOP);
          state.current = SHAKING_STOP;
          sl_zigbee_app_debug_println(
              "Shaking stopped, duration: %dms", tdiff);
          handlerShakerShakingStop(tdiff);
      }
      break;

    case SHAKING_STILL_SHAKING:
      // event was already fired, check if we need to fire the stop shaking event
      continiousShaking += tdiff;
      if ( isShaking && tdiff >= STILL_SHAKING_REEVENT_MS ) {
          state.eventTick = sl_sleeptimer_get_tick_count();
          sl_zigbee_app_debug_println("Still Shaking for %dms",
                                      continiousShaking);
          handlerShakerStillShaking(continiousShaking);
      } else if ( !isShaking ) {
          LOG_TRANSITION(SHAKING_STOP);
          state.current = SHAKING_STOP;
          sl_zigbee_app_debug_println("Shaking stopped, duration: %dms",
                                      continiousShaking);
          handlerShakerShakingStop(continiousShaking);
      }
      break;

    case SHAKING_STOP:
      // event was already fired, just transition to idle and cooldown
      LOG_TRANSITION(SHAKING_IDLE);
      state.current = SHAKING_IDLE;
      state.eventTick = sl_sleeptimer_get_tick_count();
      break;
  }
}


static sl_status_t sl_sensor_imu_enable(bool enable)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t imuState = sl_imu_get_state();
  if (enable && (IMU_STATE_DISABLED == imuState)) {
    sc = sl_imu_init();
    if (SL_STATUS_OK == sc) {
      sl_imu_configure(IMU_SAMPLE_RATE);
      state.isInitialized = true;
    } else {
      state.isInitialized = false;
    }
  } else if (!enable && (IMU_STATE_READY == imuState)) {
      (void)sl_board_disable_sensor(SL_BOARD_SENSOR_IMU);
      state.isInitialized = false;
  }
  return sc;
}


static sl_status_t sl_sensor_imu_get(int16_t avec[3])
{
  sl_status_t sc = SL_STATUS_NOT_READY;
  if (state.isInitialized) {
    if (sl_imu_is_data_ready()) {
      sl_imu_update();
      sl_imu_get_acceleration(avec);
      sc = SL_STATUS_OK;
    }
  } else {
    sc = SL_STATUS_NOT_INITIALIZED;
  }
  return sc;
}
