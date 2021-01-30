/*
 * debug_uart.hpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_DEBUG_UART_HPP_
#define SRC_DEBUG_UART_HPP_

#include <cstdint>
#include "xintc.h"

namespace debug_uart
{
	int init();
	int interrupt_connect(XIntc &IntrController);
	void update();

	int send(uint8_t *buffer, int length);
	int receive(uint8_t *buffer, int length);
}

#endif /* SRC_DEBUG_UART_HPP_ */
