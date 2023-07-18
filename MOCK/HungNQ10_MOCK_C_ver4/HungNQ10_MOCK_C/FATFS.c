/*******************************************************************************
* Include
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "HAL.h"
#include "FATFS.h"
#include "APP.h"

/*******************************************************************************
* Define
*******************************************************************************/
#define SIZE_BOOTSECTOR     512                  /* Size of the boot sector in bytes */
#define CONVERT_2BYTE(a,b)    (((b) << 8) | (a)) /* Convert two bytes to a single integer value */
#define CLUSTER(x, y)       (((uint32_t)(y)<<16) | (x))/* Combine two 16-bit integers to form a 32-bit integer */
#define FATFS_LITTLE_ENDIAN_2_BYTE(X) (((*((X) + 1)) << 8)|(*(X))) /* Convert a little-endian representation of a 2-byte integer to a native integer */
#define FATFS_LITTLE_ENDIAN_4_BYTE(X) (((*((X) + 3))<<24)|((*((X) + 2)) << 16)|((*((X) + 1)) << 8)|(*(X))) /* Convert a little-endian representation of a 4-byte integer to a native integer */
#define FATFS_MAX_VOLUME_FAT12                     4085U /* Maximum number of clusters in a FAT12 volume */
#define FATFS_MAX_VOLUME_FAT16                     65525U/* Maximum number of clusters in a FAT16 volume */
#define FATFS_DIR_ENTRY_DELETED                    0xE5U /* Value indicating a deleted directory entry */
#define FATFS_DIR_ENTRY_END                        0x00U /* Value indicating the end of a directory entry */
#define FATFS_ENTRY_RESERVED                       0x0CU /* Value indicating a reserved directory entry */
#define BYTE_ENTRY                                 32U   /* Size of a directory entry in bytes */

/* BOOT SECTOR */
#define FATFS_BYTE_PER_SECTOR                      0x0BU /* Offset in boot sector for the number of bytes per sector */
#define FATFS_SECTOR_PER_CLUSTER                   0x0DU /* Offset in boot sector for the number of sectors per cluster */
#define FATFS_RESERVED_SECTOR                      0x0EU /* Offset in boot sector for the number of reserved sectors */
#define FATFS_NUMBER_OF_FAT                        0x10U /* Offset in boot sector for the number of FAT tables */
#define FATFS_NUMBER_ENTRY                         0x11U /* Offset in boot sector for the number of entries in root directory */
#define FATFS_TOT_SECTORS_16                       0x13U /* Offset in boot sector for the total number of logical sectors */
#define FATFS_SIZE_INDEX                           0x16U /* Offset in boot sector for the size of a FAT table */
#define FATFS_TOT_SECTORS_32                       0x20U /* Offset in boot sector for the total number of logical sectors */


/* ROOT DIRECTORY */
#define FATFS_ATTRIBUTE                            0x0BU  /* Attribute */
#define FATFS_CREATION_TIME                        0x0CU  /* Creation time */
#define FATFS_CREATION_DATE                        0x10U  /* Creation date */
#define FATFS_LAST_ACCESS_DATE                     0x12U  /* Last access date */
#define FATFS_HIGHCLUSTER_NUM                      0x14U  /* High cluster number */
#define FATFS_TIME                                 0x16U  /* Time */
#define FATFS_DATE                                 0x18U  /* Date */
#define FATFS_LOWCLUSTER_NUM                       0x1AU  /* Low cluster number */
#define FATFS_SIZE                                 0x1CU  /* Size */

/*******************************************************************************
* Prototypes
*******************************************************************************/

/**
 * @brief read Boot sector
 */
static void FATFS_HandleBootSector(void);

/**
 * @brief detlete all nodes
 *
 * @param[in_out] head first address of the list list
 */
static void FATFS_DeleteList(FATFS_EntryList_Struct_t **head);

/**
 * @brief cluster next
 *
 * @param[in] fat fat table
 * @param[in_out] cluster next cluster
 */
static void FATFS_NextCluster(uint8_t *fat, uint32_t *cluster);

