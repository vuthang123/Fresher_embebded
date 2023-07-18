
#ifndef _HAL_FLASH_H_
#define _HAL_FLASH_H_

#include "stdint.h"



/*******************************************************************************
 * API
 ******************************************************************************/
/*!
  * @brief: flash data input into flash
  * @param: address to flash data to flash
  * @param: input data 32 bits need to flash data into flash
  * @return: return 1: if success
*/

uint8_t HALFLASH_ProgramLongWord(uint32_t Addr,uint8_t *Data);
/*!
  * @brief: erase a sector in flash
  * @param: address to erase
  * @return: return 1: if success
*/
uint8_t HALFLASH_EraseSector(uint32_t Addr);

#endif /*_HAL_FLASH_H_*/
/*******************************************************************************
 * EOF
 ******************************************************************************/