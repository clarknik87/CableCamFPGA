/*
 * storm_uart.cpp
 *
 *  Created on: Jan 30, 2021
 *      Author: Nik Clark
 */


#include "storm_uart.hpp"
#include "debug_uart.hpp"
#include "xuartlite.h"
#include "xparameters.h"
#include "xil_printf.h"
#include <cstdint>
#include <cstdlib>

#include "packets/version.hpp"

constexpr int buffer_size{64};
static uint8_t sendBuffer[buffer_size];
static uint8_t recvBuffer[buffer_size];
static volatile int recv_length;
static volatile bool send_complete = true;
static volatile bool update_required = false;

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
		const int start_byte = 0xFB;
		static int payload_length = 0;

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

namespace storm_uart
{
	int init()
	{
		int status = XST_SUCCESS;

		status += XUartLite_Initialize(&uartDevice, XPAR_AXI_UARTLITE_STORM32_DEVICE_ID);
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to initialize storm uart\r\n");

		status += XUartLite_SelfTest(&uartDevice);
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Storm uart failed self test\r\n");

		return XST_SUCCESS;
	}

	int interrupt_connect(XIntc &IntrController)
	{
		int status = XST_SUCCESS;
		const int interrupt_id = XPAR_INTC_0_UARTLITE_0_VEC_ID;

		status += XIntc_Connect(&IntrController, interrupt_id, (XInterruptHandler)XUartLite_InterruptHandler, (void *)&uartDevice );
		if(status != XST_SUCCESS) xil_printf("<ERROR> = Failed to connect interrupt handler for storm uart\r\n");

		XIntc_Enable(&IntrController, interrupt_id);

		XUartLite_SetSendHandler(&uartDevice, SendHandler, (void *)&uartDevice);
		XUartLite_SetRecvHandler(&uartDevice, RecvHandler, (void *)&uartDevice);

		XUartLite_EnableInterrupt(&uartDevice);

		return status;
	}

	int init_storm_parameters()
	{
		VersionPkt::request  vrqpkt;
		VersionPkt::response vrspkt;
		if(sendreceive(vrqpkt.raw, sizeof(vrqpkt.pkt), vrspkt.raw, sizeof(vrspkt.pkt)) == XST_SUCCESS)
		{
			if(vrspkt.check_crc())
			{
				xil_printf("<INFO> = Firmware Version:   %d\r\n", vrspkt.pkt.fwversion);
				xil_printf("<INFO> = Layout Version:     %d\r\n", vrspkt.pkt.layoutversion);
				xil_printf("<INFO> = Board Capabilities: %d\r\n", vrspkt.pkt.boardcapabilities);
			}
			else
				xil_printf("<ERROR> = Invalid version packet received\r\n");
		}
		else
			xil_printf("<ERROR> = Unable to retrieve version data\r\n");

		return XST_SUCCESS;
	}

	void update()
	{
		if(update_required)
		{
			update_required = false;

			// Send received packet through debug_uart
			debug_uart::send(recvBuffer, recv_length);
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

	int sendreceive(uint8_t *p_sendbuf, int p_sendlength, uint8_t *p_recvbuf, int p_recvlength)
	{
		if (p_sendbuf == nullptr || p_recvbuf == nullptr)
			return XST_NO_DATA;

		if (send(p_sendbuf, p_sendlength) == XST_DEVICE_BUSY)
			return XST_DEVICE_BUSY;

		while(!update_required)
		{
		}

		if(p_recvlength != recv_length)
			return XST_BUFFER_TOO_SMALL;

		memcpy(p_recvbuf, recvBuffer, recv_length);

		return XST_SUCCESS;
	}
}

