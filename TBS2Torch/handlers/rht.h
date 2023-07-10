/*
 * rht.h
 *
 *  Created on: Jun 20, 2023
 *      Author: achetroi
 */

#ifndef HANDLERS_RHT_H_
#define HANDLERS_RHT_H_

/** @brief Initialize Relative Humidity and Temperature handler
 *
 */
void handlerRhtInit(void);


/* @brief update measurements
 *
 * @note Reads current temperature and humidity and updates attributes
 */
sl_status_t handlerRhtUpdate(void);


/* @brief change update interval
 *
 */
sl_status_t handlerRhtSetUpdateIntervalMs(uint32_t interval);

#endif /* HANDLERS_RHT_H_ */
