/*
 * pwm_interpreter.cpp
 *
 *  Created on: Mar 12, 2021
 *      Author: Nik Clark
 */

#include "pwm_interpreter.hpp"

/****************** Constant Definitions ***************************/
static constexpr int PWM_INTERPRETER_ENABLE_REG_OFFSET = 0;
static constexpr int PWM_INTERPRETER_DUTY_REG_OFFSET	= 4;
static constexpr int PWM_INTERPRETER_PERIOD_REG_OFFSET = 8;
static constexpr int PWM_INTERPRETER_ID_REG_OFFSET		= 12;

/****************** Class Definitions ******************************/
PWMInterpreter::PWMInterpreter(uintptr_t p_baseaddress)
{
	baseaddress 	= p_baseaddress;
	isEnabled 		= false;
	isIntrEnabled 	= false;

	IDstr[0] = 'E';
	IDstr[1] = 'R';
	IDstr[2] = 'R';
	IDstr[3] = '\0';
}


void PWMInterpreter::Enable()
{
	u32 mask;
	mask = Xil_In32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask |= 0x1;
	Xil_Out32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);

	isEnabled = true;
}

void PWMInterpreter::Disable()
{
	u32 mask;
	mask = Xil_In32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask &= (0xFFFFFFFF-0x1);
	Xil_Out32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);

	isEnabled = false;
}

void PWMInterpreter::EnableInterrupt()
{
	u32 mask;
	mask = Xil_In32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask |= 0x2;
	Xil_Out32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);

	isIntrEnabled = true;
}

void PWMInterpreter::DisableInterrupt()
{
	u32 mask;
	mask = Xil_In32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask &= (0xFFFFFFFF-0x2);
	Xil_Out32(baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);

	isIntrEnabled = FALSE;
}

uint32_t PWMInterpreter::ReadDutyPeriod()
{
	return Xil_In32(baseaddress + PWM_INTERPRETER_DUTY_REG_OFFSET);
}

uint32_t PWMInterpreter::ReadPeriod()
{
	return Xil_In32(baseaddress + PWM_INTERPRETER_PERIOD_REG_OFFSET);
}

char* PWMInterpreter::ReadID()
{
	u32 raw;
	raw = Xil_In32(baseaddress + PWM_INTERPRETER_ID_REG_OFFSET);

	IDstr[0] = (raw & 0x000000FF) >> 0;
	IDstr[1] = (raw & 0x0000FF00)	>> 8;
	IDstr[2] = (raw & 0x00FF0000) >> 16;
	IDstr[3] = (raw & 0xFF000000) >> 24;

	return IDstr;
}
