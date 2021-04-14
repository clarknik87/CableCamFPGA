/*
 * seven_segment.cpp
 *
 *  Created on: Apr 12, 2021
 *      Author: Nik Clark
 */

#include "seven_segment.hpp"

static constexpr int SEVEN_SEGMENT_VALUE_REG	= 0;
static constexpr int SEVEN_SEGMENT_RADIX_REG	= 4;
static constexpr int SEVEN_SEGMENT_DECPOS_REG	= 8;
static constexpr int SEVEN_SEGMENT_RESERVED_REG	= 12;

SevenSegment::SevenSegment(uintptr_t p_baseaddress)
{
	baseaddress = p_baseaddress;
	SetDec();
	SetPointPosition(0x0);
	DisplayValue(0);
}

void SevenSegment::SetHex()
{
	Xil_Out32(baseaddress+SEVEN_SEGMENT_RADIX_REG, 0x0);
}

void SevenSegment::SetDec()
{
	Xil_Out32(baseaddress+SEVEN_SEGMENT_RADIX_REG, 0x1);
}

void SevenSegment::SetPointPosition(uint32_t pos)
{
	Xil_Out32(baseaddress+SEVEN_SEGMENT_DECPOS_REG, pos);
}

void SevenSegment::DisplayValue(int32_t val)
{
	if(val > -1000 && val < 1000)
	{
		if(val < 0)
		{
			val *= -1;
			SetPointPosition(3);
		}
		else
		{
			val *= 1;
			SetPointPosition(0);
		}
		Xil_Out32(baseaddress+SEVEN_SEGMENT_VALUE_REG, (uint32_t)(val));
	}
}
