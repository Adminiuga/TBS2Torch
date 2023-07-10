/*
 * hall-conn-manager.c
 *
 * Connection manager based on hall sensor.
 *
 * Primary function is to reset current network. To do so, the magnetic field
 * should be detected for "medium hall detection" interval + "long hall
 * detection" interval + 5 additional seconds while led is red + while led is
 * blinking.
 *
 * if the field disappears right after "medium hall detection" callback, then
 * indicate the network status.
 *
 * if there's no network, then instead start the find&binf or network steering
 * right after "long hall detection" interval.
 *
 * Supplementary function: toggle the light, on short duration field detection
 *
 *  Created on: Jun 17, 2023
 *      Author: achetroi
 */

#include "app/framework/include/af.h"
#include "on-off.h"

#include "../hw/drivers.h"
#include "network.h"

// -------------------------
// Local macros
#define CONN_MAN_COOL_DOWN_PERIOD_MS     1 * 1000
#define CONN_MAN_READY_TO_LEAVE_DELAY_MS 5 * 1000
#define CONN_MAN_BLINK_DURATION_MS       300
#define CONN_MAN_BLINK_COUNT             5 * 2
#define LED_COLOR_RED     hal_rgb_led_set_rgbcolor(0xFF, 0x00, 0x00);
#define LED_COLOR_GREEN   hal_rgb_led_set_rgbcolor(0, 0xFF, 0x00);
#define LED_COLOR_ORANGE  hal_rgb_led_set_rgbcolor(0xFF, 0xA5, 0x00);
#define LED_ON            hal_rgb_led_turnon(); hal_rgb_led_set_brightness(0xFE);
#define LED_OFF           hal_rgb_led_turnoff(); hal_rgb_led_set_brightness(0xFE);

typedef enum {
  CONNECTION_MANAGER_IDLE,
  CONNECTION_MANAGER_ARMED_1,
  CONNECTION_MANAGER_ARMED_2,
  CONNECTION_MANAGER_READY_TO_LEAVE,
  CONNECTION_MANAGER_LEAVING_BLINKING_ON,
  CONNECTION_MANAGER_LEAVING_BLINKING_OFF,
  CONNECTION_MANAGER_NWK_STATE_BLINKING_OFF,
  CONNECTION_MANAGER_NWK_STATE_BLINKING_ON,
  CONNECTION_MANAGER_LEAVE
} ConnManStateEnum_t;

typedef struct {
  bool               isInitialized;
  uint32_t           coolDownTick;
  ConnManStateEnum_t current;
  uint8_t            blinkCount;
} ConnManState_t;

// local module variables
static ConnManState_t state = {false, 0, CONNECTION_MANAGER_IDLE};
static sl_zigbee_event_t leaving_event;


// -------------------------------------------------------------------------
// Forward local function definitions
static void initConnectionManager(void);
static void cleanUpAndStop(void);
static void blinkGreen(void);
static bool isOnNetwork(void);
static void blinkNwkState(bool blinkOn);

// -------------------------------------------------------------------------
// Forward event handler definitions
static void leavingEventHandler(sl_zigbee_event_t *event);

/*
 * Use "immediate field detection" callback to toggle the light,
 * unless the reset sequence is in progress
 */
void handlerHallTriggerCallback(float field_level, bool alarm)
{
  if ( !state.isInitialized ) initConnectionManager();

  sl_zigbee_app_debug_println("hall-conn-man callback: alarm: 0x%02x", alarm);

  uint32_t tick = sl_sleeptimer_get_tick_count();
  uint32_t tdiff = sl_sleeptimer_tick_to_ms(tick - state.coolDownTick);

  if ( alarm ) return; // not doing anything on field detection

  if ( state.current == CONNECTION_MANAGER_IDLE ){
      if ( tdiff >= CONN_MAN_COOL_DOWN_PERIOD_MS) {
          state.coolDownTick = tick;
          emberAfOnOffClusterSetValueCallback(emberAfPrimaryEndpoint(),
                                              ZCL_TOGGLE_COMMAND_ID,
                                              false);
      }
  } else if ( state.current == CONNECTION_MANAGER_ARMED_1 ) {
      // indicate stack status
      networkHandlersIndicateNetworkStatus();
      state.current = CONNECTION_MANAGER_NWK_STATE_BLINKING_OFF;
      state.blinkCount = CONN_MAN_BLINK_COUNT;
      sl_zigbee_event_set_delay_ms(&leaving_event, CONN_MAN_BLINK_DURATION_MS);
      return;
  } else if ( state.current == CONNECTION_MANAGER_ARMED_2) {
      networkHandlersFindAndBind();
  }
  cleanUpAndStop();
}


