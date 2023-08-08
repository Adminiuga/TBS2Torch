/*
 * network-steering.c
 *
 *  Created on: May 25, 2023
 *      Author: achetroi
 */

#include "app/framework/include/af.h"
#include "stack/config/token-stack.h"

#include "sl_component_catalog.h"
#include "sl_token_api.h"
#include "network-steering.h"
#include "find-and-bind-target.h"

#include "network.h"
#include "app-global.h"

#if defined(SL_CATALOG_LED0_PRESENT)
#include "led-blink.h"
#define COMMISSIONING_STATUS_LED 0
#else // !SL_CATALOG_LED0_PRESENT
#define handlerLedBlinkBlinkLedOn(time, led)
#define handlerLedBlinkBlinkLedOff(time, led)
#define handlerLedBlinkCountedBlink(count, time, led)
#define handlerLedBlinkPatternBlink(count, length, pattern, ledIndex)
#endif // SL_CATALOG_LED0_PRESENT


#define LED_BLINK_SHORT_MS         100
#define LED_BLINK_LONG_MS          750
#define LED_BLINK_IDENTIFY_MS      500
#define LED_BLINK_NETWORK_UP_COUNT 3
#define FIND_AND_BIND_DELAY_MS     1000

typedef struct {
  uint8_t  joinAttempt;         // ReJoin Attempt Number
  bool     isCurrentlySteering; // true if currently is trying to steer the network
  bool     haveNetworkToken;    // is there network token (join or rejoin)
  uint32_t currentChannel;      // current channel
} networkState_t;

static networkState_t networkState;

// local events
static sl_zigbee_event_t commissioning_led_event;
static sl_zigbee_event_t finding_and_binding_event;
static sl_zigbee_event_t network_seeker_joiner_event;

// forward declarations
static void networkHandlerAttemptToJoin(bool restart);
static void networkHandlerResetState(void);
static void rejoinNetwork(void);

// declarations for event handlers
static void networkSeekerJoinerHandler(sl_zigbee_event_t *event);

//----------------------
// Implemented Callbacks

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action. The framework
 * will always process the stack status after the callback returns.
 */
void emberAfStackStatusCallback(EmberStatus status)
{
  // Note, the ZLL state is automatically updated by the stack and the plugin.
  sl_zigbee_event_set_inactive(&finding_and_binding_event);

  if (status == EMBER_NETWORK_DOWN) {
    sl_zigbee_event_set_inactive(&commissioning_led_event);

    handlerLedBlinkBlinkLedOn(LED_BLINK_LONG_MS, COMMISSIONING_STATUS_LED);
    sl_zigbee_app_debug_print("Current Network State: SteeringInProgress: %x, ",
                              networkState.isCurrentlySteering);
    sl_zigbee_app_debug_print("JoinAttempt: %d, ",
                              networkState.joinAttempt);
    sl_zigbee_app_debug_print("Status: 0x%02x ", status);

    EmberNetworkStatus nwkState = emberAfNetworkState();
    sl_zigbee_app_debug_println("Network State: 0x%02x/%s ",
                                nwkState,
                                networkHandlersNetworkStateFriendlyName(nwkState));
    if ( !(networkState.isCurrentlySteering) ) {
        networkHandlerAttemptToJoin(false);
    }
  } else if (status == EMBER_NETWORK_UP) {
    emberSetPowerDescriptor(ZDO_POWER_DESCRIPTOR);
    networkState.joinAttempt = 0;
    handlerLedBlinkCountedBlink(LED_BLINK_NETWORK_UP_COUNT, LED_BLINK_SHORT_MS, COMMISSIONING_STATUS_LED);
    // delay finding & binding if network fluctuates
    sl_zigbee_event_set_delay_ms(&finding_and_binding_event,
                                 FIND_AND_BIND_DELAY_MS);
  }
}

