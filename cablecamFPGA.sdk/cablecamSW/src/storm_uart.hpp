/*
 * storm_uart.hpp
 *
 *  Created on: Jan 30, 2021
 *      Author: ecestudent
 */

#ifndef SRC_STORM_UART_HPP_
#define SRC_STORM_UART_HPP_

#include <cstdint>
#include "xintc.h"
#include "controller_state.hpp"

namespace storm_uart
{

	int init();
	int interrupt_connect(XIntc &IntrController);
	int start_gimbal_control();
	void update(HandController &userInput);

	int send(uint8_t *buffer, int length);
	int sendwait(uint8_t *buffer, int length);
	int sendreceive(uint8_t *p_sendbuf, int p_sendlength, uint8_t *p_recvbuf, int p_recvlength);

}

#endif /* SRC_STORM_UART_HPP_ */
