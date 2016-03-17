#ifndef _WRITE_H_
#define _WRITE_H_

#include "deca_device_api.h"

extern void _clear(void);
extern void _write_a(int offset, uint8 val);
extern void _write_h1(int offset, uint8 val);
extern void _write_h2(int offset, uint8 v);
extern void _write_s(int offset, int len, uint8 * str);
extern void write_a(int offset, uint8 val);
extern void write_s(int offset, int len, uint8 * str);
extern void write_c(int offset, uint8 str);
extern void write_h2(int offset, uint8 v);
extern void write_hn(int offset, int len, uint8 v[]);

#endif
