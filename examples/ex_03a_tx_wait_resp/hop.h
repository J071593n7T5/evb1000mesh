#ifndef _HOP_H_
#define _HOP_H_

#include <stdbool.h>
#include "deca_device_api.h"
#include "auxiliary.h"

extern void clearHop(FRAME * f);
extern void addHop(FRAME * f, uint8 hop);
extern bool isExistingHop(FRAME * f, uint8 hop);

#endif
