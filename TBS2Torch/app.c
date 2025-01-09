/***************************************************************************//**
 * @file app.c
 * @brief Callbacks implementation and application specific code.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "stdio.h"
#include "string.h"

#include "app/framework/include/af.h"
#include "app/framework/include/af-types.h"
#include "zap-command-structs.h"
#include "app_button_press.h"
#include "sl_component_catalog.h"

#ifdef SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT
#include "network_test_config.h"
#endif //SL_CATALOG_ZIGBEE_NETWORK_TEST_PRESENT

#ifdef SL_CATALOG_ZIGBEE_ZLL_LEVEL_CONTROL_SERVER_PRESENT
#include "zll-level-control-server.h"
#define MIN_LEVEL EMBER_AF_PLUGIN_ZLL_LEVEL_CONTROL_SERVER_MINIMUM_LEVEL
#define MAX_LEVEL EMBER_AF_PLUGIN_ZLL_LEVEL_CONTROL_SERVER_MAXIMUM_LEVEL
#else // !SL_CATALOG_ZIGBEE_ZLL_LEVEL_CONTROL_SERVER_PRESENT
#include "level-control-config.h"
#define MIN_LEVEL EMBER_AF_PLUGIN_LEVEL_CONTROL_MINIMUM_LEVEL
#define MAX_LEVEL EMBER_AF_PLUGIN_LEVEL_CONTROL_MAXIMUM_LEVEL
#endif // SL_CATALOG_ZIGBEE_ZLL_LEVEL_CONTROL_SERVER_PRESENT

#ifndef EZSP_HOST
#include "config/zigbee_sleep_config.h"
#endif // EZSP_HOST

#include "rz_led_blink.h"
#include "rz_button.h"
#include "config/ota-client-policy-config.h"
#include "hw/drivers.h"
#include "app-global.h"
#include "handlers/network.h"
#include "handlers/hall.h"
#include "handlers/rht.h"
#include "handlers/shaker.h"


/********************
 * local handlers
 *******************/
static void btn0_short_press_handler();
static void btn0_short_multiple_press_handler(uint8_t count);
static void btn0_medium_press_handler();
static void btn0_long_press_handler();
static void btn1_short_press_handler();
static void btn1_short_multiple_press_handler(uint8_t count);
static void btn1_medium_press_handler();
static void btn1_long_press_handler();

//----------------------
// Implemented Callbacks

/** @brief Init
 * Application init function
 */
void emberAfMainInitCallback(void)
{
  rz_led_blink_init();
  rz_button_init();
  networkHandlersInit();
  handlerRhtInit();
  sl_status_t status = handlerHallInit();
  sl_zigbee_app_debug_println("Hall Sensor init status: 0x%02x", status);
  handlerShakerInit();
  emberSetPowerDescriptor(ZDO_POWER_DESCRIPTOR);
}

/** @brief Post Attribute Change
 *
 * This function is called by the application framework after it changes an
 * attribute value. The value passed into this callback is the value to which
 * the attribute was set by the framework.
 */
void emberAfPostAttributeChangeCallback(uint8_t endpoint,
                                        EmberAfClusterId clusterId,
                                        EmberAfAttributeId attributeId,
                                        uint8_t mask,
                                        uint16_t manufacturerCode,
                                        uint8_t type,
                                        uint8_t size,
                                        uint8_t* value)
{
  sl_zigbee_app_debug_print("%d Post attr change: ep: %d, cluster: 0x%04x, attr: 0x%04x, size: %d ",
                            TIMESTAMP_MS, endpoint, clusterId, attributeId, size);
  if ( size == 2 ) {
    sl_zigbee_app_debug_println("value: 0x%02x%02x", value[1], value[0]);
  } else {
    sl_zigbee_app_debug_println("value: 0x%02x", value[0]);
  }

  if (clusterId == ZCL_LEVEL_CONTROL_CLUSTER_ID
       && mask == CLUSTER_MASK_SERVER) {
    if (attributeId == ZCL_CURRENT_LEVEL_ATTRIBUTE_ID) {
      sl_zigbee_app_debug_println("Level from post attr change: %d", (uint8_t) *value);
      hal_rgb_led_set_brightness(CLAMP(value[0], MIN_LEVEL, MAX_LEVEL));
    } else if ( attributeId == ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID ) {
        // get the supposed on/off state
        uint8_t onOff;
        EmberAfStatus status = emberAfReadServerAttribute(endpoint,
                                                          ZCL_ON_OFF_CLUSTER_ID,
                                                          ZCL_ON_OFF_ATTRIBUTE_ID,
                                                          (uint8_t *) &onOff,
                                                          sizeof(onOff));
        if ( status != EMBER_ZCL_STATUS_SUCCESS ) {
            sl_zigbee_app_debug_println("Couldn't read current 'on/off' state: %s, forcing light off");
            hal_rgb_led_turnoff();
            return;
        }
        // update on/off state, since we obviously either transitioning or still
        // transitioning
        assert( 2 == size );
        // sync state, unless is off but still transitioning
        sl_zigbee_app_debug_print("Remaining time: %d, onOff is '%s':", *((uint16_t *) value), onOff ? "ON" : "OFF");
        if ( onOff || ! ( (uint16_t *) *value) ) {
          sl_zigbee_app_debug_print(" Syncing light state");
          hal_rgb_led_turnonoff(onOff);
        }
        sl_zigbee_core_debug_println("");
    }
  }
}

