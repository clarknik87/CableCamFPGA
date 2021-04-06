/*
 * controller_state.hpp
 *
 *  Created on: Apr 4, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_CONTROLLER_STATE_HPP_
#define SRC_CONTROLLER_STATE_HPP_

#include <cstdint>

class HandController
{
	uint32_t driveMotor;
	uint32_t cameraYaw;
	uint32_t cameraPitch;
	uint32_t endpointSwitch;
	uint32_t controlSwitch;
public:
	uint32_t getYaw() 				{ return cameraYaw; }
	uint32_t getPitch() 			{ return cameraPitch; }
	uint32_t getDriveMotor() 		{ return driveMotor; }
	uint32_t getEndpointSwitch() 	{ return endpointSwitch; }
	uint32_t getControlSwitch() 	{ return controlSwitch; }

	void setYaw(uint32_t val);
	void setPitch(uint32_t val);
	void setDriveMotor(uint32_t val);
	void setEndpointSwitch(uint32_t val);
	void setControlSwitch(uint32_t val);
};


#endif /* SRC_CONTROLLER_STATE_HPP_ */
