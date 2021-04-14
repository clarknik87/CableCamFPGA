/*
 * seven_segment.hpp
 *
 *  Created on: Apr 12, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DRIVERS_SEVEN_SEGMENT_HPP_
#define SRC_DRIVERS_SEVEN_SEGMENT_HPP_

#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

class SevenSegment
{
	uintptr_t baseaddress;
public:
	SevenSegment(uintptr_t p_baseaddress);
	void SetHex();
	void SetDec();
	void SetPointPosition(uint32_t pos);
	void DisplayValue(int32_t val);
};


#endif /* SRC_DRIVERS_SEVEN_SEGMENT_HPP_ */
