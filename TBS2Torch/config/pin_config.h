#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[ADC0]
// [ADC0]$

// $[CMU]
// [CMU]$

// $[DBG]
// DBG SWV on PF2
#ifndef DBG_SWV_PORT                            
#define DBG_SWV_PORT                             gpioPortF
#endif
#ifndef DBG_SWV_PIN                             
#define DBG_SWV_PIN                              2
#endif
#ifndef DBG_SWV_LOC                             
#define DBG_SWV_LOC                              0
#endif

// [DBG]$

// $[ETM]
// [ETM]$

// $[PTI]
// PTI DFRAME on PB13
#ifndef PTI_DFRAME_PORT                         
#define PTI_DFRAME_PORT                          gpioPortB
#endif
#ifndef PTI_DFRAME_PIN                          
#define PTI_DFRAME_PIN                           13
#endif
#ifndef PTI_DFRAME_LOC                          
#define PTI_DFRAME_LOC                           6
#endif

// PTI DOUT on PB12
#ifndef PTI_DOUT_PORT                           
#define PTI_DOUT_PORT                            gpioPortB
#endif
#ifndef PTI_DOUT_PIN                            
#define PTI_DOUT_PIN                             12
#endif
#ifndef PTI_DOUT_LOC                            
#define PTI_DOUT_LOC                             6
#endif

// [PTI]$

// $[GPIO]
// [GPIO]$

// $[I2C0]
// [I2C0]$

// $[I2C1]
// I2C1 SCL on PC5
#ifndef I2C1_SCL_PORT                           
#define I2C1_SCL_PORT                            gpioPortC
#endif
#ifndef I2C1_SCL_PIN                            
#define I2C1_SCL_PIN                             5
#endif
#ifndef I2C1_SCL_LOC                            
#define I2C1_SCL_LOC                             17
#endif

// I2C1 SDA on PC4
#ifndef I2C1_SDA_PORT                           
#define I2C1_SDA_PORT                            gpioPortC
#endif
#ifndef I2C1_SDA_PIN                            
#define I2C1_SDA_PIN                             4
#endif
#ifndef I2C1_SDA_LOC                            
#define I2C1_SDA_LOC                             17
#endif

// [I2C1]$

// $[IDAC0]
// [IDAC0]$

// $[LESENSE]
// [LESENSE]$

// $[LETIMER0]
// [LETIMER0]$

// $[LEUART0]
// [LEUART0]$

// $[LFXO]
// [LFXO]$

// $[MODEM]
// [MODEM]$

// $[PCNT0]
// [PCNT0]$

// $[PCNT1]
// [PCNT1]$

// $[PCNT2]
// [PCNT2]$

// $[PRS.CH0]
// [PRS.CH0]$

// $[PRS.CH1]
// [PRS.CH1]$

// $[PRS.CH2]
// [PRS.CH2]$

// $[PRS.CH3]
// [PRS.CH3]$

// $[PRS.CH4]
// [PRS.CH4]$

// $[PRS.CH5]
// [PRS.CH5]$

// $[PRS.CH6]
// [PRS.CH6]$

// $[PRS.CH7]
// [PRS.CH7]$

// $[PRS.CH8]
// [PRS.CH8]$

// $[PRS.CH9]
// [PRS.CH9]$

// $[PRS.CH10]
// [PRS.CH10]$

// $[PRS.CH11]
// [PRS.CH11]$

// $[TIMER0]
// TIMER0 CC0 on PD11
#ifndef TIMER0_CC0_PORT                         
#define TIMER0_CC0_PORT                          gpioPortD
#endif
#ifndef TIMER0_CC0_PIN                          
#define TIMER0_CC0_PIN                           11
#endif
#ifndef TIMER0_CC0_LOC                          
#define TIMER0_CC0_LOC                           19
#endif

// TIMER0 CC1 on PD12
#ifndef TIMER0_CC1_PORT                         
#define TIMER0_CC1_PORT                          gpioPortD
#endif
#ifndef TIMER0_CC1_PIN                          
#define TIMER0_CC1_PIN                           12
#endif
#ifndef TIMER0_CC1_LOC                          
#define TIMER0_CC1_LOC                           19
#endif

// TIMER0 CC2 on PD13
#ifndef TIMER0_CC2_PORT                         
#define TIMER0_CC2_PORT                          gpioPortD
#endif
#ifndef TIMER0_CC2_PIN                          
#define TIMER0_CC2_PIN                           13
#endif
#ifndef TIMER0_CC2_LOC                          
#define TIMER0_CC2_LOC                           19
#endif

// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[USART0]
// USART0 CTS on PA2
#ifndef USART0_CTS_PORT                         
#define USART0_CTS_PORT                          gpioPortA
#endif
#ifndef USART0_CTS_PIN                          
#define USART0_CTS_PIN                           2
#endif
#ifndef USART0_CTS_LOC                          
#define USART0_CTS_LOC                           30
#endif

// USART0 RTS on PA3
#ifndef USART0_RTS_PORT                         
#define USART0_RTS_PORT                          gpioPortA
#endif
#ifndef USART0_RTS_PIN                          
#define USART0_RTS_PIN                           3
#endif
#ifndef USART0_RTS_LOC                          
#define USART0_RTS_LOC                           30
#endif

// USART0 RX on PA1
#ifndef USART0_RX_PORT                          
#define USART0_RX_PORT                           gpioPortA
#endif
#ifndef USART0_RX_PIN                           
#define USART0_RX_PIN                            1
#endif
#ifndef USART0_RX_LOC                           
#define USART0_RX_LOC                            0
#endif

// USART0 TX on PA0
#ifndef USART0_TX_PORT                          
#define USART0_TX_PORT                           gpioPortA
#endif
#ifndef USART0_TX_PIN                           
#define USART0_TX_PIN                            0
#endif
#ifndef USART0_TX_LOC                           
#define USART0_TX_LOC                            0
#endif

// [USART0]$

// $[USART1]
// [USART1]$

// $[USART2]
// [USART2]$

// $[USART3]
// USART3 CLK on PC2
#ifndef USART3_CLK_PORT                         
#define USART3_CLK_PORT                          gpioPortC
#endif
#ifndef USART3_CLK_PIN                          
#define USART3_CLK_PIN                           2
#endif
#ifndef USART3_CLK_LOC                          
#define USART3_CLK_LOC                           18
#endif

// USART3 RX on PC1
#ifndef USART3_RX_PORT                          
#define USART3_RX_PORT                           gpioPortC
#endif
#ifndef USART3_RX_PIN                           
#define USART3_RX_PIN                            1
#endif
#ifndef USART3_RX_LOC                           
#define USART3_RX_LOC                            18
#endif

// USART3 TX on PC0
#ifndef USART3_TX_PORT                          
#define USART3_TX_PORT                           gpioPortC
#endif
#ifndef USART3_TX_PIN                           
#define USART3_TX_PIN                            0
#endif
#ifndef USART3_TX_LOC                           
#define USART3_TX_LOC                            18
#endif

// [USART3]$

// $[VDAC0]
// [VDAC0]$

// $[WTIMER0]
// [WTIMER0]$

// $[WTIMER1]
// [WTIMER1]$

// $[CUSTOM_PIN_NAME]
// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

// $[CSEN]
// [CSEN]$

