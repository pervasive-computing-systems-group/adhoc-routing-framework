#include "LED_APH_SHData.h"

// Turn on the LEDs
void LED_APH_SHData::runHandler(Message* pMsg) {
	lightLed(BLUE_LED_PIN, LED_BLINK);
}
