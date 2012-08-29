#include "config.h"
#include <avr/io.h>

void depth_init()
{
	DEPTH_PROBE_DDR = 0X00;
	DEPTH_PROBE_PORT = 0X01;
}