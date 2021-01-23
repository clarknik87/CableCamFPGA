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

void gpio_handler(void *callback);
int gpio_init(XGpio &GpioDevice);
int gpio_interrupt_connect(XGpio &GpioDevice, XIntc &IntrController);



#endif /* SRC_GPIO_HPP_ */
