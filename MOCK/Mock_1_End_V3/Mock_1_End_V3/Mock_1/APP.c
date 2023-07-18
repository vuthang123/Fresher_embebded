/*********************************************************************
* Include
*********************************************************************/
#include "APP.h"
/*********************************************************************
 * Definitions
 ********************************************************************/

/*********************************************************************
 * Prototypes
 ********************************************************************/

/*********************************************************************
 * Variables
 ********************************************************************/

/*********************************************************************
 * Code
 ********************************************************************/


/* Open app */
static void s_APP_Open(const uint8_t *const file_Path)
{
    if(FAT_SUCCESS == FAT_Init(file_Path))
    {
        FAT_Read_Boot_Sector(&s_App_FAT_Infor);
    }
    else
    {
        printf("\nInit unsuccesfully");
    }
}

/*********************************************************************/
/* print dir list*/
static void s_APP_Print_Dir(void)
{
    FAT_Entry_List_Struct_t *ptr = NULL;

    s_numElement = 1;
    system("cls");
    printf("\nIndex\t\tName\t\t\tDate modified\t\t\tType\t\tSize\n\n");
    ptr = sp_dirHead;
    while(ptr != NULL)
    {
        printf("%d", s_numElement);
        printf("\t\t%s", ptr->name);
        printf("\t\t%.2d/%.2d/%d", ptr->modifyDay, ptr->modifyMonth, ptr->modifyYear);
        printf(" %.2d:%.2d:%.2d",ptr->modifyHour, ptr->modifyMin, ptr->modifySec);
        printf("\t\t%s", ptr->entryType);
        printf("\t\t%d bytes", ptr->fileSize);
        printf("\n");
        ptr = ptr->next;
        s_numElement++;
    }
}

/*********************************************************************/
static void APP_PrintFile(uint8_t * buffer,uint32_t size)
{
    uint32_t index;
    for(index = 0; index < size; ++index)
    {
        printf("%c", buffer[index]);
    }
}

/*********************************************************************/
/* get index option */
static uint32_t s_APP_Get_Choice(void)
{
    uint32_t option;

    printf("\nEnter the option: ");
    scanf("%d",&option);

    return option;
}

/*********************************************************************/
/* show file/ folder */
static void s_APP_Get_Dir(const uint32_t dirIndex)
{
    uint32_t i;
    FAT_Entry_List_Struct_t *ptr = sp_dirHead;
    uint32_t fileBuffSize;
    uint8_t *pfileBuff = NULL;

    for(i = 0; i < dirIndex-1; i++)
    {
        ptr = ptr->next;
    }
    if(ptr->attributes == 0x10u)
    {
        /* read folder (subdirectory) */
        if(FAT_READ_DIR_FAILED != FAT_Read_Dir(ptr->start_Cluster, &sp_dirHead))
        {
            /* print folder */
            s_APP_Print_Dir();
        }
        else
        {
            printf("\nCan not read folder.");
        }
    }
    else
    {
        /* read file */
        fileBuffSize = ((ptr->fileSize/s_App_FAT_Infor.byte_Per_Sector)+1)*(s_App_FAT_Infor.byte_Per_Sector*s_App_FAT_Infor.sector_Per_Cluster);
        pfileBuff = (uint8_t *)malloc(fileBuffSize);
        if(FAT_READ_FILE_FAILED != FAT_Read_File(ptr->start_Cluster, pfileBuff))
        {
            fwrite(pfileBuff, 1, ptr->fileSize, stdout);
            printf("\n");
        }
        else
        {
            printf("\nCan not read file");
        }
        free(pfileBuff);
    }
}

/*********************************************************************/
/* deinit app */
static void s_APP_Closed(void)
{
    if(FAT_DEINIT_FAILED == FAT_Deinit())
    {
        printf("\nDeinit unsuccesfully.");
    }
}

/*********************************************************************/
/* run app */
void menu(const uint8_t * const file_Path)
{
    uint32_t root = 0;
    uint32_t option;

    /* init app */
    s_APP_Open(file_Path);
    /* read root directory */
    FAT_Read_Dir(root, &sp_dirHead);
    s_APP_Print_Dir();
    option = s_APP_Get_Choice();
    while(option <= s_numElement)    /* get option */
    {
        s_APP_Get_Dir(option);
        option = s_APP_Get_Choice();        
    }
    /* deinit app */
    s_APP_Closed();
}


