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
 * main.cpp
 *
 * This is the entry point for the CableCam software system.
 * This file acts essentially as a basic task scheduler by
 * calling each task's initialization functions, followed by
 * each task's loop (update) functions.
 */


// Xilinx/Standard Library Includes
#include <stdio.h>
#include "xil_printf.h"
#include "microblaze_sleep.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "xparameters.h"

// Project Specific Includes
#include "interrupt_ctrl.hpp"
#include "controller_state.hpp"
#include "gpio.hpp"
#include "debug_uart.hpp"
#include "storm_uart.hpp"
#include "user_ctrl.hpp"
#include "platform.h"

#include "drivers/seven_segment.hpp"

static int taskInit(XIntc &mainIntrController)
{
    gpio::init();
    interrupt::init(mainIntrController);
    debug_uart::init();
    storm_uart::init();
    user_ctrl::init();

    return XST_SUCCESS;
}

static int taskConnect(XIntc &mainIntrController, HandController &userInput)
{
	gpio::interrupt_connect(mainIntrController);
	debug_uart::interrupt_connect(mainIntrController);
	storm_uart::interrupt_connect(mainIntrController);
	user_ctrl::interrupt_connect(mainIntrController, userInput);

	return XST_SUCCESS;
}

int main()
{
	// Initialize platform
    init_platform();
    xil_printf("<status> = System reset.\r\n");

    // Instantiate Objects
    XIntc mainIntrController;
    HandController userInput;
    SevenSegment segmentDisplays(XPAR_AXI_SEVENSEGMENTDRIV_0_S_AXI_BASEADDR);

    // Call task init() functions
    taskInit(mainIntrController);
    xil_printf("<status> = Peripherals initialized\r\n");

    // Connect task interrupts
    taskConnect(mainIntrController, userInput);
    xil_printf("<status> = Interrupts connected\r\n");

    // Start interrupt controller
    interrupt::start(mainIntrController);
    xil_printf("<status> = Started interrupt controller\r\n");

    // Initialize Gimbal Control Module using storm_uart
    //storm_uart::start_gimbal_control();

    while(true)
    {
    	user_ctrl::update_controller_state(userInput);
    	//read position sensor
    	//display sensor data on 7 segment displays
    	debug_uart::update();
    	user_ctrl::update_drive_motor(userInput);
    	storm_uart::update(userInput);
    }

    cleanup_platform();
    return 0;
}
