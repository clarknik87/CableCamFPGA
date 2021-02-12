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

namespace storm_uart
{

	int init();
	int interrupt_connect(XIntc &IntrController);
	int init_storm_parameters();
	void update();

	int send(uint8_t *buffer, int length);
	int sendreceive(uint8_t *p_sendbuf, int p_sendlength, uint8_t *p_recvbuf, int p_recvlength);

}

#endif /* SRC_STORM_UART_HPP_ */