/** @brief Complete network steering.
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt.
 *
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID.
 *
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network.
 *
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete.
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  networkState.isCurrentlySteering = false;
  sl_zigbee_app_debug_println("Network steering complete %s: 0x%02X",
                              (status == EMBER_SUCCESS) ? "successfully" : "unsuccessfully",
                              status);
  if (status != EMBER_SUCCESS) {
      networkHandlerAttemptToJoin(false);
  }
}


void networkHandlersLeaveNetwork(void)
{
  networkState.haveNetworkToken = false;
  networkState.currentChannel   = 0xFF;
  EmberNetworkStatus nwkState = emberAfNetworkState();
  sl_zigbee_app_debug_println("Leaving network. Current network state 0x%02x/%s",
                              nwkState,
                              networkHandlersNetworkStateFriendlyName(nwkState));
  emberLeaveNetwork();
}


void networkHandlersIndicateNetworkStatus(void)
{
  EmberNetworkStatus nwkState = emberAfNetworkState();

  if (nwkState == EMBER_NO_NETWORK
      || nwkState == EMBER_JOINED_NETWORK_NO_PARENT) {
      handlerLedBlinkBlinkLedOn(LED_BLINK_LONG_MS, COMMISSIONING_STATUS_LED);
  } else {
      handlerLedBlinkCountedBlink(LED_BLINK_NETWORK_UP_COUNT,
                                  LED_BLINK_SHORT_MS,
                                  COMMISSIONING_STATUS_LED);
  }
}


//---------------
// Event handlers

static void commissioning_led_event_handler(sl_zigbee_event_t *event)
{

  sl_zigbee_event_set_inactive(&commissioning_led_event);
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
      uint16_t identifyTime;
      emberAfReadServerAttribute(emberAfPrimaryEndpoint(),
                                 ZCL_IDENTIFY_CLUSTER_ID,
                                 ZCL_IDENTIFY_TIME_ATTRIBUTE_ID,
                                 (uint8_t *)&identifyTime,
                                 sizeof(identifyTime));
      if (identifyTime > 0) {
        // identify blink, each blink is 1s and blinker supports
        // up to 255 blinks. Break down identifyTime in number of blinks
        handlerLedBlinkCountedBlink((uint8_t) identifyTime & 0x00ff,
                                     LED_BLINK_IDENTIFY_MS,
                                     COMMISSIONING_STATUS_LED);

        sl_zigbee_event_set_delay_ms(&commissioning_led_event,
                                     1000*2);
      } else {
          // turn off status led
          handlerLedBlinkBlinkLedOff(0, COMMISSIONING_STATUS_LED);
      }
  }
}

/*
 * Handler to control network joining/steering
 */
static void networkSeekerJoinerHandler(sl_zigbee_event_t *event)
{
  sl_zigbee_app_debug_println("networkSeekerJoinHandler here");
  EmberStatus status;
  EmberNetworkStatus nwkState = emberAfNetworkState();

  switch (nwkState) {
    case EMBER_JOINED_NETWORK:
      sl_zigbee_event_set_active(&commissioning_led_event);
      break;

    case EMBER_NO_NETWORK:
      sl_zigbee_app_debug_println("networkSeekerJoiner: Starting network steering");
      networkState.isCurrentlySteering = true;
      status = emberAfPluginNetworkSteeringStart();
      sl_zigbee_app_debug_println("networkSeekerJoiner: Start status: 0x%02X",
                                  status);
      handlerLedBlinkBlinkLedOn(LED_BLINK_LONG_MS, COMMISSIONING_STATUS_LED);
      break;

    default:
      sl_zigbee_app_debug_println("networkSeekerJoiner: network state: %s",
                                  networkHandlersNetworkStateFriendlyName(nwkState));
  }
}

static void finding_and_binding_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(&finding_and_binding_event);
  EmberNetworkStatus networkState = emberAfNetworkState();
  if (networkState == EMBER_JOINED_NETWORK) {
    sl_zigbee_app_debug_println("Find and bind target start: 0x%02X",
                                emberAfPluginFindAndBindTargetStart(emberAfPrimaryEndpoint()));
    handlerLedBlinkCountedBlink(LED_BLINK_NETWORK_UP_COUNT,
                                LED_BLINK_SHORT_MS,
                                COMMISSIONING_STATUS_LED);
  } else {
      sl_zigbee_app_debug_print("Find and bind target is unavailable, while off network: %s, ",
                                networkHandlersNetworkStateFriendlyName(networkState));
      sl_zigbee_app_debug_println("attempting to join");
      networkHandlerAttemptToJoin(true);
  }
}

static void networkHandlerResetState()
{
  networkState.joinAttempt = 0;
  networkState.isCurrentlySteering = false;
}

/*************************
 * Public methods
 *************************/

/** @brief Init
 * Application init function
 */
