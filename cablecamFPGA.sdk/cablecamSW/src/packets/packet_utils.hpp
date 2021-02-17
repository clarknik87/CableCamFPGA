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

namespace acknowledgePkt
{

	constexpr uint8_t ACK_OK 				= 0;
	constexpr uint8_t ACK_ERR_FAIL 			= 1;
	constexpr uint8_t ACK_ERR_ACCESS_DENIED = 2;
	constexpr uint8_t ACK_ERR_NOT_SUPPORTED = 3;
	constexpr uint8_t ACK_ERR_TIMEOUT 		= 150;
	constexpr uint8_t ACK_ERR_CRC			= 151;
	constexpr uint8_t ACK_PAYLOADLEN		= 152;

	union response
		{
	#pragma pack(1)
			struct Packet
			{
				uint8_t 	startsign;
				uint8_t 	length;
				uint8_t 	command;
				uint8_t		data;
				uint16_t	crc;
			}pkt;
			uint8_t raw[sizeof(pkt)];
	#pragma pack()

			bool check_crc()
			{
				uint16_t calc_crc = packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
				if( calc_crc != pkt.crc )
					return false;

				return true;
			}
		};
}


#endif /* SRC_PACKETS_PACKET_UTILS_HPP_ */
