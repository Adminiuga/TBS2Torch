/*
 * network-steering.h
 *
 *  Created on: May 25, 2023
 *      Author: achetroi
 */

#ifndef HANDLERS_NETWORK_H_
#define HANDLERS_NETWORK_H_

#ifndef HANDLERS_NETWORK_MAX_REJOIN_ATTEMPTS
#define HANDLERS_NETWORK_MAX_REJOIN_ATTEMPTS 15
#endif
#ifndef HANDLERS_NETWORK_REJOIN_BACKOFF_S
#define HANDLERS_NETWORK_REJOIN_BACKOFF_S 5
#endif
#ifndef HANDLERS_NETWORK_MAX_REJOIN_BACKOFF_S
#define HANDLERS_NETWORK_MAX_REJOIN_BACKOFF_S 1800
#endif


void networkHandlersInit(void);
void networkHandlersFindAndBind(void);
void networkHandlersLeaveNetwork(void);
char * networkHandlersNetworkStateFriendlyName(EmberNetworkStatus networkState);
void networkHandlersIndicateNetworkStatus(void);


#endif /* HANDLERS_NETWORK_H_ */
