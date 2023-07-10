/*
 * imu.h
 *
 *  Created on: Jun 24, 2023
 *      Author: achetroi
 */

#ifndef HANDLERS_SHAKER_H_
#define HANDLERS_SHAKER_H_

typedef enum {
  SHAKING_IDLE,
  SHAKING_START,
  SHAKING_STILL_SHAKING,
  SHAKING_STOP
} HandlerShakerShakingState_t;

void handlerShakerInit(void);

#endif /* HANDLERS_SHAKER_H_ */
