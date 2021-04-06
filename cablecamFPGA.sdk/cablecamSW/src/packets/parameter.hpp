/*
 * parameter.hpp
 *
 *  Created on: Feb 12, 2021
 *      Author: ecestudent
 */

#ifndef SRC_PACKETS_PARAMETER_HPP_
#define SRC_PACKETS_PARAMETER_HPP_

#include "packet_utils.hpp"

/*************************************************************************
 *
 * This packet can be used to query and set all of the various parameters
 * on the storm32 module. The full list can be found in the google drive.
 * The following parameters seem to be the most relevant:
 *
 * 00 -> Pitch P: 540 (0x021C)
 * 01 -> Pitch I: 4000 (0x0FA0)
 * 02 -> Pitch D: 350 (0x015E)
 * 06 -> Roll P: 800 (0x0320)
 * 07 -> Roll I: 4800 (0x12C0)
 * 08 -> Roll D: 2000 (0x07D0)
 * 12 -> Yaw P: 460 (0x01CC)
 * 13 -> Yaw I: 1400 (0x0578)
 * 14 -> Yaw D: 1000 (0x03E8)
 *
 *
 ************************************************************************/
namespace ParameterConst
{
	constexpr int pitchP = 0;
	constexpr int pitchI = 1;
	constexpr int pitchD = 2;

	constexpr int rollP = 0;
	constexpr int rollI = 6;
	constexpr int rollD = 7;

	constexpr int yawP = 12;
	constexpr int yawI = 13;
	constexpr int yawD = 14;
}

namespace GetParameterPkt
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	data;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x02;
			pkt.command		= 0x03;
			pkt.data		= 0x00;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}

		void updateCRC()
		{
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
				uint16_t	paramnum;
				uint16_t	paramval;
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

namespace SetParameterPkt
{

	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint16_t	paramnum;
			uint16_t 	paramval;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x04;
			pkt.command		= 0x04;
			pkt.paramnum	= 0x00;
			pkt.paramval	= 0x00;
			pkt.crc 		= packet_utils::crc_calculate(&raw[1], sizeof(pkt)-3);
		}
	};

	//This packet uses standard acknowledge packets for all responses
	typedef acknowledgePkt::response response;

}


#endif /* SRC_PACKETS_PARAMETER_HPP_ */
