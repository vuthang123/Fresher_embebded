#ifndef _FATFS_H_
#define _FATFS_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

typedef enum
{
    FATF_SUCCESS = 0x00,
    FATF_INIT_FAILED = 0x01,
    FATF_UPDATE_FAILED = 0x02,
    FATF_DEINIT_FAILED = 0x03
} FATF_error_code_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/* ----------------Static Variable------------------- */

static FILE *s_fp = NULL;           /* FAT file */
static uint16_t s_sectorSize = 512; /* size of sector */

/*
 * @brief: open the image file
 * @param[in]: file path
 * @return:  true - open successfully
 *           false - open fail
 */
FATF_error_code_t FATF_Init(const uint8_t * const file_Path);

/*
 * @brief: read a sector and store data in buffer
 * @param[in]: pos_Offset - possision of sector
 * @param[out]: buff - buffer array to store data
 * @return: number of bytes read.
 */
uint32_t FATF_ReadSector(uint32_t pos_Offset, uint8_t * buffer);

/*
 * @brief: read multi sector and store data in buffer
 * @param[in1]: num - number of sectors that need to read.
 * @param[in2]: pos_Offset- pos_Offset of starting sector.
 * @param[out]: buff - buffer array to store data
 * @return: number of bytes read.
 */
uint32_t FATF_ReadMultiSector(uint32_t pos_Offset, uint32_t number, uint8_t * buffer);

/*
 * @brief update size sector after read boot
 * 
 * @param new_sector_size 
 * @return FATF_ERROR_CODE_t 
 */
FATF_error_code_t FATF_UpdateSectorSize(const uint16_t new_sector_size);

/*
 * @brief close file
 * 
 * @return FATF_ERROR_CODE_t 
 */
FATF_error_code_t FATF_Deinit(void);

#endif /* _FAT_H_ */

