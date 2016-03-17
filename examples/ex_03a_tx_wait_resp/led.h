#ifndef _LED_H_
#define _LED_H_

extern void led_set(led_t led, bool val);
extern void led_watchdog0(int prescaler);
extern void led_watchdog1(void);
extern void led_watchdog2(void);
extern void led_watchdog3(void);

#endif
