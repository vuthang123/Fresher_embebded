/*******************************************************************************
* Include
*******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "HAL.h"
#include "FATFS.h"
/*******************************************************************************
* Define
*******************************************************************************/
#define HAL_DEFAUL_SECTOR_SIZE 512U 
uint16_t sizeSector = HAL_DEFAUL_SECTOR_SIZE;

static FILE *sp_file = NULL;

/*******************************************************************************
* Prototype
*******************************************************************************/

HAL_Error_t HAL_Init(const char * FilePath) /* Open file */
{
    HAL_Error_t status = HAL_success;

    sp_file = fopen(FilePath, "rb"); /*open file to read*/
    if (NULL == sp_file)
    {
        status = HAL_initFail;
    }

    return status;
}

HAL_Error_t HAL_UpdateSectorSize(uint16_t bytesPerSect)  /* Update sector */
{
    HAL_Error_t status = HAL_success;

    if(0 == (bytesPerSect % 512))
    {
        sizeSector = bytesPerSect;
        status = HAL_success;
    }
    else
    {
        status = HAL_updateSectorSizeFail;
    }

    return status;
}

int32_t HAL_ReadSector(uint32_t index, uint8_t *buff) /* Read 1 sector */
{
    int32_t bytesRead = 0;/*Read byte number variable variable*/

    if(NULL != sp_file)
    {
        if(0 == fseek(sp_file,index*sizeSector,SEEK_SET));
        {
            bytesRead = fread(buff,1,sizeSector,sp_file);
        }
    }

    return bytesRead;/*Total number of bytes read successfully*/
}

int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff) /* read multiple sectors */
{
    int32_t bytesRead = 0;

    if(NULL != sp_file)
    {
        if(0 == fseek(sp_file, index*sizeSector,SEEK_SET));
        {
            bytesRead = fread(buff,1,sizeSector*num,sp_file);
        }
    }

    return bytesRead;
}

void HAL_DeInit(void) /* close file */
{
    fclose(sp_file);/*Close file*/
}
/*******************************************************************************
* End of file
*******************************************************************************/
