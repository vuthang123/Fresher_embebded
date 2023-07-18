/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "FATFS.h"
#include "APP.h"

/*******************************************************************************
* Define
*******************************************************************************/
#define MAX_FILE_NAME 255U

/*******************************************************************************
* Prototypes
*******************************************************************************/
/**
 * @brief print Error
 */
void printError(FATFS_Error_t status);

/*******************************************************************************
* Code
*******************************************************************************/

int main(void)
{
    FATFS_Error_t retval;
    uint8_t fileName[MAX_FILE_NAME];

    printf("Open file:");
    fflush(stdin);
    gets(fileName);
    retval = APP(fileName);
    printError(retval);

    return 0;
}

void printError(FATFS_Error_t status)
{
    switch(status)
    {
        case 1:
            printf("Error:Open file failed");
            break;
        case 2:
            printf("Error:Read directory failed");
            break;
        case 3:
            printf("Error:Read file failed");
            break;
        case 4:
            printf("Error:Read folder failed");
            break;
    }
}
/*******************************************************************************
* EOF
*******************************************************************************/
