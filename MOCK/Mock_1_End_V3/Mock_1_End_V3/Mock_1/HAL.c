/*********************************************************************
* Include
*********************************************************************/

#include "HAL.h"

/*********************************************************************
* Definition
*********************************************************************/

/*********************************************************************
* Code
*********************************************************************/


FAT_error_code_t FAT_Init(const int8_t * const file_Path)
{
    FAT_error_code_t status = FAT_INIT_FAILED;

    if(FATF_SUCCESS == FATF_Init(file_Path))
    {
        /* get FAT information */
        if(FAT_SUCCESS == FAT_Read_Boot_Sector(&FAT_Infor))
        {
            /* update sector size */
            if(FATF_SUCCESS == FATF_UpdateSectorSize(FAT_Infor.byte_Per_Sector))
            {
                status = FAT_SUCCESS;
            }
        }
    }

    return status;
}

/****************************************************************************************************/
FAT_error_code_t FAT_Read_Boot_Sector(FAT_Boot_Struct_t * FAT_Infor)
{
    FAT_error_code_t status = FAT_SUCCESS;
    FAT_Boot_Struct_t FAT_INFOR;
    uint8_t buffer[512]; /* read 512 bytes of Boot Sector */
    uint8_t i = 0;
    uint32_t sector_Per_Vol = 0;
    uint32_t total_Cluster = 0;
	uint16_t byte_read = FATF_ReadSector(0,buffer);
    if(512 == byte_read)
    {
        /* Read boot */
        FAT_INFOR.byte_Per_Sector     = FAT_CONVERT_2BYTE(&buffer[OFFSET_BOOT_BYTES_PER_SEC]);
        FAT_INFOR.sector_Per_Cluster  = buffer[OFFSET_BOOT_SEC_PER_CLUS];
        FAT_INFOR.sector_PrevFAT      = buffer[OFFSET_BOOT_SEC_PREV_FAT];
        FAT_INFOR.number_FAT          = buffer[OFFSET_BOOT_NUM_FAT];
        FAT_INFOR.number_RootEntry    = FAT_CONVERT_2BYTE(&buffer[OFFSET_BOOT_NUM_ROOT_ENTRY]);
        sector_Per_Vol                = FAT_CONVERT_2BYTE(&buffer[OFFSET_BOOT_SEC_PER_VOL]);
        FAT_INFOR.sector_Per_FAT      = FAT_CONVERT_2BYTE(&buffer[OFFSET_BOOT_SEC_PER_FAT]);
        FAT_INFOR.FAT_Length       	  = FAT_INFOR.number_FAT*FAT_INFOR.sector_Per_FAT;
        FAT_INFOR.RDET_Length      	  = (FAT_INFOR.number_RootEntry*32)/FAT_INFOR.byte_Per_Sector;
        FAT_INFOR.byte_Per_Cluster    = FAT_INFOR.byte_Per_Sector*FAT_INFOR.sector_Per_Cluster;
        FAT_INFOR.start_Sector_Data   = (FAT_INFOR.sector_PrevFAT + FAT_INFOR.FAT_Length + FAT_INFOR.RDET_Length);
        total_Cluster 				  = (sector_Per_Vol - FAT_INFOR.start_Sector_Data)/FAT_INFOR.sector_Per_Cluster;
        /* check FAT type base on total cluster in volume */
        if(DEFAULT_MAX_SIZE_FAT12 >= total_Cluster)  /* FAT12 */
        {
        	FAT_INFOR.FAT_EOF = ENTRY_EOF_FAT12;
        }
        else if(DEFAULT_MAX_SIZE_FAT16 >= total_Cluster) /* FAT16 */
        {
            FAT_INFOR.FAT_EOF = ENTRY_EOF_FAT16;
        }
        else
        {
        	FAT_INFOR.FAT_EOF = ENTRY_EOF_FAT32; /* FAT32 */
		}
        *FAT_Infor = FAT_INFOR;
    }
    else
    {
        status = FAT_INIT_FAILED;
    }

    return status;
}

