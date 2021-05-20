#ifndef HARDWARE_LED_SINGLE_HOP_H
#define HARDWARE_LED_SINGLE_HOP_H

#include <sstream>
#include <string>

#include "hardware_single_hop.h"
#include "field_test_defines.h"
#include "led_helper.h"

class HardwareLedSingleHop : public HardwareSingleHop
{
private:
	/**
	 * @brief Constructor helper which sets up the led pins
	 *
	 */
	void _hardwareLedAODV();

public:
	/*!
	 * @brief Construct a new HardwareLedAODV object
	 *
	 * @param ip the ip address of the current device
	 */
	HardwareLedSingleHop(uint32_t ip);
	HardwareLedSingleHop(const char* ip);
	~HardwareLedSingleHop();

	virtual int handlePackets() override;
};

#endif
