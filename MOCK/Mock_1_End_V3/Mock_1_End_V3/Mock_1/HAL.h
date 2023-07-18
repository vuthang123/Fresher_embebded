#ifndef _HAL_H_
#define _HAL_H_

/*********************************************************************
* Definition
*********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FATFS.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* -----------Offset of Boot Sector----------------- */
#define OFFSET_BOOT_BYTES_PER_SEC         0x0BU   /* number of bytes per sector */
#define OFFSET_BOOT_SEC_PER_CLUS          0x0DU   /* number of sector per clus (data region) */
#define OFFSET_BOOT_SEC_PREV_FAT          0x0EU   /* number of sectors previous FAT */
#define OFFSET_BOOT_NUM_FAT               0x10U   /* number of FATs */
#define OFFSET_BOOT_NUM_ROOT_ENTRY        0x11U   /* number of root entries */
#define OFFSET_BOOT_SEC_PER_VOL           0x13U   /* total sector of volume (FAT16/12) */
#define OFFSET_BOOT_SEC_PER_FAT           0x16U   /* number of sector in 1 FAT12/16 */
/* -----------Offset of Directory Entry----------------- */
#define OFFSET_ENTRY_TYPE                 0x08U
#define OFFSET_ENTRY_ATTRIBUTES           0x0BU
#define OFFSET_ENTRY_CLUS_HIGH            0x14U
#define OFFSET_ENTRY_CLUS_LOW             0x1AU
#define OFFSET_ENTRY_TIME_MODIFY      	  0x16U
#define OFFSET_ENTRY_DATE_MODIFY	      0x18U
#define OFFSET_ENTRY_FILE_SIZE            0x1CU
/* ------------------------------------------------------*/
#define OFFSET_LFN_FIRST                  0x01U
#define OFFSET_LFN_NEXT                   0x0EU
#define OFFSET_LFN_LAST                   0x1CU
#define SIZE_ENTRY_LFN_FIRST              5U
#define SIZE_ENTRY_LFN_NEXT               6U
#define SIZE_ENTRY_LFN_LAST               2U
/* --------------------------------------------------- */
#define ENTRY_EMPTY                       0x00U
#define ENTRY_EOF_FAT12                   0xFFFU
#define ENTRY_EOF_FAT16                   0xFFFFU
#define ENTRY_EOF_FAT32                   0xFFFFFFFFU
#define SIZE_ENTRY_SORT_NAME              8U
#define SIZE_ENTRY_LONG_NAME              255U
#define SIZE_ENTRY_TYPE                   3U
#define ENTRY_ATT_SUB_ENTRY               0x0FU
#define DEFAULT_MAX_SIZE_FAT12        	  4085U
#define DEFAULT_MAX_SIZE_FAT16            65525U
#define DEFAULT_MAX_SIZE_FAT32            65526U
/* ------------Macro-like Funtion------------- */

/* convert 2 bytes */
#define FAT_CONVERT_2BYTE(buff)      			((buff)[0] | ((buff)[1]<< 8))
/* convert 4 bytes */
#define FAT_CONVERT_4BYTE(buff)      			((buff)[0] | ((buff)[1] << 8) | \
                                       			((buff)[2] << 16) | ((buff)[3] << 24))

typedef enum
{
    FAT_ENTRY_LFN = 0x00,
    FAT_ENTRY_NORMAL = 0x01,
    FAT_ENTRY_LAST = 0x02
}FAT_entry_status_t;

typedef enum
{
    FAT_SUCCESS = 0x00,
    FAT_INIT_FAILED = 0x01,
    FAT_READ_DIR_FAILED = 0x02,
    FAT_READ_FILE_FAILED = 0x03,
    FAT_DEINIT_FAILED = 0x04
} FAT_error_code_t;

/* Struct Boot Sector */
typedef struct
{
    uint16_t    byte_Per_Sector;
    uint8_t     sector_Per_Cluster;
    uint16_t    sector_PrevFAT;
    uint16_t 	byte_Per_Cluster;
    uint32_t	start_Sector_Data;
    uint32_t	sector_Per_FAT;
    uint8_t     number_FAT;
    uint16_t    number_RootEntry;
    uint32_t	FAT_Length;
    uint32_t	RDET_Length;
    uint32_t    first_Cluster;
    uint32_t	FAT_EOF;
}FAT_Boot_Struct_t;

/* Struct Directory Entry */
typedef struct entry
{
    uint8_t     name[256];
    uint8_t     entryType[4];
    uint8_t     attributes;
    uint32_t    start_Cluster;
    uint8_t     modifyDay;
    uint8_t     modifyMonth;
    uint16_t    modifyYear;
    uint8_t     modifyHour;
    uint8_t     modifyMin;
    uint8_t     modifySec;
    uint32_t    fileSize;  /* byte */
    struct entry *next;
}FAT_Entry_List_Struct_t;


/* ---------------Static Variable------------- */

static FAT_Boot_Struct_t FAT_Infor;    			/* store disk information */
static FAT_Entry_List_Struct_t *sp_head = NULL; 	/* point to first entry */
static FAT_Entry_List_Struct_t *sp_tail = NULL; 	/* point to last entry */

/*********************************************************************
* API
*********************************************************************/

/*
* @brief: init FAT
* @param[in]: FAT file path
* @return:  FAT_error_code_success - init successfully
*           FAT_error_code_fail - init fail
*/
FAT_error_code_t FAT_Init(const int8_t * const file_Path);

/*
* @brief: read boot sector & get disk information to init FAT
* @param[out]: pointer to Boot Sector struct
* @return:  FAT_error_code_success - read boot successfully
*           FAT_error_code_fail - read boot fail
*/
FAT_error_code_t FAT_Read_Boot_Sector(FAT_Boot_Struct_t * FAT_Infor);

/*
* @brief: read next cluster read
* @param[in1]: currentCluster - current cluster
* @param[in1]: root - current cluster is root or not
* @return: next cluster
*/
static uint32_t s_FAT_Read_Cluster(const uint32_t cur_Cluster, const bool root);

/*
* @brief: Read entry & create a Struct entry object
* @param[in]: entry_buff - entry buffer
* @param[out]: parsed_entry - pointer to the entry after parse
* @return:  FAT_ENTRY_LFN - if parsing long file name,
*           FAT_ENTRY_NORMAL - if parsing normal entry,
*           FAT_ENTRY_LAST - if parsing last entry,
*/
static FAT_entry_status_t s_FAT_Read_Entry(const uint8_t * pCurrent_Entry, FAT_Entry_List_Struct_t **head_entry_list);

/*
* @brief: reset entry list
* @param: none
* @return: none
*/
static void s_FAT_Dir_List(void);
/*
* @brief: read directory
* @param[in]: start_cluster of directory
* @param[out]: pointer to the first file/subdirectory
* @return:  FAT_error_code_success - read directory successfully
*           FAT_error_code_read_dir - read directory fail
*/
FAT_error_code_t FAT_Read_Dir(const uint32_t start_cluster, FAT_Entry_List_Struct_t **head_entry_list);

/*
* @brief: read file
* @param[in]: start_cluster of file
* @param[out]: buffer of file
* @return:  FAT_error_code_success - read file successfully
*           FAT_error_code_read_file - read file fail
*/
FAT_error_code_t FAT_Read_File(const uint32_t start_cluster, uint8_t * pfileBuff);

/*
* @brief: de init FAT
* @param[in]: none
* @return:  FAT_error_code_success - deinit successfully
*           FAT_error_code_deinit - deinit fail
*/
FAT_error_code_t FAT_Deinit(void);

#endif /* _HAL_H_ */


