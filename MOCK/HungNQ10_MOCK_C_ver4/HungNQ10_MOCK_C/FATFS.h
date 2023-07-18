/*******************************************************************************
* Define
*******************************************************************************/
#ifndef _FATFS_H_
#define _FATFS_H_

#define FATFS_ATTR_VOLUME_ID                       0x08U /* attribute Volume ID offset */
#define FATFS_ATTR_DIRECTORY                       0x10U /* Attribute directory offset */


typedef enum
{
    FATFS_OK = 0, /* Process succeed */
    FATFS_InitFail = 1, /* Initialization failed */
    FATFS_ReadDirFail = 2, /* Read directory failed */
    FATFS_ReadFileFail = 3, /* Read file failed */
    FATFS_ReadFolderFail = 4, /* Read folder failed */
}FATFS_Error_t;

/* format of entry */
typedef struct {
    uint8_t     fileName[11];   /* filename extenstion */
    uint8_t     reserved[2];    /* reserved */
    uint16_t    timeStamp;      /* creation time */
    uint16_t    dateStamp;      /* creation date */
    uint16_t    accessDate;     /* last access date */
    uint16_t    clusterHi;      /* cluster high */
    uint16_t    writeTime;      /* last modification time */
    uint16_t    writeDate;      /* last modification date */
    uint16_t    clusterLOW;     /* cluster low*/
    uint32_t    fileSize;       /* file size  */
	uint8_t     attributes;     /* file attribute */
} FATFS_DirectoryEntry_Struct_t;

typedef struct LinkList
{
    FATFS_DirectoryEntry_Struct_t entry;
    struct LinkList *next;
}FATFS_EntryList_Struct_t;

typedef struct posEntry
{
    uint32_t    cluster;     /* The cluster number of the file or directory */
    uint32_t    fileSize;    /* The size of the file, in bytes */
	uint8_t     attributes;  /* The attributes of the file or directory */
} FATFS_PosEntry_Struct_t;

/*******************************************************************************
* Prototypes
*******************************************************************************/

/**
 * @brief read file and bootsector
 *
 * @param[in] FileFath fath file
 * @param[in_out] startOfRoot position read root
 * @param[in_out] sizeSector number of 1 sector
 * @return FATFS_Error_t error
 */
FATFS_Error_t FATFS_Init(const char * FilePath, uint32_t *fristartOfRoot, uint16_t *sizeSector);

/**
 * @brief Read the sector at the location and create a link list for the file folder
 *
 * @param[in_out] head first address of the list list
 * @param[in] position position read
 * @return FATFS_Error_t error
 */
FATFS_Error_t FATFS_ReadDirectory(FATFS_EntryList_Struct_t **head, uint32_t position);

/**
 * @brief read file
 *
 * @param[in] cluster cluster to read
 * @param[in_out] buffer buffer stores the contents of the file
 * @return FATFS_Error_t error
 */
FATFS_Error_t FATFS_ReadFile(uint32_t cluster, uint8_t *buffer);

/**
 * @brief get attributes, cluster, file size of entry
 *
 * @param[in_out] head first address of the list list
 * @param[in_out] posEntry attributes, cluster, file size of entry
 * @param[in] select entry to read
 */
void FATFS_PosEntry(FATFS_EntryList_Struct_t **head, FATFS_PosEntry_Struct_t *posEntry, uint8_t select);

/**
 * @brief read folder 
 *
 * @param[in_out] head first address of the list list
 * @param[in] cluster cluster to read
 * @return uint8_t error
 */
FATFS_Error_t FATFS_LoadDirectory(FATFS_EntryList_Struct_t **head, uint32_t cluster);

/**
 * @brief fclose file
 */
void FATFS_Deinit();

#endif /* _FATFS_H_ */

/*******************************************************************************
* EOF
*******************************************************************************/