/*******************************************************************************
* Global variables
*******************************************************************************/
typedef struct
{
    uint32_t bytePerSector;       /* Number of bytes per sector */
    uint32_t entriesNumber;       /* Number of entries in the root directory */
    uint32_t sectorPerCluster;    /* Number of sectors per cluster */
    uint32_t fatSize;             /* Size of the FAT table */
    uint32_t startRoot;           /* Start sector for the root directory */
    uint16_t rootSize;            /* Size of the root directory */
    uint16_t startFat;            /* Start sector for the FAT table */
    uint16_t reserved;           /* Start sector for the data area */
    uint16_t numFAT;              /* Number of FAT tables */
    uint32_t totalLogicalSectors; /* Total number of logical sectors */
    uint16_t typeOf_Fat;          /* Type of FAT */
    uint32_t endOf_File;          /* End of the file marker */
}FATFS_BootSector_Parameter_struct_t;

uint32_t posRoot = 0;   /* position sector of root */
uint32_t startOfRoot = 0; /* value start of root */
static FATFS_BootSector_Parameter_struct_t bootInfo;/*Data of the boot sector*/

/*******************************************************************************
* Code
*******************************************************************************/

static void FATFS_HandleBootSector(void)
{
    uint8_t *buff = NULL;/*The pointer points to the data location*/
    uint16_t sectorSize = 0;/*Variable receiving updated value*/
    uint32_t totalCluster = 0;/*Maximum number of FATFS_CLUSTERs of managed fat tables*/
    
    buff = (uint8_t*)malloc(512);
    HAL_ReadSector(0,buff);/*Read data of area 0*/
    sectorSize = FATFS_LITTLE_ENDIAN_2_BYTE(&buff[FATFS_BYTE_PER_SECTOR]);
    if(512 != sectorSize )
    {
        HAL_UpdateSectorSize(sectorSize);
    }
    /*Information of the boot area*/
    bootInfo.bytePerSector = sectorSize;/*Number of bytes of a sector*/
    bootInfo.sectorPerCluster = buff[FATFS_SECTOR_PER_CLUSTER];
    bootInfo.fatSize = FATFS_LITTLE_ENDIAN_2_BYTE(&buff[FATFS_SIZE_INDEX]);
    bootInfo.entriesNumber = FATFS_LITTLE_ENDIAN_2_BYTE(&buff[FATFS_NUMBER_ENTRY]);
    bootInfo.numFAT = buff[FATFS_NUMBER_OF_FAT];
    bootInfo.startRoot = 1 + bootInfo.numFAT*bootInfo.fatSize;
    bootInfo.rootSize = (bootInfo.entriesNumber * BYTE_ENTRY)/bootInfo.bytePerSector;
    bootInfo.reserved = bootInfo.startRoot + bootInfo.rootSize;
    bootInfo.startFat = FATFS_LITTLE_ENDIAN_2_BYTE(&buff[FATFS_RESERVED_SECTOR]);
    if(buff[FATFS_TOT_SECTORS_16] != 0)/*Check if this field equal 0 or not */
    {
        bootInfo.totalLogicalSectors = FATFS_LITTLE_ENDIAN_2_BYTE(&buff[FATFS_TOT_SECTORS_16]);
    }
    else
    {
        bootInfo.totalLogicalSectors = FATFS_LITTLE_ENDIAN_4_BYTE(&buff[FATFS_TOT_SECTORS_32]);
    }
    /* Check Type of FAT */
    totalCluster = ((bootInfo.totalLogicalSectors - bootInfo.reserved)/bootInfo.sectorPerCluster - 2);
    if(totalCluster <= FATFS_MAX_VOLUME_FAT12)
    {
        bootInfo.typeOf_Fat = 1;/*Assign FAT12 to 1*/
        bootInfo.endOf_File = 0xFFF;/*end of FAT12*/
    }
    else if ((totalCluster > FATFS_MAX_VOLUME_FAT12) && (totalCluster <= FATFS_MAX_VOLUME_FAT16))
    {
        bootInfo.typeOf_Fat = 0;/*Assign FAT16 to 0*/
        bootInfo.endOf_File = 0xFFFF;/*end of FAT16*/
    }
    free(buff);
}

