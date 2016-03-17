#ifndef _PENDING_H_
#define _PENDING_H_

#include <stdbool.h>
#include "deca_device_api.h"
#include "auxiliary.h"

extern void pendingInit(void);
extern void pendingAdd(FRAME * f);
extern bool pendingHasFrames();
extern void pendingSend(void);

#endif
