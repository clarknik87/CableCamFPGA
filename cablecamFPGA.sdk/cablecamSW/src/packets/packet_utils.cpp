/*
 * version.hpp
 *
 *  Created on: Feb 11, 2021
 *      Author: Nik Clark
 */

#include "packet_utils.hpp"

namespace packet_utils
{

static constexpr uint16_t X25_INIT_CRC = 0xffff;

static void crc_accumulate(uint8_t data, uint16_t *crcAccum)
{
uint8_t tmp;
  tmp = data ^ (uint8_t)(*crcAccum &0xff);
  tmp ^= (tmp<<4);
  *crcAccum = (*crcAccum>>8) ^ (tmp<<8) ^ (tmp <<3) ^ (tmp>>4);
}


uint16_t crc_calculate(const uint8_t* pBuffer, uint16_t length)
{
uint16_t crcTmp = X25_INIT_CRC;
  while (length--) {
    crc_accumulate(*pBuffer++, &crcTmp);
  }
  return crcTmp;
}
}
