/*
 * controller_state.cpp
 *
 *  Created on: Apr 5, 2021
 *      Author: Nik Clark
 */

#include "controller_state.hpp"

constexpr uint32_t maxDriveSpeed = 170000;

void HandController::setYaw(uint32_t val)
{
	uint32_t scaledval = val/100;
	if( scaledval >= 1000 && scaledval <= 2000 )
		cameraYaw = 3000 - scaledval;
}

void HandController::setPitch(uint32_t val)
{
	uint32_t scaledval = val/100;
	if( scaledval >= 1000 && scaledval <= 2000 )
		cameraPitch = scaledval;
}

void HandController::setEndpointSwitch(uint32_t val)
{
	if( val >= 1000 && val <= 2000 )
		endpointSwitch = val;
}

void HandController::setControlSwitch(uint32_t val)
{
	if( val >= 1000 && val <= 2000 )
		controlSwitch = val;
}

void HandController::setDriveMotor(uint32_t val)
{
	if( val >= 100000 && val <= 200000 && val <= maxDriveSpeed )
	{
		if( val != 168726 && val != 168725 ) //The controller randomly sends out blips like this
			driveMotor = val;
	}
}

void HandController::setAutoSpeed(uint32_t val)
{
	if( val >= 100000 && val <= 200000 && val <=maxDriveSpeed )
			autospeed = val;
}