/** @brief On/off Cluster Server Post Init
 *
 * Following resolution of the On/Off state at startup for this endpoint, perform any
 * additional initialization needed; e.g., synchronize hardware state.
 *
 * @param endpoint Endpoint that is being initialized
 */
void emberAfPluginOnOffClusterServerPostInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.
  uint8_t onOff;
  if (emberAfReadServerAttribute(endpoint,
                                 ZCL_ON_OFF_CLUSTER_ID,
                                 ZCL_ON_OFF_ATTRIBUTE_ID,
                                 (uint8_t *) &onOff,
                                 sizeof(onOff))
      == EMBER_ZCL_STATUS_SUCCESS) {
      sl_zigbee_app_debug_println("Current OnOff is %d", onOff);
  }

  // hardware state sync
  emberAfPostAttributeChangeCallback(endpoint,
                                     ZCL_ON_OFF_CLUSTER_ID,
                                     ZCL_ON_OFF_ATTRIBUTE_ID,
                                     CLUSTER_MASK_SERVER,
                                     0,
                                     ZCL_INT8U_ATTRIBUTE_TYPE,
                                     sizeof(onOff),
                                     &onOff);
  handlerRhtUpdate();
}

/** @brief
 *
 * Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void)
{
  #ifndef EZSP_HOST
  sl_mac_calibrate_current_channel();
  #endif
}


/** @brief Shaking Callback for Stop
 *
 *  @note Called when shaking stops. If shaking was shorter than 1s,
 *  consider it as a toggle command
 *
 *  @param durationMs duration of shaking in MS
 */
void handlerShakerShakingStop(uint32_t durationMs) {
  if ( (700 < durationMs) && (durationMs < 1600) ) {
      sl_zigbee_app_debug_println("Toggling light on short shake of %dms",
                                  durationMs);
      emberAfOnOffClusterSetValueCallback(emberAfPrimaryEndpoint(),
                                          ZCL_TOGGLE_COMMAND_ID,
                                          false);
  }
}


/** @brief Button short duration press callback
 *
 * Called after repeated or single presses for a button
 *
 * @param button Button number
 * @param pressedTimes Button was pressed so many times in quick succession
 */
void rz_button_short_press_cb(uint8_t button)
{
  sl_zigbee_app_debug_println("Short button press hander for button %d",
                              button);
  switch (button) {
    case 0:
      btn0_short_press_handler();
      break;

    case 1:
      btn1_short_press_handler();
      break;
  }
}


/************************
 * btn0 short press hander:
 *  - on/off on short press
 *
 * @param counter
 */
static void btn0_short_press_handler()
{
  emberAfSetCommandEndpoints(emberAfPrimaryEndpoint(), emberAfPrimaryEndpoint());
  emberAfFillCommandOnOffClusterToggle();
  EmberStatus status = emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, emberAfGetNodeId());
  sl_zigbee_app_debug_println("Toggle command sent, status: 0x%02x", status);
}

static void btn1_short_press_handler()
{
  networkHandlersFindAndBind();
  #ifdef SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
  sl_zigbee_app_framework_force_wakeup();
  #endif //SL_CATALOG_ZIGBEE_FORCE_SLEEP_AND_WAKEUP_PRESENT
}

/** @brief Button medium duration press callback
 *
 * Called for medium duration presses
 *
 * @param button Button number
 */
void rz_button_medium_press_cb(uint8_t button)
{
  sl_zigbee_app_debug_println("Medium duration button press hander for button %d",
                              button);

  switch (button) {
    case 0:
      btn0_medium_press_handler();
      break;

    case 1:
      break;
  }
};


/** @brief medium press handler for btn0
 *
 * On medium press do a fadein/fadeout depending on the current on off state
 *
 */