/****************************************************************************************************/
static uint32_t s_FAT_Read_Cluster(const uint32_t cur_Cluster, const bool root)
{
    uint32_t update_Cluster;     /* return value */
    uint8_t *p_buff = NULL;      /* FAT buffer */
    uint32_t secIndex;           /* sector include offset 'currentCluster' */
    uint32_t firstByteIndex;     /* first byte index of 'updateCluster' in buff */

    if(true == root)
    {
        update_Cluster = cur_Cluster + 1;
    }
    else
    {
        /* FAT12 */
        if(FAT_Infor.FAT_EOF == ENTRY_EOF_FAT12)
        {
            secIndex = FAT_Infor.sector_PrevFAT + ((cur_Cluster * 3) / 2) / FAT_Infor.byte_Per_Sector;
            firstByteIndex = ((cur_Cluster * 3) / 2) % FAT_Infor.byte_Per_Sector;
            
            /* read sector which include value 'updateCluster' */
            if(firstByteIndex == FAT_Infor.byte_Per_Sector - 1)    /* updateCluster is in 2 sector */
            {
                p_buff = (uint8_t *)malloc(2 * FAT_Infor.byte_Per_Sector);
                FATF_ReadMultiSector(secIndex,2,p_buff);
            }
            else
            
            {
                p_buff = (uint8_t *)malloc(FAT_Infor.byte_Per_Sector);
                FATF_ReadSector(secIndex,p_buff);
            }
            /* read 'updateCluster' */
            if(0 == cur_Cluster % 2)
            {
                update_Cluster = FAT_CONVERT_2BYTE(&p_buff[firstByteIndex]) & 0xFFF;
            }
            else
            {
                update_Cluster = (FAT_CONVERT_2BYTE(&p_buff[firstByteIndex]) & 0xFFF0) >> 4;
            }
            free(p_buff);
        }
        /* FAT16 */
        else if(FAT_Infor.FAT_EOF == ENTRY_EOF_FAT16)
        {
            secIndex = FAT_Infor.sector_PrevFAT + (cur_Cluster * 2) / FAT_Infor.byte_Per_Sector;
            firstByteIndex = (cur_Cluster * 2) % FAT_Infor.byte_Per_Sector;
            p_buff = (uint8_t *)malloc(FAT_Infor.byte_Per_Sector);
            /* read sector which include value 'updateCluster' */
            FATF_ReadSector(secIndex,p_buff);
            /* read 'updateCluster' */
            update_Cluster = FAT_CONVERT_2BYTE(&p_buff[firstByteIndex]);
            free(p_buff);
        }
        /* FAT32 */
        else
        {
        	secIndex = FAT_Infor.sector_PrevFAT + (cur_Cluster * 4) / FAT_Infor.byte_Per_Sector;
            firstByteIndex = (cur_Cluster * 4) % FAT_Infor.byte_Per_Sector;
            p_buff = (uint8_t *)malloc(FAT_Infor.byte_Per_Sector);
            /* read sector which include value 'updateCluster' */
            FATF_ReadSector(secIndex,p_buff);
            /* read 'updateCluster' */
            update_Cluster = FAT_CONVERT_2BYTE(&p_buff[firstByteIndex]);
            free(p_buff);
		}
    }

    return update_Cluster;
}

