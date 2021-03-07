/*
 * interrupt_ctrl.cpp
 *
 *  Created on: Mar 7, 2021
 *      Author: Nik Clark
 */

#include "interrupt_ctrl.hpp"

namespace interrupt
{

int init(XIntc &IntrController)
{
	int status = XST_SUCCESS;

	// Initialize Interrupt
	status += XIntc_Initialize(&IntrController, XPAR_AXI_INTC_0_DEVICE_ID);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize interrupt controller\r\n");

	status += XIntc_SelfTest(&IntrController);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed interrupt controller self-test\r\n");

	//Enable Exception Handler
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &IntrController);
	Xil_ExceptionEnable();
	return status;
}

int start(XIntc &IntrController)
{
	//Allow interrupts
	XIntc_Start(&IntrController, XIN_REAL_MODE);
	return XST_SUCCESS;
}

}
