/*
 * pwm_generator.cpp
 *
 *  Created on: Apr 6, 2021
 *      Author: Nik Clark
 */

#include "pwm_generator.hpp"

static constexpr int PWM_GENERATOR_SPEED_REG_OFFSET		= 0;
static constexpr int PWM_GENERATOR_ID_REG_OFFSET		= 12;

PWMGenerator::PWMGenerator(uintptr_t p_baseaddress)
{
	baseaddress = p_baseaddress;
}

void PWMGenerator::SetSpeed(uint32_t speed)
{
	if(speed >= 100000 && speed <= 200000)
	{
		Xil_Out32(baseaddress + PWM_GENERATOR_SPEED_REG_OFFSET, speed);
	}
}

void PWMGenerator::StopDriveMotor()
{
	Xil_Out32(baseaddress + PWM_GENERATOR_SPEED_REG_OFFSET, 150000);
}

char* PWMGenerator::ReadID()
{
	u32 raw;
	raw = Xil_In32(baseaddress + PWM_GENERATOR_ID_REG_OFFSET);

	IDstr[0] = (raw & 0x000000FF) >> 0;
	IDstr[1] = (raw & 0x0000FF00)	>> 8;
	IDstr[2] = (raw & 0x00FF0000) >> 16;
	IDstr[3] = (raw & 0xFF000000) >> 24;

	return IDstr;
}