/* delete all link list */
static void FATFS_DeleteList(FATFS_EntryList_Struct_t **head)
{
    FATFS_EntryList_Struct_t *current = NULL;
    FATFS_EntryList_Struct_t *next = NULL;

    current = *head;
    while(current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

static void FATFS_NextCluster(uint8_t *fat, uint32_t *cluster)
{
    uint8_t firtsCluster=0;/*front cluster*/
    uint8_t secondCluster=0;/*adjacent cluster*/
    uint8_t indexSector=0;/*the index of sector*/
    uint16_t byteOffset=0;/*number of bytes in an offset*/
    uint16_t offset=0;

    if(1 == bootInfo.typeOf_Fat)
    {
        byteOffset = (*cluster * 3)/2+bootInfo.bytePerSector*bootInfo.startFat;
    }
    else
    {
//        byteOffset = (*cluster * 2)+bootInfo.bytePerSector;
        byteOffset = (*cluster * 2)+bootInfo.bytePerSector*bootInfo.startFat;
    }
    indexSector = byteOffset/bootInfo.bytePerSector;
    offset = byteOffset%bootInfo.bytePerSector;
    HAL_ReadSector(indexSector,fat);
    if((bootInfo.bytePerSector - 1) == offset)
    /*Check if it's in the last position in the sector*/
    {
        firtsCluster = fat[offset];/*Assign first_cluster to the last value of sector*/
        indexSector++;/*Jump to next sector*/
        HAL_ReadSector(indexSector,fat);
        secondCluster = fat[0];/*Assign second_cluster to the first value of the next sector*/
    }
    else/*If the first and second positions are both within a sector*/
    {
        firtsCluster = fat[offset];
        secondCluster = fat[offset+1];
    }
    if(1 == bootInfo.typeOf_Fat)/*if it's fat 12*/
    {
        if(0 == (*cluster % 2))/*cluster is an even number*/
        {
            *cluster = ((secondCluster)&0x0F) << 8 | firtsCluster;
        }
        else /*cluster_source is odd*/
        {
            *cluster = ((secondCluster)<<4)|((firtsCluster)>>4);
        }
    }
    else if(0 == bootInfo.typeOf_Fat)/*if it's fat 16*/
    {
        *cluster = CONVERT_2BYTE(firtsCluster,secondCluster);
    }

}

FATFS_Error_t FATFS_Init(const char * FilePath, uint32_t *fristartOfRoot, uint16_t *sizeSector)
{
    uint8_t *buff = NULL;
    HAL_Error_t error = HAL_success;
    FATFS_Error_t status = FATFS_OK;

    error = HAL_Init(FilePath); /* read file */
    buff = malloc(SIZE_BOOTSECTOR);
    if (NULL == buff || error != HAL_success)
    {
        status = FATFS_InitFail;
    }
    else
    {
        /* read bootsector */
        FATFS_HandleBootSector();
        *sizeSector = bootInfo.bytePerSector;
        startOfRoot  = bootInfo.startFat + ((bootInfo.numFAT)*bootInfo.fatSize);
        *fristartOfRoot = startOfRoot;
        posRoot = bootInfo.startFat + ((bootInfo.numFAT)*bootInfo.fatSize) + bootInfo.rootSize;
    }

    return status;
}

FATFS_Error_t FATFS_ReadDirectory(FATFS_EntryList_Struct_t **head, uint32_t position)
{
    uint8_t* buffer = NULL;
    FATFS_EntryList_Struct_t *temp = NULL;
    FATFS_EntryList_Struct_t *ptr = NULL;
    FATFS_Error_t status = FATFS_OK;
    uint32_t sectorOfRoot = 0;
    uint32_t index = 0;

    sectorOfRoot = bootInfo.rootSize;/* sector number of FAT */
    
    buffer =(uint8_t *)malloc(sectorOfRoot * bootInfo.bytePerSector);

    
    if (buffer == NULL)
    {
        status = FATFS_ReadDirFail;
    }
    else if (HAL_ReadMultiSector(position, sectorOfRoot, buffer) != (int32_t)sectorOfRoot * bootInfo.bytePerSector) /* read buffer to position */
    {
        status = FATFS_ReadDirFail;
    }
    else
    {
        while (buffer[index] != FATFS_DIR_ENTRY_END) /* check DIR_ENTRY_END */
        {
            if ((buffer[index] != FATFS_DIR_ENTRY_DELETED)) /* check the file occupying the entry has been deleted */
            {
                if (buffer[0x0B + index] != 0x0F) /* check entry main */
                {
                	
                    temp = (FATFS_EntryList_Struct_t *)malloc(sizeof(FATFS_EntryList_Struct_t));
                    if(temp == NULL)
                    {
                        status = FATFS_ReadDirFail;
                    }
                    /* copy information of entry */
                    memcpy(temp->entry.fileName, &buffer[index], 11);
                    temp->entry.attributes = buffer[FATFS_ATTRIBUTE + index];
                    memcpy(temp->entry.reserved, &buffer[FATFS_ENTRY_RESERVED + index],2);
                    temp->entry.timeStamp = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_CREATION_TIME+index]));
                    temp->entry.dateStamp = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_CREATION_DATE+index]));
                    temp->entry.accessDate = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_LAST_ACCESS_DATE+index]));
                    temp->entry.clusterHi = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_HIGHCLUSTER_NUM+index]));
                    temp->entry.writeTime = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_TIME+index]));
                    temp->entry.writeDate = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_DATE+index]));
                    temp->entry.clusterLOW = (uint16_t)(FATFS_LITTLE_ENDIAN_2_BYTE(&buffer[FATFS_LOWCLUSTER_NUM+index]));
                    temp->entry.fileSize = (uint32_t)(FATFS_LITTLE_ENDIAN_4_BYTE(&buffer[FATFS_SIZE+index]));
                    temp->next = NULL;
                    if (*head == NULL)
                    {
                        *head = temp;
                    }
                    else
                    {
                        ptr = *head;
                        while (ptr->next != NULL)
                        {
                            ptr = ptr->next;
                        }
                        ptr->next = temp;
                    }
                }
            }
            /* move to the next entry address */
            index += 32;
        }
    }
    free(buffer);

    return status;
}