/****************************************************************************************************/
static FAT_entry_status_t s_FAT_Read_Entry(const uint8_t * pCurrent_Entry, FAT_Entry_List_Struct_t **head_entry_list)
{
    FAT_entry_status_t status;
    FAT_Entry_List_Struct_t *list_Dir;
    uint16_t i;
    static uint16_t longName[SIZE_ENTRY_LONG_NAME];
    static uint8_t longNameIndex = 0;
    uint16_t get_Date;
    uint16_t get_Time;
    uint16_t start_Cluster_L;
    uint16_t start_Cluster_H;

    /* parsing long file name entry */
    if(ENTRY_ATT_SUB_ENTRY == pCurrent_Entry[OFFSET_ENTRY_ATTRIBUTES])
    {
        /* read long name */
        for(i = 0; i < SIZE_ENTRY_LFN_FIRST; ++i)
        {
            longName[longNameIndex] = pCurrent_Entry[OFFSET_LFN_FIRST + 2*i];
            longNameIndex++;
        }
        for(i = 0; i < SIZE_ENTRY_LFN_NEXT; ++i)
        {
            longName[longNameIndex] = pCurrent_Entry[OFFSET_LFN_NEXT + 2*i];
            longNameIndex++;
        }
        for(i = 0; i < SIZE_ENTRY_LFN_LAST; ++i)
        {
            longName[longNameIndex] = pCurrent_Entry[OFFSET_LFN_LAST + 2*i];
            longNameIndex++;
        }
        /* out put */
        *head_entry_list = NULL;
        status = FAT_ENTRY_LFN;
    }
    /* parse last entry */
    else if(ENTRY_EMPTY == pCurrent_Entry[0])
    {
        /* out put */
        *head_entry_list = NULL;
        status = FAT_ENTRY_LAST;
    }
    /* parsing normal entry */
    else
    {
        /* read entry information */
        list_Dir = (FAT_Entry_List_Struct_t *)malloc(sizeof(FAT_Entry_List_Struct_t));
        list_Dir->next = NULL;
        /* read short name */
        for(i = 0; i < SIZE_ENTRY_SORT_NAME; ++i)
        {
            list_Dir->name[i] = pCurrent_Entry[i];
        }
        list_Dir->name[SIZE_ENTRY_SORT_NAME] = '\0';
        /* read type */
        for(i = 0; i < SIZE_ENTRY_TYPE; ++i)
        {
            list_Dir->entryType[i] = pCurrent_Entry[OFFSET_ENTRY_TYPE + i];
        }
        list_Dir->entryType[SIZE_ENTRY_TYPE] = '\0';
        /* read attributes */
        list_Dir->attributes = pCurrent_Entry[OFFSET_ENTRY_ATTRIBUTES];
        /* read modified time */
        get_Date = FAT_CONVERT_2BYTE(&pCurrent_Entry[OFFSET_ENTRY_DATE_MODIFY]);
        list_Dir->modifyDay 	= (get_Date & 0x1F);
        list_Dir->modifyMonth 	= (get_Date & 0x1E0) >> 5;
        list_Dir->modifyYear 	= (get_Date >> 9) + 1980U;
        get_Time 				= FAT_CONVERT_2BYTE(&pCurrent_Entry[OFFSET_ENTRY_TIME_MODIFY]);
        list_Dir->modifySec 	= 2*(get_Time & 0x1F);
        list_Dir->modifyMin 	= (get_Time & 0x7E0) >> 5;
        list_Dir->modifyHour 	= (get_Time >> 11);
        /* read start clus */
        start_Cluster_H = FAT_CONVERT_2BYTE(&pCurrent_Entry[OFFSET_ENTRY_CLUS_HIGH]);
        start_Cluster_L = FAT_CONVERT_2BYTE(&pCurrent_Entry[OFFSET_ENTRY_CLUS_LOW]);
        list_Dir->start_Cluster = ((start_Cluster_H <<16) | start_Cluster_L);
        /* read size of file */
        list_Dir->fileSize = FAT_CONVERT_4BYTE(&pCurrent_Entry[OFFSET_ENTRY_FILE_SIZE]);
        /* if has long name => get long name instead of short name */
        if(0 != longNameIndex)
        {
            for(i = 0; i < longNameIndex; ++i)
            {
                list_Dir->name[i] = longName[i];
            }
            list_Dir->name[longNameIndex] = '\0';
            longNameIndex = 0;
        }
        /* output */
        *head_entry_list = list_Dir;
        status = FAT_ENTRY_NORMAL;
    }
    return status;
}

/****************************************************************************************************/
static void s_FAT_Dir_List(void)
{
    FAT_Entry_List_Struct_t *ptr = NULL;

    while(sp_head != NULL)
    {
        ptr = sp_head;
        sp_head = sp_head->next;
        free(ptr);
        ptr = NULL;
    }
}

