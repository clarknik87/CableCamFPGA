/*
 * gpio.cpp
 *
 *  Created on: Jan 12, 2021
 *      Author: Nik Clark
 */

#include "gpio.hpp"
#include "stdint.h"
#include "xgpio.h"
#include "xintc.h"
#include "xparameters.h"
#include "xil_printf.h"

static XGpio gpioDevice;

namespace gpio
{

void handler(void *callback)
{
	uint16_t switch_val;

	XGpio *GpioDevice;
	GpioDevice = (XGpio *)callback;

	XGpio_InterruptClear(GpioDevice, XGPIO_IR_CH2_MASK);

	switch_val = XGpio_DiscreteRead(GpioDevice, GPIO_SWITCH_CHANNEL);
	XGpio_DiscreteWrite(GpioDevice, GPIO_LED_CHANNEL, switch_val);
}

int init()
{
	int status = XST_SUCCESS;

	// Initialize GPIO
	status += XGpio_Initialize(&gpioDevice, XPAR_AXI_GPIO_0_DEVICE_ID);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize GPIO\r\n");

	// Set GPIO directions. 0 = output, 1 = input
	XGpio_SetDataDirection(&gpioDevice, GPIO_LED_CHANNEL, 0);
	XGpio_SetDataDirection(&gpioDevice, GPIO_SWITCH_CHANNEL, 0xFFFF);

	// Enable GPIO interrupts
	XGpio_InterruptEnable(&gpioDevice, XGPIO_IR_CH2_MASK);
	XGpio_InterruptGlobalEnable(&gpioDevice);

	xil_printf("GPIO initialized\r\n");
	return status;
}

int interrupt_connect(XIntc &IntrController)
{
	int status = XIntc_Connect(&IntrController,XPAR_INTC_0_GPIO_0_VEC_ID, (Xil_ExceptionHandler)gpio::handler, &gpioDevice );
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to connect GPIO interrupt\r\n");
	else xil_printf("GPIO interrupt handler connected\r\n");

	XIntc_Enable(&IntrController, XPAR_INTC_0_UARTLITE_0_VEC_ID);

	return status;
}

} // End namespace gpio
