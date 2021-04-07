/*
 * user_ctrl.hpp
 *
 *  Created on: Mar 7, 2021
 *      Author: ecestudent
 */

#ifndef SRC_USER_CTRL_HPP_
#define SRC_USER_CTRL_HPP_

#include "xintc.h"
#include "controller_state.hpp"

namespace user_ctrl
{
	int init();
	int interrupt_connect(XIntc &IntrController, HandController &userInput);
	void update_controller_state(HandController &userInput);
	void update_drive_motor(HandController &userInput);
}


#endif /* SRC_USER_CTRL_HPP_ */
