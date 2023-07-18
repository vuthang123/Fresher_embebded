#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "HAL.h"

/*********************************************************************
* API
*********************************************************************/

/* --------------static variable------------- */
static FAT_Entry_List_Struct_t *sp_dirHead = NULL;    /* pointer to first element of a directory */
static FAT_Boot_Struct_t s_App_FAT_Infor;   /* store disk information */
static uint32_t s_numElement;   /* number of file/subdirectory in a directory */
/*********************************************************************
* Prototype
*********************************************************************/

/*
* @brief: init app
* @param[in]: file path
* @return:none
*/
static void s_APP_Open(const uint8_t * const file_Path);

/*
* @brief: print file/subdir list of directory
* @param[in]: none
* @return: none
*/
static void s_APP_Print_Dir(void);

/*
* @brief: get option from user
* @param[in]: none
* @return: option
*/
static uint32_t s_APP_Get_Choice(void);

static void APP_PrintFile(uint8_t * buffer, uint32_t size);

/*
* @brief: read & show file/subdirectory of directory
* @param[in]: index of file/ subdirectory
* @return: none
*/
static void s_APP_Get_Dir(const uint32_t dirIndex);

/*
* @brief: deinit app
* @param[in]: none
* @return: none
*/
static void s_APP_Closed(void);

/*
* @brief: Run application to read directory
* @param[in]: file path
* @return: none
*/
void menu(const uint8_t * const file_Path);

#endif /* _APP_H_ */


