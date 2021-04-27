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
#include "drivers/pwm_generator.hpp"
#include "drivers/timer_helper.hpp"

namespace user_ctrl
{
	static PWMInterpreter channel1(XPAR_PWM_INTERPRETER_0_S00_AXI_BASEADDR);
	static PWMInterpreter channel2(XPAR_PWM_INTERPRETER_1_S00_AXI_BASEADDR);
	static PWMInterpreter channel3(XPAR_PWM_INTERPRETER_2_S00_AXI_BASEADDR);
	static PWMInterpreter channel4(XPAR_PWM_INTERPRETER_3_S00_AXI_BASEADDR);
	static PWMInterpreter channel5(XPAR_PWM_INTERPRETER_4_S00_AXI_BASEADDR);
	static PWMInterpreter channel6(XPAR_PWM_INTERPRETER_5_S00_AXI_BASEADDR);

	static PWMGenerator	  driveMotor(XPAR_PWMGENERATOR_0_S_AXI_BASEADDR);
	static int32_t 		  waypointA;
	static int32_t		  waypointB;


	void handler(void * callback)
	{
		HandController *userInput = (HandController *)callback;
		update_controller_state(*userInput);
	}

	int init()
	{
		channel1.Enable();
		channel2.Enable();
		channel3.Enable();
		channel4.Enable();
		channel5.Enable();
		channel6.Enable();
		driveMotor.StopDriveMotor();

		xil_printf("PWM_Generator ID = %s", driveMotor.ReadID());

		//channel1.EnableInterrupt();
		//channel2.EnableInterrupt();
		//channel3.EnableInterrupt();
		//channel4.EnableInterrupt();
		//channel5.EnableInterrupt();
		//channel6.EnableInterrupt();

		waypointA = -1000;
		waypointB = 1000;

		return XST_SUCCESS;
	}

	int interrupt_connect(XIntc &IntrController, HandController &userInput)
	{
		//Connect all input channels to the handler.
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_0_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_1_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_2_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_3_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_4_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);
		XIntc_Connect(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_5_INTERRUPT_INTR, (Xil_ExceptionHandler)user_ctrl::handler, &userInput);

		//Enable all channels in the interrupt vector table
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_0_INTERRUPT_INTR);
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_1_INTERRUPT_INTR);
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_2_INTERRUPT_INTR);
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_3_INTERRUPT_INTR);
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_4_INTERRUPT_INTR);
		//XIntc_Enable(&IntrController, XPAR_AXI_INTC_0_PWM_INTERPRETER_5_INTERRUPT_INTR);

		return XST_SUCCESS;
	}

	void update_controller_state(HandController &userInput)
	{
		userInput.setYaw(channel1.ReadDutyPeriod());
		userInput.setPitch(channel2.ReadDutyPeriod());
		userInput.setAutoSpeed(channel3.ReadDutyPeriod());
		userInput.setDriveMotor(channel4.ReadDutyPeriod());
		userInput.setEndpointSwitch(channel5.ReadDutyPeriod());
		userInput.setControlSwitch(channel6.ReadDutyPeriod());
	}

	static bool waitState = false;
	static int autoDirection = 1;
	static uint64_t time1 = 0;
	static uint64_t time2;
	constexpr uint64_t delayTime = 1.5*XPAR_MICROBLAZE_CORE_CLOCK_FREQ_HZ;

	void update_drive_motor(HandController &userInput, int32_t position)
	{
		//set waypoints if in manual mode, and if endpoint switch not centered
		if( userInput.getControlSwitch() == SwitchPosition::up )
		{
			if( userInput.getEndpointSwitch() == SwitchPosition::up && position < 0 )
				waypointA = position;
			else if( userInput.getEndpointSwitch() == SwitchPosition::down && position > 0 )
				waypointB = position;
		}

		//Manual Mode
		if( userInput.getControlSwitch() == SwitchPosition::up )
		{
			if( (userInput.getDriveMotor() >= 150000 && position < waypointB) ||
				(userInput.getDriveMotor() <= 150000 && position > waypointA)	)
			{
				driveMotor.SetSpeed(userInput.getDriveMotor());
			}
			else
			{
				driveMotor.StopDriveMotor();
			}
		}
		//Auto Mode
		else if( userInput.getControlSwitch() == SwitchPosition::down )
		{
			if( !waitState )
			{
				if( (autoDirection > 0 && position < waypointB) || (autoDirection < 0 && position > waypointA) )
				{
					driveMotor.SetSpeed(150000 + (autoDirection * userInput.getAutoSpeed()));
				}
				else
				{
					driveMotor.StopDriveMotor();
					waitState = true;
					time1 = globalTimer.getTicks();
				}
			}
			else
			{
				time2 = globalTimer.getTicks();
				if( time2-time1 >= delayTime )
				{
					autoDirection = -autoDirection;
					waitState = false;
				}
			}
		}
	}
}

