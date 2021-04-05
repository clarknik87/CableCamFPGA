/*
 * axis_control.hpp
 *
 *  Created on: Feb 15, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_PACKETS_AXIS_CONTROL_HPP_
#define SRC_PACKETS_AXIS_CONTROL_HPP_

#include "packet_utils.hpp"

namespace setpitch
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	pitchvalue;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x02;
			pkt.command		= 0x0A;
			pkt.pitchvalue	= 0x00;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}
	};


	typedef acknowledgePkt::response response;

}

namespace setroll
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	rollvalue;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x02;
			pkt.command		= 0x0B;
			pkt.rollvalue	= 0x00;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}
	};


	typedef acknowledgePkt::response response;

}

namespace setyaw
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	yawvalue;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x02;
			pkt.command		= 0x0C;
			pkt.yawvalue	= 0x00;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}
	};


	typedef acknowledgePkt::response response;

}

#endif /* SRC_PACKETS_AXIS_CONTROL_HPP_ */
