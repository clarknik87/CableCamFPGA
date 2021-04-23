/*
 * timer_helper.hpp
 *
 *  Created on: Apr 21, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DRIVERS_TIMER_HELPER_HPP_
#define SRC_DRIVERS_TIMER_HELPER_HPP_

#include "xtmrctr.h"
#include "xil_types.h"
#include <cstdint>

class TimerHelper
{
	XTmrCtr TimerDevice;
public:
	TimerHelper(uint16_t deviceID);
	uint64_t getTicks();
};

extern TimerHelper globalTimer;

#endif /* SRC_DRIVERS_TIMER_HELPER_HPP_ */
