#ifndef __DRIVER_FLASH_H__
#define __DRIVER_FLASH_H__

#include "MKE16Z4.h"
#include "stdbool.h"
#include "stdint.h"
#include "HAL_FLASH.h"

/*******************************************************************************
 * API
 ******************************************************************************/
/**
    @brief:Read data from Flash memory address.
    @param:Address: The address of the Flash memory to read.
    @return:The data value read from the Flash address.
*/
uint32_t DRIFLASH_ReadFlashAddress(uint32_t Addr);
/**
    @brief:Delete multiple sectors in Flash memory.
    @param:Addr Address of the first sector to delete.
    @param:Size Number of sectors to delete.
    @return:Sector delete result. Returns 0 if successful, 
    otherwise returns a non-zero value.
*/
uint8_t DRIFLASH_EraseMultiSector(uint32_t Addr,uint8_t Size);

/**
    @brief: Erase all contents of Flash memory.
    @param: Addr: Address of the first sector to delete.
    @param: Size: Number of sectors to delete.
    @return: None.
*/
void DRIFLASH_AllEraseFlash(uint32_t Addr, uint8_t Size);

/**
    @brief: Write data to Flash memory.
    @param: Addr: The address where the data is written.
    @param: Data: Pointer to the data to write.
    @return: None.
*/
void DRIFLASH_Write_Flash(uint32_t Addr,uint8_t *Data);

#endif /*__DRIVER_FLASH_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/