static void btn0_medium_press_handler(void)
{
  // medium press will toggle with fadein/fadeout
  EmberAfStatus status;
  bool isStateOn;
  status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                      ZCL_ON_OFF_CLUSTER_ID,
                                      ZCL_ON_OFF_ATTRIBUTE_ID,
                                      (uint8_t *) &isStateOn,
                                      sizeof(isStateOn));
  if (status != EMBER_ZCL_STATUS_SUCCESS) {
      sl_zigbee_app_debug_println("Couldn't read current 'on/off' state to do fadein/fadeout");
      return;
  }

  // Get transition time on/off attribute
  uint16_t transitionTime;
  status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                      ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                      ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID,
                                      (uint8_t *) &transitionTime,
                                      sizeof(transitionTime));
  if (status == EMBER_ZCL_STATUS_UNSUPPORTED_ATTRIBUTE) {
      // Get On transition time or off Transition time,
      // if on_off_transition_time is not supported
      uint16_t attributeToGet;

      if (isStateOn) {
          attributeToGet = ZCL_ON_TRANSITION_TIME_ATTRIBUTE_ID;
      } else {
          attributeToGet = ZCL_OFF_TRANSITION_TIME_ATTRIBUTE_ID;
      }
      status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                          ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                          attributeToGet,
                                          (uint8_t *) &transitionTime,
                                          sizeof(transitionTime));
      if (status != EMBER_ZCL_STATUS_SUCCESS
          || transitionTime == 0x0000) {
          transitionTime = (isStateOn) ? FADEIN_TRANSITION_TIME : FADEOUT_TRANSITION_TIME;
      }
  }

  uint8_t targetLevel;
  if (isStateOn) {
      // we'll be transitioning to off state smoothly, as defined in app config
      targetLevel = MIN_LEVEL;
  } else {
      // we'll be transitioning to on level smoothly, as defined in app config2
      status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                          ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                          ZCL_ON_LEVEL_ATTRIBUTE_ID,
                                          (uint8_t *) &targetLevel,
                                          sizeof(targetLevel));
      if (status != EMBER_ZCL_STATUS_SUCCESS
          || targetLevel == 0xFF) {
          status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                              ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                              ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                              (uint8_t *) &targetLevel,
                                              sizeof(targetLevel));
      }
      if (status != EMBER_ZCL_STATUS_SUCCESS || targetLevel == 0x00) {
          targetLevel = MAX_LEVEL;
      }
  }

  // control fade in/fade out by a fake command data
  uint8_t cmd_data[5];
  MEMSET(cmd_data, 0, sizeof(cmd_data));
  EmberAfClusterCommand cmd;
  cmd.buffer = (uint8_t *) &cmd_data;
  cmd.bufLen = sizeof(cmd_data);
  cmd.clusterSpecific = true;
  cmd.commandId = ZCL_MOVE_TO_LEVEL_WITH_ON_OFF_COMMAND_ID;
  cmd.payloadStartIndex = 0;

  cmd_data[0] = targetLevel;
  emberAfCopyInt16u((uint8_t *) &(cmd_data[1]), 0, transitionTime);
  sl_zigbee_app_debug_println("Move to %d level from button for %s*0.1s, on/off: %d",
          targetLevel,
          transitionTime,
          isStateOn);

  emberAfLevelControlClusterMoveToLevelWithOnOffCallback(&cmd);
}

/** @brief Button long duration press callback
 *
 * Called for medium duration presses
 *
 * @param button Button number
 */
void rz_button_long_press_cb(uint8_t button)
{
  sl_zigbee_app_debug_println("Long duration button press hander for button %d",
                              button);

  switch (button) {
    case 0:
      btn0_long_press_handler();
      break;

    case 1:
      btn1_long_press_handler();
      break;
  }
};


/** @brief BTN0 Long press handler
 *
 * On Long press, set the color to white
 *
 */
static void btn0_long_press_handler(void)
{
  EmberAfStatus status;
  uint8_t currentLevel;
  status = emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                      ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                      ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                      (uint8_t *) &currentLevel,
                                      sizeof(currentLevel));
  if (status != EMBER_ZCL_STATUS_SUCCESS) {
      sl_zigbee_app_debug_println("Couldn't read current 'current level' state to do fadein/fadeout");
      return;
  }
  // ToDo: update current_x, current_y
  currentLevel = (currentLevel == 0x00) ? 1 : currentLevel;
  hal_rgb_led_set_rgbcolor(currentLevel, currentLevel, currentLevel);
}


/** @brief BTN1 Long press handler
 *
 * On Long press, leave network
 *
 */
static void btn1_long_press_handler(void)
{
  networkHandlersLeaveNetwork();
}

void sl_zigbee_common_rtos_wakeup_stack_task()
{
};


void emberAfPluginLevelControlClusterServerPostInitCallback(uint8_t endpoint)
{
  uint16_t remainingTime = 0;
  sl_zigbee_app_debug_println("%d Level Control cluster initialized.", TIMESTAMP_MS);

  // hardware state sync
  emberAfPostAttributeChangeCallback(endpoint,
                                     ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                     ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID,
                                     CLUSTER_MASK_SERVER,
                                     0,
                                     ZCL_INT16U_ATTRIBUTE_TYPE,
                                     sizeof(remainingTime),
                                     (uint8_t *) &remainingTime);
};
