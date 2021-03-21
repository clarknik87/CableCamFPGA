/*
 * pwm_interpreter.hpp
 *
 *  Created on: Mar 12, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DRIVERS_PWM_INTERPRETER_HPP_
#define SRC_DRIVERS_PWM_INTERPRETER_HPP_

/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

/**************************** Class Definition ************************************/

class PWMInterpreter
{
	uintptr_t baseaddress;
	char IDstr[4];
	bool isEnabled;
	bool isIntrEnabled;

public:
	PWMInterpreter(uintptr_t p_baseaddress);

	void 	 Enable();
	void 	 Disable();
	void 	 EnableInterrupt();
	void 	 DisableInterrupt();
	uint32_t ReadDutyPeriod();
	uint32_t ReadPeriod();
	char* 	 ReadID();
	uint32_t ReadRaw(int reg);
	void 	 WriteRaw(int reg, uint32_t reg_val);
};

#endif /* SRC_DRIVERS_PWM_INTERPRETER_HPP_ */