/****************************************************************************************************/
FAT_error_code_t FAT_Read_Dir(const uint32_t start_cluster, FAT_Entry_List_Struct_t **head_entry_list)
{
    FAT_error_code_t status = FAT_SUCCESS;
    FAT_entry_status_t statusEntry;
    FAT_Entry_List_Struct_t *list_Dir;
    uint32_t entryIndex = 0;
    uint32_t clusterIndex = start_cluster;
    uint32_t start_Sector_Index;
    uint8_t clusterBuff[FAT_Infor.byte_Per_Cluster];
    bool root = false;
    bool EOFDir = false;

    /* reset dir List */
    s_FAT_Dir_List();
    if(0 == start_cluster)
    {
        root = true;
    }
    while((clusterIndex != FAT_Infor.FAT_EOF) && (false == EOFDir))
    {
        /* starting sector of root */
        if(root)
        {
            start_Sector_Index = FAT_Infor.FAT_Length + FAT_Infor.sector_PrevFAT + clusterIndex * FAT_Infor.sector_Per_Cluster;
        }
        /* stating sector of cluster in data */
        else
        {
            start_Sector_Index = FAT_Infor.start_Sector_Data + (clusterIndex-2)*FAT_Infor.sector_Per_Cluster;
        }
        /* read cluster */
        FATF_ReadMultiSector(start_Sector_Index, FAT_Infor.sector_Per_Cluster, &clusterBuff[0]);
        while(FAT_Infor.byte_Per_Cluster > entryIndex)
        {
        	/* Read entry */
            statusEntry = s_FAT_Read_Entry(&clusterBuff[entryIndex], &list_Dir);
            if(FAT_ENTRY_NORMAL == statusEntry)
            {
                /* update dir list */
                if(NULL == sp_head)
                {
                    sp_head = list_Dir;
                    sp_tail = list_Dir;
                }
                else
                {
                    sp_tail->next = list_Dir;
                    sp_tail = sp_tail->next;
                }
            }
            else if(FAT_ENTRY_LAST == statusEntry)
            {
                /* end loop */
                entryIndex = FAT_Infor.byte_Per_Cluster;
            }
            entryIndex += 32;
		}
        if(NULL != list_Dir)   /* if list tail == NULL */
        {
            clusterIndex = s_FAT_Read_Cluster(clusterIndex, root);
        }
        else
        {
            EOFDir = true;
        }
    }
    /* return pointer head of directory */
    *head_entry_list = sp_head;
    if(NULL != *head_entry_list)
    {
        status = FAT_SUCCESS;
    }
    else
    {
        status = FAT_READ_DIR_FAILED;
    }

    return status;
}

/****************************************************************************************************/
FAT_error_code_t FAT_Read_File(const uint32_t start_cluster, uint8_t * pfileBuff)
{
    FAT_error_code_t status = FAT_SUCCESS;
    uint32_t start_Sector_Index;
    uint32_t position_cluster = start_cluster;
    uint32_t buffIndex = 0;
    
    while(position_cluster != FAT_Infor.FAT_EOF)
    {
        if(0 == (buffIndex % FAT_Infor.byte_Per_Cluster))
        {
        
            start_Sector_Index = FAT_Infor.start_Sector_Data + (position_cluster - 2) * FAT_Infor.sector_Per_Cluster;
            buffIndex += FATF_ReadMultiSector(start_Sector_Index, FAT_Infor.sector_Per_Cluster, &pfileBuff[buffIndex]);
            position_cluster = s_FAT_Read_Cluster(position_cluster, false);  	    
        }
        else
        {
            status = FAT_READ_FILE_FAILED;
        }
    } 
    return status;
}

/****************************************************************************************************/
FAT_error_code_t FAT_Deinit(void)
{
    FAT_error_code_t status = FAT_SUCCESS;

    if(FATF_DEINIT_FAILED == FATF_Deinit())
    {
        status = FAT_DEINIT_FAILED;
    }
    else
    {
        status = FAT_SUCCESS;
    }

    return status;
}


