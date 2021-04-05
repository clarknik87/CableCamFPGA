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
	static PWMInterpreter channel2(XPAR_PWM_INTERPRETER_1_S00_AXI_BASEADDR);
	static PWMInterpreter channel4(XPAR_PWM_INTERPRETER_3_S00_AXI_BASEADDR);
	static PWMInterpreter channel5(XPAR_PWM_INTERPRETER_4_S00_AXI_BASEADDR);
	static PWMInterpreter channel6(XPAR_PWM_INTERPRETER_5_S00_AXI_BASEADDR);

	void handler(void * callback)
	{
		HandController *userInput = (HandController *)callback;
		update_controller_state(*userInput);
	}

	int init()
	{
		channel1.Enable();
		channel2.Enable();
		channel4.Enable();
		channel5.Enable();
		channel6.Enable();

		//channel1.EnableInterrupt();
		//channel2.EnableInterrupt();
		//channel4.EnableInterrupt();
		//channel5.EnableInterrupt();
		//channel6.EnableInterrupt();

		return XST_SUCCESS;
	}

	int interrupt_connect(XIntc &IntrController, HandController &userInput)
	{
		//Connect all input channels to the handler.
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_0_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_1_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_3_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_4_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_5_VEC_ID, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);

		//Enable all channels in the interrupt vector table
		//XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_0_VEC_ID);
		//XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_1_VEC_ID);
		//XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_3_VEC_ID);
		//XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_4_VEC_ID);
		//XIntc_Enable(&IntrController, XPAR_INTC_0_PWM_INTERPRETER_5_VEC_ID);

		return XST_SUCCESS;
	}

	void update_controller_state(HandController &userInput)
	{
		userInput.cameraYaw		 = channel1.ReadDutyPeriod();
		userInput.cameraPitch	 = channel2.ReadDutyPeriod();
		userInput.driveMotor 	 = channel4.ReadDutyPeriod();
		userInput.endpointSwitch = channel5.ReadDutyPeriod();
		userInput.controlSwitch  = channel6.ReadDutyPeriod();
	}
}

