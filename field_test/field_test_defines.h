#include "defines.h"

// Implemented routing protocols
#define USE_SINGLE_HOP		0
#define USE_AODV			1
// The routing protocol to use in field test (select from above)
#define RT_PROTOCOL			USE_AODV

#define LED_AODV_DEBUG	1 && DEBUG
#define LED_SH_DEBUG	1 && DEBUG
#define LED_DEBUG 		1 && DEBUG

// If we actually flash that LED
#define HELLO_LED_ON 1
#define AODV_LED_ON 1
#define OTHER_LED_ON 1

// Pins to use
#define HELLO_LED_PIN 18
#define AODV_LED_PIN 19
#define OTHER_LED_PIN 20

// How long to flash for
#define LIGHT_MS 1
