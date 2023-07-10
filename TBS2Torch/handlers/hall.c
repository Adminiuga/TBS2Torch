/*
 * hall.c
 *
 *  Created on: Jun 11, 2023
 *      Author: achetroi
 */


#include "math.h"
#include "af.h"
#include "zigbee_app_framework_event.h"
#include "sl_sensor_hall.h"

#include "hall.h"

// -----------------------------------------------------------------------------
// Private macros
#define HALL_SHORT_POLL_MS 500
#define HALL_LONG_POLL_MS  1000
#define TRIGGER_LEVEL      1.0f
#define TRIGGER_HISTERESIS 0.1f
#define TRIGGER_INVALID    -99.99f
#define MEDIUM_DURATION_INTERVAL_MS 3*1000
#define LONG_DURATION_INTERVAL_MS   6*1000

typedef struct {
  uint32_t  lastTriggerTick;
  uint32_t  lastClearTick;
  float     previousMeasure;
  float     triggerLevel;
  float     histeresis;
  bool      isTriggered;
  bool      mediumTriggered;  // medium duration CB triggered
  bool      longTriggered;    // long duration CB triggered
} HallSensorState_t;

// -----------------------------------------------------------------------------
// private global variables
static sl_zigbee_event_t hall_measurement_event;
static HallSensorState_t localState = {
    0, 0, TRIGGER_INVALID, TRIGGER_LEVEL, TRIGGER_HISTERESIS, false, false, false};

// -----------------------------------------------------------------------------
// local functions forward declaration
static void hall_measurement_event_handler(sl_zigbee_event_t *event);
static void processNewMeasurement(float *fieldLevel);

// -----------------------------------------------------------------------------
// Callback definitions
/** @brief TriggerCallback
 *
 * Called when the magnetic field crosses threshold level, in either direction,
 * i.e. called when field goes above the threshold, or below the threshold
 *
 * @param field_level field level hen reached trigger threshold
 */
WEAK(void handlerHallTriggerCallback(float field_level, bool alarm) {})

/** @brief medium duration trigger
 *
 * Called when the magnetic field stays over the threshold level
 * for the MEDIUM_DURATION_INTERVAL_MS
 *
 * @param field_level field level of the last measurement
 */
WEAK(void handlerHallMediumDurationTriggerCallback(float field_level,
                                                   bool alarm) {})

/** @brief long duration trigger
 *
 * Called when the magnetic field stays over the threshold level
 * for the LONG_DURATION_INTERVAL_MS
 *
 * @param field_level field level of the last measurement
 */
WEAK(void handlerHallLongDurationTriggerCallback(float field_level,
                                                 bool alarm) {})


// -----------------------------------------------------------------------------
// Public function definitions

sl_status_t handlerHallInit(void)
{
  sl_zigbee_event_init(&hall_measurement_event, hall_measurement_event_handler);
  sl_status_t sc = sl_sensor_hall_init();

  if (sc == SL_STATUS_OK){
      sl_zigbee_event_set_delay_ms(&hall_measurement_event, HALL_SHORT_POLL_MS);
  }

  return sc;
}

// -----------------------------------------------------------------------------
// Private functions definitions

static void hall_measurement_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(&hall_measurement_event);

  float field_level;
  bool alert, tamper;

  sl_status_t status = sl_sensor_hall_get(&field_level, &alert, &tamper);
  if (status == SL_STATUS_OK) {
      processNewMeasurement(&field_level);
  } else {
      sl_zigbee_app_debug_println("Couldn't read hall sensor: 0x02x", status);
  }

  sl_zigbee_event_set_delay_ms(&hall_measurement_event, HALL_SHORT_POLL_MS);
}


static void processNewMeasurement(float *fieldLevel)
{
  float absLevel = fabsf(*fieldLevel);
  uint32_t tdiff;

  if (absLevel > (localState.triggerLevel + localState.histeresis)) {
      // the level is above threshold
      // update the last trigger time if it is a positive cross
      if ( !localState.isTriggered ) {
          // this is new positive crossing
          handlerHallTriggerCallback(*fieldLevel, true);
          localState.lastTriggerTick = sl_sleeptimer_get_tick_count();
          localState.isTriggered = true;
          localState.mediumTriggered = false;
          localState.longTriggered = false;
      } else {
          // we were above threshold before
          tdiff = sl_sleeptimer_get_tick_count() - localState.lastTriggerTick;
          if (tdiff >= sl_sleeptimer_ms_to_tick(MEDIUM_DURATION_INTERVAL_MS)
              && !(localState.mediumTriggered)) {
              localState.mediumTriggered = true;
              handlerHallMediumDurationTriggerCallback(*fieldLevel, true);
          }
          if (tdiff >= sl_sleeptimer_ms_to_tick(LONG_DURATION_INTERVAL_MS)
              && !(localState.longTriggered)) {
              localState.longTriggered = true;
              handlerHallLongDurationTriggerCallback(*fieldLevel, true);
          }
      }
  } else if (absLevel < (localState.triggerLevel - localState.histeresis)) {
      // the level is below threshold
      // update the last clear time if it is a negative cross
      if ( localState.isTriggered ) {
          handlerHallTriggerCallback(*fieldLevel, false);
          localState.lastTriggerTick = sl_sleeptimer_get_tick_count();
          localState.isTriggered = false;
      } else {
          // we were below threshold before
          tdiff = sl_sleeptimer_get_tick_count() - localState.lastTriggerTick;
          if (tdiff >= sl_sleeptimer_ms_to_tick(MEDIUM_DURATION_INTERVAL_MS)
              && localState.mediumTriggered) {
              localState.mediumTriggered = false;
              handlerHallMediumDurationTriggerCallback(*fieldLevel, false);
          }
          if (tdiff >= sl_sleeptimer_ms_to_tick(LONG_DURATION_INTERVAL_MS)
              && localState.longTriggered) {
              localState.longTriggered = false;
              handlerHallLongDurationTriggerCallback(*fieldLevel, false);
          }
      }
  }
}
