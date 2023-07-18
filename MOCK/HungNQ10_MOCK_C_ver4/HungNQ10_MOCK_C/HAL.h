#ifndef _HAL_H_
#define _HAL_H_
/*******************************************************************************
* Define
*******************************************************************************/



typedef enum
{
    HAL_success = 0, /* Process succeed */
    HAL_initFail = 1, /*File opening failed*/
    HAL_updateSectorSizeFail = 2,/*Update Size Sector failed*/
}HAL_Error_t;

/*******************************************************************************
* Prototypes
*******************************************************************************/

/**
 * @brief read files
 *
 * @param[in] FileFath file path
 * @return uint8_t ERROR_HAL_NULL
 */
HAL_Error_t HAL_Init(const char * FilePath);

/**
 * @brief update sector size
 *
 * @param[in_out] bytesPerSect number of bytes of a sector
 */
HAL_Error_t HAL_UpdateSectorSize(uint16_t bytesPerSect);

/**
 * @brief read one sector
 *
 * @param[in] index number of sectors to read
 * @param[in_out] buff
 * @return int32_t number of bytes read
 */
int32_t HAL_ReadSector(uint32_t index, uint8_t *buff);

/**
 * @brief  read multiple sectors
 *
 * @param[in] index number of sectors to read
 * @param[in] num
 * @param[in_out] buff
 * @return int32_t number of bytes read
 */
int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff);

/**
 * @brief fclose file 
 *
 */
void HAL_DeInit(void);

#endif /* _HAL_H_ */

/*******************************************************************************
* EOF
*******************************************************************************/

