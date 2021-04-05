/*
 * controller_state.hpp
 *
 *  Created on: Apr 4, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_CONTROLLER_STATE_HPP_
#define SRC_CONTROLLER_STATE_HPP_

struct HandController
{
	uint32_t driveMotor;
	uint32_t cameraYaw;
	uint32_t cameraPitch;
	uint32_t endpointSwitch;
	uint32_t controlSwitch;
};


#endif /* SRC_CONTROLLER_STATE_HPP_ */
