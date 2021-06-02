#include "LED_APH_SHData.h"

// Turn on the LEDs
void LED_APH_SHData::runReceiveHandler(Message* pMsg) {
	lightLed(BLUE_LED_PIN, LED_BLINK);
}

// Turn on more LEDs...
void LED_APH_SHData::runSendHandler(int nBytesSent, char* pMsg) {
	lightLed(BLUE_LED_PIN, LED_BLINK);
}
