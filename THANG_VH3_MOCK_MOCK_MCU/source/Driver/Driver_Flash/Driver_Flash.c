#include "Driver_Flash.h"
#include "HAL_FLASH.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ONE_KB 1024U

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
    @brief:Read data from Flash memory address.
    @param:Address: The address of the Flash memory to read.
    @return:The data value read from the Flash address.
*/
uint32_t DRIFLASH_ReadFlashAddress(uint32_t Address)
{
    return *((volatile uint32_t*)Address);
}

/**
    @brief:Delete multiple sectors in Flash memory.
    @param:Addr Address of the first sector to delete.
    @param:Size Number of sectors to delete.
    @return:Sector delete result. Returns 0 if successful, 
    otherwise returns a non-zero value.
*/
uint8_t  DRIFLASH_EraseMultiSector(uint32_t Addr,uint8_t Size)
{
    uint8_t index;

    for(index = 0; index<Size; index++)
    {
        HALFLASH_EraseSector(Addr + index * ONE_KB);
    }

    return true;
}
/**
    @brief: Erase all contents of Flash memory.
    @param: Addr: Address of the first sector to delete.
    @param: Size: Number of sectors to delete.
    @return: None.
*/
void DRIFLASH_AllEraseFlash(uint32_t Addr,uint8_t Size)
{
    __disable_irq();
    
    DRIFLASH_EraseMultiSector(Addr, Size);
    
    __enable_irq();
}
/**
    @brief: Write data to Flash memory.
    @param: Addr: The address where the data is written.
    @param: Data: Pointer to the data to write.
    @return: None.
*/
void DRIFLASH_Write_Flash(uint32_t Addr,uint8_t *Data)
{
    __disable_irq();
    
   HALFLASH_ProgramLongWord(Addr, Data);
   
    __enable_irq();
}
/*******************************************************************************
 * EOF
 ******************************************************************************/