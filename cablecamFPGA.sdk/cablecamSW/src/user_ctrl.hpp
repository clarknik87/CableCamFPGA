/*
 * user_ctrl.hpp
 *
 *  Created on: Mar 7, 2021
 *      Author: ecestudent
 */

#ifndef SRC_USER_CTRL_HPP_
#define SRC_USER_CTRL_HPP_

#include "xintc.h"

namespace user_ctrl
{
	int init();
	int interrupt_connect(XIntc &IntrController);
}


#endif /* SRC_USER_CTRL_HPP_ */
