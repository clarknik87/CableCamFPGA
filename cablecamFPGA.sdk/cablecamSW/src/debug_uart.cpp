/*
 * debug_uart.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Nik Clark
 */


#include "debug_uart.hpp"
#include "xuartlite.h"
#include "xparameters.h"
#include "xil_printf.h"
#include <cstdint>

//constexpr int buffer_size{16};
//static uint8_t sendBuffer[buffer_size];
//static uint8_t recvBuffer[buffer_size];

static XUartLite uartDevice;

static void SendHandler(void *CallBackRef, unsigned int EventData)
{
	return;
}

static void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	xil_printf("In receive handler!\r\n");
}


namespace debug_uart
{
	int init()
	{
		int status = XST_SUCCESS;

		status += XUartLite_Initialize(&uartDevice, XPAR_AXI_UARTLITE_USB_DEVICE_ID);
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize debug uart\r\n");

		status += XUartLite_SelfTest(&uartDevice);
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Debug uart failed self test\r\n");

		return XST_SUCCESS;
	}

	int interrupt_connect(XIntc &IntrController)
	{
		int status = XST_SUCCESS;
		const int interrupt_id = XPAR_AXI_INTC_0_AXI_UARTLITE_USB_INTERRUPT_INTR;

		status += XIntc_Connect(&IntrController, interrupt_id, (XInterruptHandler)XUartLite_InterruptHandler, (void *)&uartDevice );
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to connect interrupt handler for debug uart\r\n");

		XIntc_Enable(&IntrController, interrupt_id);

		XUartLite_SetSendHandler(&uartDevice, SendHandler, (void *)&uartDevice);
		XUartLite_SetRecvHandler(&uartDevice, RecvHandler, (void *)&uartDevice);

		XUartLite_EnableInterrupt(&uartDevice);

		return status;
	}

	int send(uint8_t *buffer, int length)
	{
		int count = 0;
		count = XUartLite_Send(&uartDevice, buffer, length);
		if( count != length )
			return XST_FAILURE;

		while(XUartLite_IsSending(&uartDevice)){}
		return XST_SUCCESS;
	}

	int receive(uint8_t *buffer, int length)
	{
		int count = 0;
		count = XUartLite_Recv(&uartDevice, buffer, length);
		if( count != length)
			return XST_FAILURE;

		return XST_SUCCESS;

	}
}
