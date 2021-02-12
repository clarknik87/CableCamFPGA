/*
 * packet_utils.hpp
 *
 *  Created on: Feb 11, 2021
 *      Author: ecestudent
 */

#ifndef SRC_PACKETS_PACKET_UTILS_HPP_
#define SRC_PACKETS_PACKET_UTILS_HPP_

#include <cstdint>

namespace packet_utils
{

uint16_t crc_calculate(const uint8_t* pBuffer, uint16_t length);

}


#endif /* SRC_PACKETS_PACKET_UTILS_HPP_ */
