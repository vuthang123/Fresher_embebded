#include "Application.h"
#define FOURS (4000)
/*******************************************************************************
 * Prototypes
******************************************************************************/
/**
 * @func:   Initialization on application
 * @brief:  Function Initialization for Queue and Uart
 * @param:  None
 * @return: None 
 **/
static void Application_Init(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @func:   Initialization on application
 * @brief:  Function Initialization for Uart
 * @param:  None
 * @return: None 
 **/

static void Application_Init(void)
{  
    uart_conf_stuct_t uart = 
    {
   
        .Con_fig_bits         = 8,               
        .parityEnable         = false,          
        .parityType           = 0,                 
        .select_sort_byte     = 0,           
        .Baud_Rate            = 115200,          
        .select_Tooggle_bit   = 0,         
        .Select_Tx_Rx         = 0,              
        .Scale                = {0,0},
        .stopbit            = 0,
    };
    /*Initialization for Uart */
    Driver_UART_Init(&uart);
    
}

/**
    @brief:Before running the program, you must load the bootloader before entering 
    the bootloader, press the sw2 button into the bootloader and send the file down 
    by Hercules or Terminal when the load is complete, press the reset button and 
    then it will automatically run the program BootAPP.

    Press hold button sw3 to enter bootloader -> finish loading srec file 
    -> then press reset run app
    
*/
int main()
{
    DRIVBUTTON_init(PORTD);
    
    Application_Init();
        
    if(((FGPIOD->PDIR) & (SW3)) == 0) 
    {
         BOOT_LOADER();
    }

    BOOT_APP();
    while (1);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/