/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


// Xilinx/Standard Library Includes
#include <stdio.h>
#include "xil_printf.h"
#include "microblaze_sleep.h"
#include "xgpio.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xparameters.h"
//#include "scanvalue.h

// Project Specific Includes
#include "gpio.hpp"
#include "platform.h"

int interrupt_init(XIntc &IntrController)
{
	int status = XST_SUCCESS;

	// Initialize Interrupt
	status += XIntc_Initialize(&IntrController, XPAR_AXI_INTC_0_DEVICE_ID);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize interrupt controller\r\n");

	status += XIntc_SelfTest(&IntrController);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed interrupt controller self-test\r\n");

	// Enable Interrupt Controller
	XIntc_Enable(&IntrController, XPAR_INTC_0_GPIO_0_VEC_ID);
	XIntc_Enable(&IntrController, XPAR_INTC_0_UARTLITE_0_VEC_ID);


	//Enable Exception Handler
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &IntrController);
	Xil_ExceptionEnable();

	xil_printf("Initialized interrupt controller\r\n");
	return status;
}

int interrupt_start(XIntc &IntrController)
{
	//Allow interrupts
	XIntc_Start(&IntrController, XIN_REAL_MODE);
	xil_printf("Started interrupt controller");
	return XST_SUCCESS;
}


int main()
{
	XGpio mainGpioDevice;
	XIntc mainIntrController;

	// Initialize platform
    init_platform();
    xil_printf("System reset.\r\n");

    gpio_init(mainGpioDevice);
    interrupt_init(mainIntrController);
    gpio_interrupt_connect(mainGpioDevice, mainIntrController);
    interrupt_start(mainIntrController);


    while(1)
    {
    	MB_Sleep(1000);
    }

    cleanup_platform();
    return 0;
}