void handlerHallMediumDurationTriggerCallback(float field_level, bool alarm)
{
  if ( !state.isInitialized ) initConnectionManager();

  sl_zigbee_app_debug_println("hall-conn-man callback medium, alarm: 0x%02x",
                              alarm);

  if ( alarm ) {
      if ( state.current == CONNECTION_MANAGER_IDLE ) {
          state.current = CONNECTION_MANAGER_ARMED_1;
          // make it full brightness and green
          LED_ON
          LED_COLOR_GREEN
      } else {
          // any other state reset
          cleanUpAndStop();
      }
  }
}

void handlerHallLongDurationTriggerCallback(float field_level, bool alarm)
{
  if ( !state.isInitialized ) initConnectionManager();

  sl_zigbee_app_debug_println("hall-conn-man callback long, alarm: 0x%02x",
                              alarm);

  if ( alarm ) {
      if ( state.current == CONNECTION_MANAGER_ARMED_1 ) {
          state.current = CONNECTION_MANAGER_ARMED_2;
          // make it full brightness and Orange
          LED_ON
          LED_COLOR_ORANGE
          sl_zigbee_event_set_delay_ms(&leaving_event,
                                       CONN_MAN_READY_TO_LEAVE_DELAY_MS);
      } else {
          // any other state reset
          cleanUpAndStop();
      }
  }
}


// -------------------------------------------------------------------------
// Local Functions implementations
static void cleanUpAndStop(void)
{
  if ( state.current == CONNECTION_MANAGER_LEAVE ) {
      // this state should run its course
      sl_zigbee_event_set_delay_ms(&leaving_event, CONN_MAN_BLINK_DURATION_MS);
      return;
  } else {
      sl_zigbee_event_set_inactive(&leaving_event);
  }

  EmberStatus status;
  bool        isOn;
  uint8_t     level;

  status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                      ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                      ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                      (uint8_t *) &level,
                                      sizeof(level));
  if ( status != EMBER_SUCCESS ){
      level = 0xFE;
  }

  status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                      ZCL_ON_OFF_CLUSTER_ID,
                                      ZCL_ON_OFF_ATTRIBUTE_ID,
                                      (uint8_t *) &isOn,
                                      sizeof(isOn));
  if ( status != EMBER_SUCCESS ){
      isOn = false;
  }

  hal_rgb_led_set_brightness(level);
  hal_rgb_led_set_rgbcolor(0xFF, 0xFF, 0xFF); // ToDo: restore color
  if ( isOn ) {
      hal_rgb_led_turnon();
  } else {
      hal_rgb_led_turnoff();
  }

  state.current = CONNECTION_MANAGER_IDLE;
}


/*
 * Initialize connection manager on demand
 */
static void initConnectionManager(void)
{
  if ( state.isInitialized ) return;

  sl_zigbee_event_init(&leaving_event, leavingEventHandler);
  state.isInitialized = true;
}


#define LOG_TRANSITION(x) sl_zigbee_app_debug_println(\
        "hall-conn-man: line %d: Transition from %d to %d", \
        __LINE__, state.current, x)

