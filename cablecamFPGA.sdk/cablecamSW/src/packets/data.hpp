/*
 * data.hpp
 *
 *  Created on: Mar 29, 2021
 *      Author: Nik Clark
 */

#ifndef SRC_PACKETS_DATA_HPP_
#define SRC_PACKETS_DATA_HPP_

#include "packet_utils.hpp"

namespace GetDataPkt
{
	union request
	{
#pragma pack(1)
		struct Packet
		{
			uint8_t 	startsign;
			uint8_t 	length;
			uint8_t 	command;
			uint8_t		type;
			uint16_t	crc;
		}pkt;
		uint8_t raw[sizeof(pkt)];
#pragma pack()

		request()
		{
			pkt.startsign	= 0xFA;
			pkt.length 		= 0x01;
			pkt.command		= 0x05;
			pkt.type		= 0x00;
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

			uint8_t		type;
			uint8_t		reserved0;

			uint16_t	state;
			uint16_t	status;
			uint16_t	status2;
			uint16_t	status3;
			uint16_t	lipo_voltage;
			uint16_t	errorcnt;
			uint16_t	performance;
			uint16_t	timestamp;
			uint16_t	cycle_time;
			uint16_t	Imu1_pitch_angle;
			uint16_t	Imu1_roll_angle;
			uint16_t	Imu1_yaw_angle;
			uint16_t	Imu1_rx;
			uint16_t	Imu1_ry;
			uint16_t	Imu1_rz;
			uint16_t	PID_pitch;
			uint16_t	PID_roll;
			uint16_t	PID_yaw;
			uint16_t	RC_pitch;
			uint16_t	RC_roll;
			uint16_t	RC_yaw;
			uint16_t	Imu2_pitch_angle;
			uint16_t	Imu2_roll_angle;
			uint16_t	Imu2_yaw_angle;
			uint16_t	motor_pitch_angle;
			uint16_t	motor_roll_angle;
			uint16_t	motor_yaw_angle;
			uint16_t	reserved1;
			uint16_t	link_yaw;
			uint16_t	acc_magnitude;
			uint16_t	acc_confidence;
			uint16_t	func_input;

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


#endif /* SRC_PACKETS_DATA_HPP_ */
