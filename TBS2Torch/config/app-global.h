/*
 * app-global.h
 *
 *  Created on: May 21, 2023
 *      Author: achetroi
 */

#ifndef CONFIG_APP_GLOBAL_H_
#define CONFIG_APP_GLOBAL_H_

#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b
#define CLAMP(v, min, max) MIN(MAX(v, min), max)

// Fade out transition time in 10th of sec
#define FADEOUT_TRANSITION_TIME 5*10
// Fade in transition time in 10th of sec
#define FADEIN_TRANSITION_TIME 5*10

#endif /* CONFIG_APP_GLOBAL_H_ */
