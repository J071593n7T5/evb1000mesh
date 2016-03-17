#include <stdbool.h>

#include "deca_device_api.h"
#include "port.h"

#include "led.h"

// LED usage:
// (from left to right)
// LED_PC8
// LED_PC6
// LED_PC9
// LED_PC7

extern void
led_set(led_t led, bool val)
{
	if (val)
	{
		led_on(led);
	}
	else
	{
		led_off(led);
	}
}

extern void
led_watchdog0(int prescaler)
{
	static uint16 counter = 0;

	counter++;
	led_set(LED_PC7, ((counter >> prescaler) % 2) == 0);
}
extern void
led_watchdog1(void)
{
	static bool b = false;

	b = !b;
	led_set(LED_PC9, b);
}
extern void
led_watchdog2(void)
{
	static bool b = false;

	b = !b;
	led_set(LED_PC6, b);
}
extern void
led_watchdog3(void)
{
	static bool b = false;

	b = !b;
	led_set(LED_PC8, b);
}
