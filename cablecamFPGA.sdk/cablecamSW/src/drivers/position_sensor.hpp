/*
 * position_sensor.hpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DRIVERS_POSITION_SENSOR_HPP_
#define SRC_DRIVERS_POSITION_SENSOR_HPP_

#include "xil_types.h"
#include "xil_io.h"

class PositionSensor
{
	uintptr_t baseaddress;
	int32_t position;
public:
	PositionSensor(uintptr_t p_baseaddress);
	int32_t GetPosition();
};



#endif /* SRC_DRIVERS_POSITION_SENSOR_HPP_ */
