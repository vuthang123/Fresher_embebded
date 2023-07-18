/*********************************************************************
* Include
*********************************************************************/
#include "FATFS.h"

/*********************************************************************
* Code
*********************************************************************/

/* open the FAT file */
FATF_error_code_t FATF_Init(const uint8_t * const file_Path)
{
    FATF_error_code_t status = FATF_SUCCESS;

    s_fp = fopen(file_Path, "rb");
    if(NULL == s_fp)
    {
        status = FATF_INIT_FAILED;
    }

    return status;
}

/*********************************************************************/
/* read 1 sector */
uint32_t FATF_ReadSector(uint32_t pos_Offset, uint8_t * buffer)
{
	uint32_t byte_read = 0;
	/* fseek return 0 if success */
	if(NULL != s_fp)
	{
	    if(0 == fseek(s_fp, pos_Offset * s_sectorSize, SEEK_SET))
	    {
	        byte_read = fread(buffer, 1, s_sectorSize, s_fp);
	    }
	}

    return byte_read;
}

/*********************************************************************/
/* read multi sector */
uint32_t FATF_ReadMultiSector(uint32_t pos_Offset, uint32_t number, uint8_t * buffer)
{
    int i = 0;
    uint32_t byte_read = 0;
    /* fseek return 0 if success */
    if(NULL != s_fp)
	{
		if(0==fseek (s_fp,(s_sectorSize * pos_Offset) , SEEK_SET))
		{
			byte_read = fread (buffer,1, (s_sectorSize * number) ,s_fp);
		}
	}

    return byte_read;
}

/*********************************************************************/
/* update Sector size */
FATF_error_code_t FATF_UpdateSectorSize(const uint16_t new_sector_size)
{
    FATF_error_code_t status = FATF_SUCCESS;

    if(0 == new_sector_size % s_sectorSize)
    {
        s_sectorSize = new_sector_size;
        status = FATF_SUCCESS;
    }
    else
    {
        status = FATF_UPDATE_FAILED;
    }

    return status;
}

/*********************************************************************/
/* close the FAT file */
FATF_error_code_t FATF_Deinit(void)
{
    FATF_error_code_t status = FATF_SUCCESS;
    
    if(0 != fclose(s_fp))
    {
        status = FATF_DEINIT_FAILED;
    }
    
    return status;
}
/*********************************************************************/

