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

void gpio_handler(void *callback)
{
	uint16_t switch_val;

	XGpio *GpioDevice;
	GpioDevice = (XGpio *)callback;

	XGpio_InterruptClear(GpioDevice, XGPIO_IR_CH2_MASK);

	switch_val = XGpio_DiscreteRead(GpioDevice, GPIO_SWITCH_CHANNEL);
	XGpio_DiscreteWrite(GpioDevice, GPIO_LED_CHANNEL, switch_val);
}

int gpio_init(XGpio &GpioDevice)
{
	int status = XST_SUCCESS;

	// Initialize GPIO
	status += XGpio_Initialize(&GpioDevice, XPAR_AXI_GPIO_0_DEVICE_ID);
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize GPIO\r\n");

	// Set GPIO directions. 0 = output, 1 = input
	XGpio_SetDataDirection(&GpioDevice, GPIO_LED_CHANNEL, 0);
	XGpio_SetDataDirection(&GpioDevice, GPIO_SWITCH_CHANNEL, 0xFFFF);

	// Enable GPIO interrupts
	XGpio_InterruptEnable(&GpioDevice, XGPIO_IR_CH2_MASK);
	XGpio_InterruptGlobalEnable(&GpioDevice);

	xil_printf("GPIO initialized\r\n");
	return status;
}

int gpio_interrupt_connect(XGpio &GpioDevice, XIntc &IntrController)
{
	int status = XIntc_Connect(&IntrController,XPAR_INTC_0_GPIO_0_VEC_ID, (Xil_ExceptionHandler)gpio_handler, &GpioDevice );
	if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to connect GPIO interrupt\r\n");
	else xil_printf("GPIO interrupt handler connected\r\n");
	return status;
}