void networkHandlersInit(void)
{
  sl_zigbee_event_init(&commissioning_led_event, commissioning_led_event_handler);
  sl_zigbee_event_init(&finding_and_binding_event, finding_and_binding_event_handler);
  sl_zigbee_event_init(&network_seeker_joiner_event, networkSeekerJoinerHandler);

  networkHandlerResetState();

  tokTypeStackNodeData data;
  memset(&data, 0xFF, sizeof(data));
  halCommonGetToken(&data, TOKEN_STACK_NODE_DATA);

  if (data.panId != 0 && data.panId != 0xFFFF
      && data.zigbeeNodeId != 0xFFFF && data.zigbeeNodeId != 0xFFFE) {
      networkState.haveNetworkToken = true;
      networkState.currentChannel   = data.radioFreqChannel;
  } else {
      networkState.haveNetworkToken = false;
      networkState.currentChannel   = 0xFF;
      sl_zigbee_event_set_active(&network_seeker_joiner_event);
  }
}

void networkHandlersFindAndBind(void)
{
  sl_zigbee_event_set_active(&finding_and_binding_event);
}

char * networkHandlersNetworkStateFriendlyName(EmberNetworkStatus networkState)
{
  switch (networkState) {
    case EMBER_NO_NETWORK:
      return "No Network";
      break;

    case EMBER_JOINING_NETWORK:
      return "Joining Network";
      break;

    case EMBER_JOINED_NETWORK:
      return "Joined Network";
      break;

    case EMBER_JOINED_NETWORK_NO_PARENT:
      return "Joined Network No Parent";
      break;

    case EMBER_JOINED_NETWORK_S2S_INITIATOR:
      return "Joined Network S2S Initiator";
      break;

    case EMBER_JOINED_NETWORK_S2S_TARGET:
      return "Joined Network S2S Target";
      break;

    case EMBER_LEAVING_NETWORK:
      return "Leaving Network";
      break;
  }

  return "Unknown network state";
}


static void networkHandlerAttemptToJoin(bool restart)
{
  sl_zigbee_event_set_inactive(&network_seeker_joiner_event);

  if (restart) networkHandlerResetState();

  if (networkState.joinAttempt > HANDLERS_NETWORK_MAX_REJOIN_ATTEMPTS) {
      sl_zigbee_app_debug_println("Exhausted all %d join attempts",
                                  networkState.joinAttempt);
      return;
  }

  // on first attempt, schedule immediately
  if (networkState.joinAttempt == 0) {
      sl_zigbee_app_debug_println("networkHandlerAttemptToJoin: schedule network steering");
      sl_zigbee_event_set_active(&network_seeker_joiner_event);
  } else {
      uint32_t delayS = HANDLERS_NETWORK_REJOIN_BACKOFF_S << (networkState.joinAttempt - 1)/3;
      delayS = (delayS < HANDLERS_NETWORK_MAX_REJOIN_BACKOFF_S) ? delayS : HANDLERS_NETWORK_MAX_REJOIN_BACKOFF_S;
      sl_zigbee_app_debug_println("Scheduling attempt #%d of network steering in %d seconds",
                                  networkState.joinAttempt+1,
                                  delayS);
      sl_zigbee_event_set_delay_ms(&network_seeker_joiner_event, delayS * 1000);
  }
  networkState.joinAttempt++;
}


static void rejoinNetwork(void)
{
  EmberStatus status = EMBER_ERR_FATAL;
  if ( 11 <= networkState.currentChannel && networkState.currentChannel <= 26) {
      sl_zigbee_app_debug_print("Rejoining on %d channel, ",
                                networkState.currentChannel);
      status = emberFindAndRejoinNetworkWithReason(true,
                                                   1 << networkState.currentChannel,
                                                   EMBER_REJOIN_DUE_TO_NO_PARENT);
      sl_zigbee_app_debug_println("rejoin status: 0x%02x", status);
  }

  if (status != EMBER_SUCCESS) {
      sl_zigbee_app_debug_print("Rejoining on all channels, ");
      status = emberFindAndRejoinNetworkWithReason(true,
                                                   EMBER_ALL_802_15_4_CHANNELS_MASK,
                                                   EMBER_REJOIN_DUE_TO_NO_PARENT);
      sl_zigbee_app_debug_println("rejoin status: 0x%02x", status);
  }
}
