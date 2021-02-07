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
	void update();

	int send(uint8_t *buffer, int length);
	int receive(uint8_t *buffer, int length);

}

#endif /* SRC_STORM_UART_HPP_ */
