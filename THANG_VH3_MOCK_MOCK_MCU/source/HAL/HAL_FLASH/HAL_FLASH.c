#include "MKE16Z4.h"
#include "HAL_FLASH.h"
#include "stdbool.h"
/*******************************************************************************
* Defines
******************************************************************************/

#define HALFALSH_WAIT_CCIF            (0x00U)
#define HALFALSH_CMD_CCIF             (0x80U)
#define HALFLASH_CMD_ACCERR_FPVIOL    (0x30U)
#define HALFALSH_PROGRAM_LONGWORD     (0x06)
#define HALFALSH_ERASE_FLASH_SECTOR   (0x09)
#define HALFALSH_SHIFT_16             (16U)
#define HALFALSH_SHIFT_8              (8U)
#define HALFALSH_SHIFT_0              (0U)
/*******************************************************************************
*               Codes
******************************************************************************/
/*!
  * @brief: flash data input into flash
  * @param: address to flash data to flash
  * @param: input data 32 bits need to flash data into flash
  * @return: return 1: if success
*/
uint8_t HALFLASH_ProgramLongWord(uint32_t Addr,uint8_t *Data)
{
    /* wait previous cmd finish */
    while (FTFA->FSTAT == HALFALSH_WAIT_CCIF);

    /* clear previous cmd error */
    if(FTFA->FSTAT != HALFALSH_CMD_CCIF)
    {
        FTFA->FSTAT = HALFLASH_CMD_ACCERR_FPVIOL;
    }
    
    /* Program 4 bytes in a program flash block */
    FTFA->FCCOB0 = HALFALSH_PROGRAM_LONGWORD;

    /* Address */
    FTFA->FCCOB1 = (uint8_t)(Addr >> HALFALSH_SHIFT_16);
    FTFA->FCCOB2 = (uint8_t)(Addr >> HALFALSH_SHIFT_8);
    FTFA->FCCOB3 = (uint8_t)(Addr >> HALFALSH_SHIFT_0);

    /* Data */
    FTFA->FCCOB4 = (uint8_t)(Data[3]);
    FTFA->FCCOB5 = (uint8_t)(Data[2]);
    FTFA->FCCOB6 = (uint8_t)(Data[1]);
    FTFA->FCCOB7 = (uint8_t)(Data[0]);

    /* Clear CCIF */
    FTFA->FSTAT = HALFALSH_CMD_CCIF;
    /* wait cmd finish */
    while (FTFA->FSTAT == HALFALSH_WAIT_CCIF);

    return true;
}

/*!
  * @brief: erase a sector in flash
  * @param: address to erase
  * @return: return 1: if success
*/
uint8_t  HALFLASH_EraseSector(uint32_t Addr)
{
    /* wait previous cmd finish */
    while (FTFA->FSTAT == HALFALSH_WAIT_CCIF);

    /* clear previous cmd error */
    if(FTFA->FSTAT != HALFALSH_CMD_CCIF)
    {
        FTFA->FSTAT = HALFLASH_CMD_ACCERR_FPVIOL;
    }
    /* Erase all bytes in a program flash sector */
    FTFA->FCCOB0 = HALFALSH_ERASE_FLASH_SECTOR;

    /* fill Address */
    FTFA->FCCOB1 = (uint8_t)(Addr >> HALFALSH_SHIFT_16);
    FTFA->FCCOB2 = (uint8_t)(Addr >> HALFALSH_SHIFT_8);
    FTFA->FCCOB3 = (uint8_t)(Addr >> HALFALSH_SHIFT_0);

    /* Clear CCIF */
    FTFA->FSTAT = HALFALSH_CMD_CCIF;
    /* wait cmd finish */
    while (FTFA->FSTAT == HALFALSH_WAIT_CCIF);

    return true;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/


