/*
 * interrupt_ctrl.hpp
 *
 *  Created on: Mar 7, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_INTERRUPT_CTRL_HPP_
#define SRC_INTERRUPT_CTRL_HPP_

#include "xintc.h"

namespace interrupt
{

int init(XIntc &IntrController);
int start(XIntc &IntrController);

}



#endif /* SRC_INTERRUPT_CTRL_HPP_ */
