/*
 * user_ctrl.cpp
 *
 *  Created on: Mar 7, 2021
 *      Author: Nik Clark
 */

// Xilinx includes
#include "xparameters.h"
#include "xstatus.h"

// Local includes
#include "user_ctrl.hpp"
#include "drivers/pwm_interpreter.hpp"

namespace user_ctrl
{
	static PWMInterpreter channel1(XPAR_PWM_INTERPRETER_0_S00_AXI_BASEADDR);

	void handler(void * callback)
	{
		xil_printf("In user_cntrl handler\r\n");
	}

	int init()
	{
		channel1.Enable();
		channel1.EnableInterrupt();

		xil_printf("Channel1 ID = %s\r\n", channel1.ReadID());
		xil_printf("Channel1 Period = %u\r\n", channel1.ReadPeriod());
		xil_printf("Channel1 Duty = %u\r\n", channel1.ReadDutyPeriod());

		return XST_SUCCESS;
	}

	int interrupt_connect(XIntc &IntrController)
	{
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_0_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, NULL);

		XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_0_VEC_ID);

		return XST_SUCCESS;
	}
}

