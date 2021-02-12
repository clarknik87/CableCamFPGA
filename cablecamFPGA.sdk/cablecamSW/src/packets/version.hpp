/*
 * version.hpp
 *
 *  Created on: Feb 11, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_PACKETS_VERSION_HPP_
#define SRC_PACKETS_VERSION_HPP_

#include "packet_utils.hpp"

namespace VersionPkt
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x00;
			pkt.command		= 0x01;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}
	};


	union response
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	fwversion;
			uint16_t	layoutversion;
			uint16_t	boardcapabilities;
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


#endif /* SRC_PACKETS_VERSION_HPP_ */
