#include "hello_sim.h"

bool SimHelloMonitor::_sleep(uint64_t duration_ms)
{
	if(TIMER_DEBUG)
        std::cout << "[TIMER_DEBUG][SimHelloMonitor]: Called _sleep() with duration_ms: " << duration_ms << std::endl;

    waitSimulatedTime(duration_ms);

    return true;
}

uint64_t SimHelloMonitor::_getCurrentTimeMS()
{
    return getSimulatedTime();
}
