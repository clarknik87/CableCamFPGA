/*
 * controller_state.hpp
 *
 *  Created on: Apr 4, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_CONTROLLER_STATE_HPP_
#define SRC_CONTROLLER_STATE_HPP_

#include <cstdint>

enum class SwitchPosition
{
	up,
	down,
	center
};

class HandController
{
	uint32_t driveMotor = 150000;
	uint32_t autospeed;
	uint32_t cameraYaw;
	uint32_t cameraPitch;
	uint32_t endpointSwitch;
	uint32_t controlSwitch;

	uint32_t driveCnt;
	bool	 driveEnable = false;

	uint32_t autoCnt;
	bool	 autoEnable = false;

public:
	uint32_t 		getYaw() 				{ return cameraYaw; }
	uint32_t 		getPitch() 				{ return cameraPitch; }
	uint32_t 		getDriveMotor() 		{ return driveMotor; }
	uint32_t 		getAutoSpeed()			{ return autospeed; }
	SwitchPosition 	getEndpointSwitch();
	SwitchPosition 	getControlSwitch();

	void setYaw(uint32_t val);
	void setPitch(uint32_t val);
	void setDriveMotor(uint32_t val);
	void setAutoSpeed(uint32_t val);
	void setEndpointSwitch(uint32_t val);
	void setControlSwitch(uint32_t val);
};


#endif /* SRC_CONTROLLER_STATE_HPP_ */
