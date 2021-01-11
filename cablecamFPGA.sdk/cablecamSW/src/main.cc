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

#include <stdio.h>
//#include "platform.h"
#include "xil_printf.h"
#include "microblaze_sleep.h"
#include "xgpio.h"
#include "xparameters.h"
//#include "scanvalue.h"


int main()
{
	XGpio gpio_iface;
	u16 sw = 0;

	// Initialize platform
    //init_platform();
    xil_printf("System reset.\r\n");

    //Initialize GPIO
    if( XST_SUCCESS != XGpio_Initialize(&gpio_iface, XPAR_AXI_GPIO_0_DEVICE_ID) )
    	xil_printf("Error in initializing GPIO.\r\n");
    else
    	xil_printf("GPIO initialized.\r\n");

    // Set GPIO directions. 0 = output, 1 = input
    XGpio_SetDataDirection(&gpio_iface, 1, 0);
    XGpio_SetDataDirection(&gpio_iface, 2, 1);

    //Turn the LEDs on by default
    XGpio_DiscreteWrite(&gpio_iface, 2, 0x0000FFFF);

    // Repeatedly read from the switches and print.
    // Repeatedly read from the UART and update the LEDs
    while(1)
    {
    	// Get the value of the switches
    	sw = XGpio_DiscreteRead(&gpio_iface, 2);

    	// Get the value from the UART
    	//uval = scanValue();

    	// Get the value of diff and print to the terminal
    	//iff = sw - uval;
    	//xil_printf("Diff = %x\n", diff);

    	// Show LSB bits of diff on LEDs, wait, show MSB bits on LEDs
    	XGpio_DiscreteWrite(&gpio_iface, 1, sw);
    	MB_Sleep(100);
    }

    //cleanup_platform();
    return 0;
}
