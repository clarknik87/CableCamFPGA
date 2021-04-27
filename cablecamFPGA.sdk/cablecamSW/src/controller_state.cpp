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
	if( val >= 100000 && val <= 200000 )
	{
		if( (val > 149990 && val < 150010) || (val < 100010) || (val > 199990) )
			endpointSwitch = val;
	}
}

void HandController::setControlSwitch(uint32_t val)
{
	if( val >= 100000 && val <= 200000 )
	{
		if( val < 102000 || val > 19800 )
			controlSwitch = val;
	}
}

void HandController::setDriveMotor(uint32_t val)
{
	uint32_t scaledval = 150000;
	if( val >= 100000 && val <= 200000 )
	{
		if(!driveEnable)
		{
			if (driveMotor > 148000 && driveMotor < 152000)
			{
				if (val < 148000 || val > 152000)
				{
					driveCnt++;
				}
				else
				{
					driveCnt = 0;
				}

				if (driveCnt == 18)
				{
					driveEnable = true;
				}
			}
		}

		if(driveEnable)
		{
			if( val <= 148000 )
			{
				scaledval = (val - 100000)*(0.75) + 100000;
			}
			else if( val >= 152000 )
			{
				scaledval = (val - 152000)*0.333 + 159000;
			}
			else
			{
				driveEnable = false;
				scaledval = 150000;
			}

			driveMotor = scaledval;
		}
	}
}

void HandController::setAutoSpeed(uint32_t val)
{
	if( val >= 100000 && val <= 200000 )
	{	
		if( !autoEnable )
		{
			if( val > 102000 )
			{
				autoCnt++;
			}
			else
			{
				autoCnt = 0;
			}

			if( autoCnt == 18 )
			{
				autoEnable = true;
			}
		}

		if( autoEnable )
		{
			if( val > 102000 )
			{
				autospeed = (val - 102000)*(3.0/49.0) + 14000;
			}
			else
			{
				autoEnable = false;
				autospeed = 0;
			}
		}
	}
}

SwitchPosition HandController::getEndpointSwitch()
{
	if( endpointSwitch < 125000 )
		return SwitchPosition::up;
	else if( endpointSwitch > 175000 )
		return SwitchPosition::down;
	else
		return SwitchPosition::center;
}

SwitchPosition HandController::getControlSwitch()
{
	if( controlSwitch < 150000 )
		return SwitchPosition::up;
	else
		return SwitchPosition::down;
}
