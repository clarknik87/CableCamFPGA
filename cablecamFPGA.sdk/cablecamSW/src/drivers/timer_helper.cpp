/*
 * timer_helper.cpp
 *
 *  Created on: Apr 21, 2021
 *      Author: Nik Clark
 */

#include "timer_helper.hpp"

TimerHelper::TimerHelper(uint16_t deviceID)
{
	XTmrCtr_Initialize(&TimerDevice, deviceID);

	u32 options = XTmrCtr_GetOptions(&TimerDevice, 0);
	options |= XTC_CASCADE_MODE_OPTION;
	XTmrCtr_SetOptions(&TimerDevice, 0, options);

	XTmrCtr_Start(&TimerDevice,0);
}

uint64_t TimerHelper::getTicks()
{
	uint64_t ticks;
	ticks = (uint64_t)(XTmrCtr_GetValue(&TimerDevice,1)) << 32;
	ticks |= XTmrCtr_GetValue(&TimerDevice,0);
	return ticks;
}
