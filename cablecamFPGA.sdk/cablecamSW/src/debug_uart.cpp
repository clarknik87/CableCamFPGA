/*
 * debug_uart.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Nik Clark
 */


#include "debug_uart.hpp"
#include "storm_uart.hpp"
#include "xuartlite.h"
#include "xparameters.h"
#include "xil_printf.h"
#include <cstdint>
#include <cstdlib>

constexpr int buffer_size{64};
static uint8_t sendBuffer[buffer_size];
static uint8_t recvBuffer[buffer_size];
static int recv_length;
static bool send_complete = true;
static bool update_required = false;

static XUartLite uartDevice;

static void SendHandler(void *CallBackRef, unsigned int EventData)
{
	send_complete = true;
	return;
}

static void RecvHandler(void *CallBackRef, unsigned int EventData)
{
	// This structure will add characters to the receive buffer one by one.
	// This allows for a packet structure to signal when a transmission has ended.
	static int recv_offset = 0;
	int cnt = XUartLite_Recv(&uartDevice, recvBuffer + recv_offset, 1);
	if( cnt != 0)
	{
		const int start_byte = 0xFA;
		static int payload_length;

		if(recv_offset == 0)
		{
			if(recvBuffer[recv_offset] == start_byte)
				++recv_offset;
			else
				recv_offset = 0;
		}
		else if(recv_offset == 1)
		{
			payload_length = recvBuffer[recv_offset];
			++recv_offset;
		}
		else if(recv_offset == payload_length+4)
		{
			recv_length = recv_offset+1;
			recv_offset = 0;
			payload_length = 0;
			update_required = true;
		}
		else
		{
			++recv_offset;
		}

		if(recv_offset >= buffer_size)
			recv_offset = 0;

	}
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

	void update()
	{
		if(update_required)
		{
			update_required = false;

			//Send received packet to storm module
			storm_uart::send(recvBuffer, recv_length);
		}
	}

	/*
	 *	notes: 	when sending strings do not append the null character. If null character is in the string simply pass
	 *			the length-1. Sending the null character messes up the SDK terminal for some reason.
	 *
	 *			this function only prints the first 64 characters. This is to allow the caller to be able to modify
	 *			the input buffer immediately after function return.
	 */
	int send(uint8_t *buffer, int length)
	{
		if(XUartLite_IsSending(&uartDevice) || !send_complete)
			return XST_DEVICE_BUSY;

		if(length > buffer_size)
			length = buffer_size;

		memcpy(sendBuffer, buffer, length);

		send_complete = false;
		XUartLite_Send(&uartDevice, buffer, length);

		return XST_SUCCESS;
	}

	void stop_send()
	{
		XUartLite_Send(&uartDevice, nullptr, 0);
	}

	void clear_fifos()
	{
		XUartLite_ResetFifos(&uartDevice);
	}
}