static void leavingEventHandler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(&leaving_event);

  switch ( state.current ) {
    case CONNECTION_MANAGER_ARMED_2:
      // switch to next state and turn led red
      LOG_TRANSITION(CONNECTION_MANAGER_READY_TO_LEAVE);
      state.current = CONNECTION_MANAGER_READY_TO_LEAVE;
      LED_ON
      LED_COLOR_RED
      sl_zigbee_event_set_delay_ms(&leaving_event,
                                   CONN_MAN_READY_TO_LEAVE_DELAY_MS/2);
      break;

    case CONNECTION_MANAGER_READY_TO_LEAVE:
      // prepare to start blinking
      LOG_TRANSITION(CONNECTION_MANAGER_LEAVING_BLINKING_ON);
      state.current = CONNECTION_MANAGER_LEAVING_BLINKING_ON;
      state.blinkCount = CONN_MAN_BLINK_COUNT;
      LED_OFF
      LED_COLOR_RED
      sl_zigbee_event_set_delay_ms(&leaving_event,
                                   CONN_MAN_BLINK_DURATION_MS);
      break;

    case CONNECTION_MANAGER_LEAVING_BLINKING_ON:
      // switch to on and count blinks
      if ( state.blinkCount ) {
          LOG_TRANSITION(CONNECTION_MANAGER_LEAVING_BLINKING_OFF);
          state.current = CONNECTION_MANAGER_LEAVING_BLINKING_OFF;
          state.blinkCount--;
          LED_ON
          LED_COLOR_RED
          sl_zigbee_event_set_delay_ms(&leaving_event,
                                       CONN_MAN_BLINK_DURATION_MS);
      } else {
          LOG_TRANSITION(CONNECTION_MANAGER_LEAVE);
          state.current = CONNECTION_MANAGER_LEAVE;
          blinkGreen();
      }
      break;

    case CONNECTION_MANAGER_LEAVING_BLINKING_OFF:
      // switch to off and count blinks
      if ( state.blinkCount ) {
          LOG_TRANSITION(CONNECTION_MANAGER_LEAVING_BLINKING_ON);
          state.current = CONNECTION_MANAGER_LEAVING_BLINKING_ON;
          state.blinkCount--;
          LED_OFF
          LED_COLOR_RED
          sl_zigbee_event_set_delay_ms(&leaving_event,
                                       CONN_MAN_BLINK_DURATION_MS);
      } else {
          LOG_TRANSITION(CONNECTION_MANAGER_LEAVE);
          state.current = CONNECTION_MANAGER_LEAVE;
          blinkGreen();
      }
      break;

    case CONNECTION_MANAGER_LEAVE:
      // leave network
      networkHandlersLeaveNetwork();
      LOG_TRANSITION(CONNECTION_MANAGER_IDLE);
      state.current = CONNECTION_MANAGER_IDLE;
      // double cooldown time
      state.coolDownTick = sl_sleeptimer_get_tick_count()
                           + sl_sleeptimer_ms_to_tick(
                               CONN_MAN_COOL_DOWN_PERIOD_MS);
      cleanUpAndStop();
      break;

    case CONNECTION_MANAGER_NWK_STATE_BLINKING_OFF:
      // indicate state status
      blinkNwkState(false);
      break;

    case CONNECTION_MANAGER_NWK_STATE_BLINKING_ON:
      // indicate state status
      blinkNwkState(true);
      break;

    default:
      LOG_TRANSITION(CONNECTION_MANAGER_IDLE);
      cleanUpAndStop();
  }
}


static void blinkGreen(void)
{
  state.current = CONNECTION_MANAGER_LEAVE;
  LED_ON
  LED_COLOR_GREEN
  sl_zigbee_event_set_delay_ms(&leaving_event,
                               CONN_MAN_BLINK_DURATION_MS * 2);

}


static bool isOnNetwork(void)
{
  EmberNetworkStatus nwkState = emberAfNetworkState();
  return nwkState == EMBER_JOINED_NETWORK
         || nwkState == EMBER_JOINED_NETWORK_S2S_INITIATOR
         || nwkState == EMBER_JOINED_NETWORK_S2S_TARGET;
}


static void blinkNwkState(bool blinkOn)
{
  if ( state.blinkCount ) {
      ConnManStateEnum_t newState = blinkOn ? CONNECTION_MANAGER_NWK_STATE_BLINKING_OFF :
                                              CONNECTION_MANAGER_NWK_STATE_BLINKING_ON;
      LOG_TRANSITION(newState);
      state.current = newState;
      state.blinkCount--;
      if ( blinkOn ) {
          LED_ON;
      } else {
          LED_OFF;
      }
      if ( isOnNetwork() ) {
          LED_COLOR_GREEN;
      } else {
          LED_COLOR_RED;
      }
      sl_zigbee_event_set_delay_ms(&leaving_event,
                                   CONN_MAN_BLINK_DURATION_MS >> 2);
  } else {
      cleanUpAndStop();
      state.coolDownTick = sl_sleeptimer_get_tick_count();
  }
}
