#include "LED_APH_SHData.h"

// Turn on the LEDs
void LED_APH_SHData::runReceiveHandler(Message* pMsg) {
	lightLed(BLUE_LED_PIN, LED_BLINK);
}

// Turn on more LEDs...
void LED_APH_SHData::runSendHandler(int nBytesSent, char* pMsg) {
	if(nBytesSent > 0) {
		// Successfully sent bytes
		lightLed(BLUE_LED_PIN, LED_BLINK);
	}
	else if(nBytesSent < 0) {
		// Failed to send bytes
		lightLed(RED_LED_PIN, LED_BLINK);
	}
	else {
		// Something went wrong...
		lightLed(YELLOW_LED_PIN, LED_BLINK);
	}
}
