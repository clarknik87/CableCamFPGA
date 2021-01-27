/*
 * gpio.hpp
 *
 *  Created on: Jan 12, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_GPIO_HPP_
#define SRC_GPIO_HPP_

#include "xgpio.h"
#include "xintc.h"

#define GPIO_SWITCH_CHANNEL 2
#define GPIO_LED_CHANNEL	1

namespace gpio
{

	void handler(void *callback);
	int init();
	int interrupt_connect(XIntc &IntrController);
	void stop_send();
	void clear_fifos();

}


#endif /* SRC_GPIO_HPP_ */
