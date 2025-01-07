/***************************************************************************//**
 * @file
 * @brief sl_battery_monitor Config
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_BATTERY_MONITOR_CONFIG_H
#define SL_BATTERY_MONITOR_CONFIG_H

#include "em_adc.h"

// <<< Use Configuration Wizard in Context Menu

// <h> Battery Monitor settings

// <o SL_BATTERY_MONITOR_TIMEOUT_MINUTES> Monitor Timeout (Minutes)
// <1..1000:1>
// <i> Default: 30
// <i> The length of time between battery reads.
#define SL_BATTERY_MONITOR_TIMEOUT_MINUTES 60

// <o SL_BATTERY_MONITOR_SAMPLE_FIFO_SIZE> Sample Collection FIFO Size
// <1..20:1>
// <i> Default: 16
// <i> The number of entries in the fifo used to collect ADC reads of the battery
#define SL_BATTERY_MONITOR_SAMPLE_FIFO_SIZE 16

// <o SL_BATTERY_MONITOR_ADC_POS_SEL> ADC Pos Select Input
// <i> Default: AVDD
// <adcPosSelAVDD=> AVDD
// <adcPosSelAPORT0XCH0=> APORT0XCH0
// <adcPosSelAPORT0XCH1=> APORT0XCH1
// <adcPosSelAPORT0XCH2=> APORT0XCH2
// <adcPosSelAPORT0XCH3=> APORT0XCH3
// <adcPosSelAPORT0XCH4=> APORT0XCH4
// <adcPosSelAPORT0XCH5=> APORT0XCH5
// <adcPosSelAPORT0XCH6=> APORT0XCH6
// <adcPosSelAPORT0XCH7=> APORT0XCH7
// <adcPosSelAPORT0XCH8=> APORT0XCH8
// <adcPosSelAPORT0XCH9=> APORT0XCH9
// <adcPosSelAPORT0XCH10=> APORT0XCH10
// <adcPosSelAPORT0XCH11=> APORT0XCH11
// <adcPosSelAPORT0XCH12=> APORT0XCH12
// <adcPosSelAPORT0XCH13=> APORT0XCH13
// <adcPosSelAPORT0XCH14=> APORT0XCH14
// <adcPosSelAPORT0XCH15=> APORT0XCH15
// <adcPosSelAPORT0YCH0=> APORT0YCH0
// <adcPosSelAPORT0YCH1=> APORT0YCH1
// <adcPosSelAPORT0YCH2=> APORT0YCH2
// <adcPosSelAPORT0YCH3=> APORT0YCH3
// <adcPosSelAPORT0YCH4=> APORT0YCH4
// <adcPosSelAPORT0YCH5=> APORT0YCH5
// <adcPosSelAPORT0YCH6=> APORT0YCH6
// <adcPosSelAPORT0YCH7=> APORT0YCH7
// <adcPosSelAPORT0YCH8=> APORT0YCH8
// <adcPosSelAPORT0YCH9=> APORT0YCH9
// <adcPosSelAPORT0YCH10=> APORT0YCH10
// <adcPosSelAPORT0YCH11=> APORT0YCH11
// <adcPosSelAPORT0YCH12=> APORT0YCH12
// <adcPosSelAPORT0YCH13=> APORT0YCH13
// <adcPosSelAPORT0YCH14=> APORT0YCH14
// <adcPosSelAPORT0YCH15=> APORT0YCH15
// <adcPosSelAPORT1XCH0=> APORT1XCH0
// <adcPosSelAPORT1YCH1=> APORT1YCH1
// <adcPosSelAPORT1XCH2=> APORT1XCH2
// <adcPosSelAPORT1YCH3=> APORT1YCH3
// <adcPosSelAPORT1XCH4=> APORT1XCH4
// <adcPosSelAPORT1YCH5=> APORT1YCH5
// <adcPosSelAPORT1XCH6=> APORT1XCH6
// <adcPosSelAPORT1YCH7=> APORT1YCH7
// <adcPosSelAPORT1XCH8=> APORT1XCH8
// <adcPosSelAPORT1YCH9=> APORT1YCH9
// <adcPosSelAPORT1XCH10=> APORT1XCH10
// <adcPosSelAPORT1YCH11=> APORT1YCH11
// <adcPosSelAPORT1XCH12=> APORT1XCH12
// <adcPosSelAPORT1YCH13=> APORT1YCH13
// <adcPosSelAPORT1XCH14=> APORT1XCH14
// <adcPosSelAPORT1YCH15=> APORT1YCH15
// <adcPosSelAPORT1XCH16=> APORT1XCH16
// <adcPosSelAPORT1YCH17=> APORT1YCH17
// <adcPosSelAPORT1XCH18=> APORT1XCH18
// <adcPosSelAPORT1YCH19=> APORT1YCH19
// <adcPosSelAPORT1XCH20=> APORT1XCH20
// <adcPosSelAPORT1YCH21=> APORT1YCH21
// <adcPosSelAPORT1XCH22=> APORT1XCH22
// <adcPosSelAPORT1YCH23=> APORT1YCH23
// <adcPosSelAPORT1XCH24=> APORT1XCH24
// <adcPosSelAPORT1YCH25=> APORT1YCH25
// <adcPosSelAPORT1XCH26=> APORT1XCH26
// <adcPosSelAPORT1YCH27=> APORT1YCH27
// <adcPosSelAPORT1XCH28=> APORT1XCH28
// <adcPosSelAPORT1YCH29=> APORT1YCH29
// <adcPosSelAPORT1XCH30=> APORT1XCH30
// <adcPosSelAPORT1YCH31=> APORT1YCH31
// <adcPosSelAPORT2YCH0=> APORT2YCH0
// <adcPosSelAPORT2XCH1=> APORT2XCH1
// <adcPosSelAPORT2YCH2=> APORT2YCH2
// <adcPosSelAPORT2XCH3=> APORT2XCH3
// <adcPosSelAPORT2YCH4=> APORT2YCH4
// <adcPosSelAPORT2XCH5=> APORT2XCH5
// <adcPosSelAPORT2YCH6=> APORT2YCH6
// <adcPosSelAPORT2XCH7=> APORT2XCH7
// <adcPosSelAPORT2YCH8=> APORT2YCH8
// <adcPosSelAPORT2XCH9=> APORT2XCH9
// <adcPosSelAPORT2YCH10=> APORT2YCH10
// <adcPosSelAPORT2XCH11=> APORT2XCH11
// <adcPosSelAPORT2YCH12=> APORT2YCH12
// <adcPosSelAPORT2XCH13=> APORT2XCH13
// <adcPosSelAPORT2YCH14=> APORT2YCH14
// <adcPosSelAPORT2XCH15=> APORT2XCH15
// <adcPosSelAPORT2YCH16=> APORT2YCH16
// <adcPosSelAPORT2XCH17=> APORT2XCH17
// <adcPosSelAPORT2YCH18=> APORT2YCH18
// <adcPosSelAPORT2XCH19=> APORT2XCH19
// <adcPosSelAPORT2YCH20=> APORT2YCH20
// <adcPosSelAPORT2XCH21=> APORT2XCH21
// <adcPosSelAPORT2YCH22=> APORT2YCH22
// <adcPosSelAPORT2XCH23=> APORT2XCH23
// <adcPosSelAPORT2YCH24=> APORT2YCH24
// <adcPosSelAPORT2XCH25=> APORT2XCH25
// <adcPosSelAPORT2YCH26=> APORT2YCH26
// <adcPosSelAPORT2XCH27=> APORT2XCH27
// <adcPosSelAPORT2YCH28=> APORT2YCH28
// <adcPosSelAPORT2XCH29=> APORT2XCH29
// <adcPosSelAPORT2YCH30=> APORT2YCH30
// <adcPosSelAPORT2XCH31=> APORT2XCH31
// <adcPosSelAPORT3XCH0=> APORT3XCH0
// <adcPosSelAPORT3YCH1=> APORT3YCH1
// <adcPosSelAPORT3XCH2=> APORT3XCH2
// <adcPosSelAPORT3YCH3=> APORT3YCH3
// <adcPosSelAPORT3XCH4=> APORT3XCH4
// <adcPosSelAPORT3YCH5=> APORT3YCH5
// <adcPosSelAPORT3XCH6=> APORT3XCH6
// <adcPosSelAPORT3YCH7=> APORT3YCH7
// <adcPosSelAPORT3XCH8=> APORT3XCH8
// <adcPosSelAPORT3YCH9=> APORT3YCH9
// <adcPosSelAPORT3XCH10=> APORT3XCH10
// <adcPosSelAPORT3YCH11=> APORT3YCH11
// <adcPosSelAPORT3XCH12=> APORT3XCH12
// <adcPosSelAPORT3YCH13=> APORT3YCH13
// <adcPosSelAPORT3XCH14=> APORT3XCH14
// <adcPosSelAPORT3YCH15=> APORT3YCH15
// <adcPosSelAPORT3XCH16=> APORT3XCH16
// <adcPosSelAPORT3YCH17=> APORT3YCH17
// <adcPosSelAPORT3XCH18=> APORT3XCH18
// <adcPosSelAPORT3YCH19=> APORT3YCH19
// <adcPosSelAPORT3XCH20=> APORT3XCH20
// <adcPosSelAPORT3YCH21=> APORT3YCH21
// <adcPosSelAPORT3XCH22=> APORT3XCH22
// <adcPosSelAPORT3YCH23=> APORT3YCH23
// <adcPosSelAPORT3XCH24=> APORT3XCH24
// <adcPosSelAPORT3YCH25=> APORT3YCH25
// <adcPosSelAPORT3XCH26=> APORT3XCH26
// <adcPosSelAPORT3YCH27=> APORT3YCH27
// <adcPosSelAPORT3XCH28=> APORT3XCH28
// <adcPosSelAPORT3YCH29=> APORT3YCH29
// <adcPosSelAPORT3XCH30=> APORT3XCH30
// <adcPosSelAPORT3YCH31=> APORT3YCH31
// <adcPosSelAPORT4YCH0=> APORT4YCH0
// <adcPosSelAPORT4XCH1=> APORT4XCH1
// <adcPosSelAPORT4YCH2=> APORT4YCH2
// <adcPosSelAPORT4XCH3=> APORT4XCH3
// <adcPosSelAPORT4YCH4=> APORT4YCH4
// <adcPosSelAPORT4XCH5=> APORT4XCH5
// <adcPosSelAPORT4YCH6=> APORT4YCH6
// <adcPosSelAPORT4XCH7=> APORT4XCH7
// <adcPosSelAPORT4YCH8=> APORT4YCH8
// <adcPosSelAPORT4XCH9=> APORT4XCH9
// <adcPosSelAPORT4YCH10=> APORT4YCH10
// <adcPosSelAPORT4XCH11=> APORT4XCH11
// <adcPosSelAPORT4YCH12=> APORT4YCH12
// <adcPosSelAPORT4XCH13=> APORT4XCH13
// <adcPosSelAPORT4YCH14=> APORT4YCH14
// <adcPosSelAPORT4XCH15=> APORT4XCH15
// <adcPosSelAPORT4YCH16=> APORT4YCH16
// <adcPosSelAPORT4XCH17=> APORT4XCH17
// <adcPosSelAPORT4YCH18=> APORT4YCH18
// <adcPosSelAPORT4XCH19=> APORT4XCH19
// <adcPosSelAPORT4YCH20=> APORT4YCH20
// <adcPosSelAPORT4XCH21=> APORT4XCH21
// <adcPosSelAPORT4YCH22=> APORT4YCH22
// <adcPosSelAPORT4XCH23=> APORT4XCH23
// <adcPosSelAPORT4YCH24=> APORT4YCH24
// <adcPosSelAPORT4XCH25=> APORT4XCH25
// <adcPosSelAPORT4YCH26=> APORT4YCH26
// <adcPosSelAPORT4XCH27=> APORT4XCH27
// <adcPosSelAPORT4YCH28=> APORT4YCH28
// <adcPosSelAPORT4XCH29=> APORT4XCH29
// <adcPosSelAPORT4YCH30=> APORT4YCH30
// <adcPosSelAPORT4XCH31=> APORT4XCH31
// <adcPosSelAVDD=> AVDD
// <adcPosSelBUVDD=> BUVDD
// <adcPosSelDVDD=> DVDD
// <adcPosSelPAVDD=> PAVDD
// <adcPosSelDECOUPLE=> DECOUPLE
// <adcPosSelIOVDD=> IOVDD
// <adcPosSelIOVDD1=> IOVDD1
// <adcPosSelVSP=> VSP
// <adcPosSelOPA2=> OPA2
// <adcPosSelTEMP=> TEMP
// <adcPosSelDAC0OUT0=> DAC0OUT0
// <adcPosSelR5VOUT=> R5VOUT
// <adcPosSelSP1=> SP1
// <adcPosSelSP2=> SP2
// <adcPosSelDAC0OUT1=> DAC0OUT1
// <adcPosSelSUBLSB=> SUBLSB
// <adcPosSelOPA3=> OPA3
// <adcPosSelDEFAULT=> DEFAULT
// <adcPosSelVSS=> VSS
#define SL_BATTERY_MONITOR_ADC_POS_SEL          adcPosSelAPORT3XCH14

// <e SL_BATTERY_MONITOR_R_DIVIDER_ENABLED> Battery Voltage is using R1/R2 resistive divider
#define SL_BATTERY_MONITOR_R_DIVIDER_ENABLED    0

// <o SL_BATTERY_MONITOR_R_DIVIDER_R1> R1 Resistive divider Input Voltage leg
#define SL_BATTERY_MONITOR_R_DIVIDER_R1         1000

// <o SL_BATTERY_MONITOR_R_DIVIDER_R2> R2 Resistive divider Ground leg
#define SL_BATTERY_MONITOR_R_DIVIDER_R2         1000
// </e>

// </h> end Battery Monitor config
// <<< end of configuration section >>>

#define SL_BATTERY_MONITOR_R_DIVIDER_R1_R2 (SL_BATTERY_MONITOR_R_DIVIDER_R1 \
                                            + SL_BATTERY_MONITOR_R_DIVIDER_R2)
#define SL_BATTERY_MONITOR_R_DIVIDER_COEF  SL_BATTERY_MONITOR_R_DIVIDER_R1_R2 \
                                           / SL_BATTERY_MONITOR_R_DIVIDER_R2


// <<< sl:start pin_tool >>>
// <prs gpio=true > SL_BATTERY_MONITOR_TX_ACTIVE
// $[PRS_SL_BATTERY_MONITOR_TX_ACTIVE]
#ifndef SL_BATTERY_MONITOR_TX_ACTIVE_CHANNEL    
#define SL_BATTERY_MONITOR_TX_ACTIVE_CHANNEL     0
#endif

// PRS CH0 on PC6
#ifndef SL_BATTERY_MONITOR_TX_ACTIVE_PORT       
#define SL_BATTERY_MONITOR_TX_ACTIVE_PORT        gpioPortC
#endif
#ifndef SL_BATTERY_MONITOR_TX_ACTIVE_PIN        
#define SL_BATTERY_MONITOR_TX_ACTIVE_PIN         6
#endif
#ifndef SL_BATTERY_MONITOR_TX_ACTIVE_LOC        
#define SL_BATTERY_MONITOR_TX_ACTIVE_LOC         8
#endif
// [PRS_SL_BATTERY_MONITOR_TX_ACTIVE]$
// <<< sl:end pin_tool >>>

#endif // SL_BATTERY_MONITOR_CONFIG_H