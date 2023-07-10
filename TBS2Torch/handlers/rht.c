/*
 * rht.c
 *
 *  Created on: Jun 20, 2023
 *      Author: achetroi
 */

#include "af.h"

#include "sl_sensor_rht.h"


#ifndef HANDLER_RHT_UPDATE_INTERVAL_S
#define HANDLER_RHT_UPDATE_INTERVAL_S 60
#endif
#ifndef HANDLER_RHT_DEFAULT_ENDPOINT
#define HANDLER_RHT_DEFAULT_ENDPOINT 2
#endif


// ----------------------------------------------
static sl_zigbee_event_t read_data_event;

// ----------------------------------------------
// Local module variables
static uint32_t updateInterval = HANDLER_RHT_UPDATE_INTERVAL_S * 1000;
static bool initialized        = false;


// ----------------------------------------------
// Forward declarations
static void read_data_event_handler(sl_zigbee_event_t *event);
static uint8_t findRhtEndpoint(void);
static sl_status_t pushRhtMeasurement(uint32_t *rh, int32_t *temp);
static uint32_t filter_rh(uint32_t new_rh);
static int32_t filter_temp(int32_t new_rh);


// ----------------------------------------------
// Public functions
void handlerRhtInit(void)
{
  sl_zigbee_event_init(&read_data_event, read_data_event_handler);
  sl_zigbee_event_set_delay_ms(&read_data_event, updateInterval);

  sl_status_t status = sl_sensor_rht_init();
  sl_zigbee_app_debug_println("Initialized RHT sensor: 0x%04x", status);

  initialized = (SL_STATUS_OK == status);
}


sl_status_t handlerRhtUpdate(void)
{
  if ( !initialized ) {
      sl_zigbee_app_debug_println("RHT sensor is not initialized");
      return SL_STATUS_NOT_READY;
  }
  uint32_t rh_raw;
  int32_t temp_raw;

  sl_status_t status = sl_sensor_rht_get(&rh_raw, &temp_raw);
  if ( status == SL_STATUS_OK) {
      uint32_t rh   = filter_rh((rh_raw / 100) * 10);
      int32_t  temp = filter_temp((temp_raw / 100) * 10);
     sl_zigbee_app_debug_print("RHT Humidity: raw %d, filtered %d ",
                               (rh_raw / 100) * 10, rh);
     sl_zigbee_app_debug_println(" Temperature raw: %d, filtered %d",
                                 (temp_raw / 100) * 10, temp);
     status = pushRhtMeasurement(&rh, &temp);
  } else {
     sl_zigbee_app_debug_println("Couldn't read RHT sensor: 0x%04x", status);
  }

  return status;
}


// -------------------------------------------------
// Local functions
static void read_data_event_handler(sl_zigbee_event_t *event)
{
  sl_zigbee_event_set_inactive(&read_data_event);
  handlerRhtUpdate();
  sl_zigbee_event_set_delay_ms(&read_data_event, updateInterval);
}


static uint8_t findRhtEndpoint(void)
{
  uint8_t ep_count = emberAfEndpointCount();
  uint8_t ep;

  for(uint8_t ep_idx = 0; ep_idx < ep_count; ep_idx++) {
      ep = emberAfEndpointFromIndex(ep_idx);
      if (emberAfContainsServer(ep, ZCL_TEMP_MEASUREMENT_CLUSTER_ID)) {
          return ep;
      }
  }

  return HANDLER_RHT_DEFAULT_ENDPOINT;
}


static sl_status_t pushRhtMeasurement(uint32_t *rh, int32_t *temp)
{
  uint8_t endpoint = findRhtEndpoint();

  EmberAfStatus rh_status, temp_status;
  rh_status = emberAfWriteServerAttribute(
      endpoint,
      ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,
      ZCL_RELATIVE_HUMIDITY_MEASURED_VALUE_ATTRIBUTE_ID,
      (uint8_t*) rh,
      ZCL_INT16U_ATTRIBUTE_TYPE);

  temp_status = emberAfWriteServerAttribute(
      endpoint,
      ZCL_TEMP_MEASUREMENT_CLUSTER_ID,
      ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID,
      (uint8_t*) temp,
      ZCL_INT16S_ATTRIBUTE_TYPE);

  return (EMBER_ZCL_STATUS_SUCCESS == rh_status
          || EMBER_ZCL_STATUS_SUCCESS == temp_status) ? SL_STATUS_OK : SL_STATUS_FAIL;


}


static uint32_t filter_rh(uint32_t new_rh)
{
  static uint32_t buf[3] = {0, 0, 0};

  // Initialize
  if (buf[0] == 0 && buf[1] == 0 && buf[2] == 0) {
      buf[0] = buf[1] = buf[2] = new_rh;
      return new_rh;
  }

  buf[2] = buf[1]; buf[1] = buf[0]; buf[0] = new_rh;

  return (buf[0] + buf[1]*2 + buf[2]*3 ) / 6;
}


static int32_t filter_temp(int32_t new_temp)
{
  static int32_t buf[3] = {-1000, -1000, -1000};

  // Initialize
  if (buf[0] == -1000 && buf[1] == -1000 && buf[2] == -1000) {
      buf[0] = buf[1] = buf[2] = new_temp;
      return new_temp;
  }

  buf[2] = buf[1]; buf[1] = buf[0]; buf[0] = new_temp;

  return (buf[0] + buf[1]*2 + buf[2]*3 ) / 6;
}
