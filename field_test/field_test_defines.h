#include "defines.h"

#define LED_AODV_DEBUG	1 && DEBUG
#define LED_SH_DEBUG	1 && DEBUG
#define LED_DEBUG 		1 && DEBUG

// If we actually flash that LED
#define HELLO_LED_ON 1
#define AODV_LED_ON 1
#define OTHER_LED_ON 1

// Pins to use
#define BLUE_LED_PIN	18
#define YELLOW_LED_PIN	19
#define RED_LED_PIN		20

#define HELLO_LED_PIN	BLUE_LED_PIN
#define AODV_LED_PIN	YELLOW_LED_PIN
#define OTHER_LED_PIN	RED_LED_PIN

// How long to flash for
#define LIGHT_MS 1
#define LED_BLINK		50
#define LED_HOLD		1000
