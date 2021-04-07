/*
 * pwm_generator.hpp
 *
 *  Created on: Apr 6, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DRIVERS_PWM_GENERATOR_HPP_
#define SRC_DRIVERS_PWM_GENERATOR_HPP_

#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

class PWMGenerator
{
	uintptr_t baseaddress;
	char IDstr[4];

public:
	PWMGenerator(uintptr_t p_baseaddress);
	void	SetSpeed(uint32_t speed);
	void	StopDriveMotor();
	char* 	ReadID();
};



#endif /* SRC_DRIVERS_PWM_GENERATOR_HPP_ */