FATFS_Error_t FATFS_ReadFile(uint32_t cluster, uint8_t *buffer)
{
    uint32_t position = 0;
    FATFS_Error_t status = FATFS_OK;
    uint8_t *fat = NULL;
    uint32_t fatSize = 0;
    uint32_t count = 0;

    fatSize = bootInfo.fatSize*bootInfo.bytePerSector;
    fat = (uint8_t *)malloc(fatSize); /* allocate some memory for FAT */
    if (fat == NULL)
    {
        status = FATFS_ReadFileFail;
    }
    else if(HAL_ReadMultiSector(bootInfo.startFat, bootInfo.fatSize, fat) != (int32_t)fatSize) /* read table FAT*/
    {
        status = FATFS_ReadFileFail;
    }
    do /* check end of file */
    {
        position = (cluster - 2)*bootInfo.sectorPerCluster + posRoot; /* position of sector need to read */
        HAL_ReadMultiSector(position, 1, buffer + bootInfo.bytePerSector*count);
        count++;
        FATFS_NextCluster(fat, &cluster); /* next cluster*/
    }while (cluster != bootInfo.endOf_File);
    free(fat);

    return status;
}

/* get attributes, cluster, file size of entry */
void FATFS_PosEntry(FATFS_EntryList_Struct_t **head, FATFS_PosEntry_Struct_t *posEntry, uint8_t select)
{
    FATFS_EntryList_Struct_t *ptr = NULL;
    uint8_t count = 0;

    ptr = *head;
    while (ptr != NULL)
    {
        if((select - 1) == count)
        {
            posEntry->attributes = ptr->entry.attributes;
            posEntry->cluster = CLUSTER(ptr->entry.clusterLOW, ptr->entry.clusterHi);
            posEntry->fileSize = ptr->entry.fileSize;
        }
        count++;
        ptr = ptr->next;
    }
}

FATFS_Error_t FATFS_LoadDirectory(FATFS_EntryList_Struct_t **head, uint32_t cluster)
{
    uint8_t *fat = NULL;
    FATFS_Error_t status = FATFS_OK;
    uint32_t position = 0;
    uint32_t fatSize = 0;

    fatSize = bootInfo.fatSize* bootInfo.bytePerSector;
    fat = (uint8_t *)malloc (fatSize); /* allocate some memory for FAT */
    if (fat == NULL)
    {
        status = FATFS_ReadFolderFail;
    }
    if(HAL_ReadMultiSector(bootInfo.startFat, bootInfo.fatSize, fat) != (int32_t)fatSize) /* read table FAT*/
    {
        status = FATFS_ReadFolderFail;
    }
    else
    {
        FATFS_DeleteList(head); /* delete all link list */
        /* position sector need to read */
        if(cluster == 0)
        {
            position = startOfRoot;
        }
        else
        {
            position = (cluster - 2) + posRoot;
        }
        /* read the file's contents */
        while (cluster < bootInfo.endOf_File)
        {
            FATFS_ReadDirectory(head, position);
            if(cluster == 0)
            {
                cluster = bootInfo.endOf_File;
            }
            else
            {
                FATFS_NextCluster(fat, &cluster); /* next cluster */
            }
        }
    }
    free(fat);

    return status;
}

/* Close file */
void FATFS_Deinit()
{
    HAL_DeInit();
}
/*******************************************************************************
* EOF
*******************************************************************************/
