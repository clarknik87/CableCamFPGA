/*
 * position_sensor.cpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Nik Clark
 */

#include "position_sensor.hpp"

static constexpr int POSITION_SENSOR_POSITION_REG	= 0;
static constexpr int POSITION_SENSOR_RESERVED0_REG	= 4;
static constexpr int POSITION_SENSOR_RESERVED1_REG	= 8;
static constexpr int POSITION_SENSOR_RESERVED2_REG	= 12;

PositionSensor::PositionSensor(uintptr_t p_baseaddress)
{
	baseaddress = p_baseaddress;
}

int32_t PositionSensor::GetPosition()
{
	position = (int32_t)Xil_In32(baseaddress+POSITION_SENSOR_POSITION_REG);
	return position;

	//If stored as sign and magnitude instead.
	/*
	uint32_t raw  = Xil_In32(baseaddress+POSITION_SENSOR_POSITION_REG);
	uint32_t mag  = raw & 0x7FFFFFFF;
	uint32_t sign = (raw & 0x80000000);
	if( sign == 0 )
		position = mag;
	else
		position = -mag;
	*/
}
