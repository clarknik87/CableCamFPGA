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

	int init()
	{
		xil_printf("raw reg0 value = %u\r\n", channel1.ReadRaw(0));
		channel1.Enable();
		channel1.EnableInterrupt();
		xil_printf("raw reg0 value = %u\r\n", channel1.ReadRaw(0));
		xil_printf("raw reg0 value = %u\r\n", channel1.ReadRaw(3));

		return XST_SUCCESS;
	}

	void test()
	{
		auto print_reg = [&](int reg){xil_printf("channel1 reg%d = %u\r\n", reg, channel1.ReadRaw(reg));};

		xil_printf("channel1 ID = %s\r\n", channel1.ReadID());
		xil_printf("channel1 period = %u\r\n", channel1.ReadPeriod());
		print_reg(0);
		print_reg(1);
		print_reg(2);
		print_reg(3);
	}
}

