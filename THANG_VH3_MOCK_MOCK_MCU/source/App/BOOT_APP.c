#include "BOOT_APP.h"
#include "Driver_Flash.h" 
#include "Driver_Led.h"
#include "Driver_UART.h"
#include "MIDware_QUEUE.h"
#include "MIDware_SREC.h"
#include "MKE16Z4.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOOT_ADDRESS_START_STACK_POINTER_APP        (0xA000U)
#define BOOT_ADDRESS_RESET_HANDLER_APP              (0xA004U)
#define NEW_LINE_CHAREACTER                         (10U)
       
/*******************************************************************************
 * Prototypes
******************************************************************************/
/* Use for call app reset handler */
static void(*fptr)(void); 
/**
 * @func:   Initialization on application
 * @brief:  Function Initialization for Queue and Uart
 * @param:  None
 * @return: None 
 **/
void Application_Init(void);
/**
 * @func    Application_handle_rx_data_intterupt
 * @brief:  Function to handler interupt receiver
 * @paramV  u8Data : Reciver Interupt Data
 * @return: None 
 **/
static void Application_handle_receiver_data_intterupt(uint8_t data);

/**
    @brief: The BOOT_LOADER function runs the process of loading the program 
    from the UART interface into Flash memory.
    @param: No parameters.
    @return: No return value.
*/
void BOOT_LOADER(void);
/**
   @brief: Switch to running the main application (app).
   *This function is used to convert from the boot program (bootloader) to the 
   * main application (application).
   *The steps performed in the function include:
   * Disable interrupts by calling __disable_irq().
   *Update the stack pointer address (Stack pointer) with the value read from 
   * the flash address BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Update the vector Table Offset Register (VTOR) space with the flash address
     value BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Re-enable interrupts by calling __enable_irq().
   * Reset the main application function via the function pointer ptr_app, the 
     value of this pointer is read from the flash address BOOT_ADDRESS_RESET_HANDLER_APP.
   * This will start the main application execution.
   * This function ensures that the transition from bootloader to main application 
     is smooth and precise,ensure the transition from the boot process to the 
     main application of the system.
*/
void BOOT_APP(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
    @brief: The BOOT_LOADER function runs the process of loading the program 
    from the UART interface into Flash memory.
    @param: No parameters.
    @return: No return value.
*/
void BOOT_LOADER(void)
{
    uint8_t  *u8Input_Data_Queue = NULL;
    uint8_t  *newlinw  = "\n";
    uint32_t index =0 ;
    uint32_t u32Address_Flash;
    uint8_t  *pu8Data_Flash;
    Middleware_Srec_Pasre_data_line_struct_t s_Output = {0,0,{0}};
    
    /*Initialization state for Srec parse data line */
    Middleware_Srec_Pasre_status s_Status =  MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
    /*Initialization for Queue */
    MIDware_QueueInit();
    /*Handler Data while there interrupt */
    Driver_Uart_Hander_Data(Application_handle_receiver_data_intterupt);
    /*hander interrupt from Hal uart*/
    Driver_Uart_Rx();
    /*Clear Flash*/
    DRIFLASH_AllEraseFlash(BOOT_ADDRESS_START_STACK_POINTER_APP,32U);
    while(1)
    {     
          if (MIDware_Queue_Empty() == false)
          {
                  u8Input_Data_Queue = MIDware_Queue_Popdata();
                  s_Status = Middleware_SREC_Parse(u8Input_Data_Queue,&s_Output);
                  MIDware_Queue_Dequeue();
                 
                  switch(s_Status)
                  {
                      case MIDDLEWARE_SREC_PASRE_STATE_START:          
                      {
                           Driver_LPUART0_Transmission_Milt_byte("Start\r\n",Check_length_str("Start\r\n"));
                           Driver_LPUART0_Transmission_Milt_byte(newlinw,1);
                           break;
                            
                      }
                      case MIDDLEWARE_SREC_PASRE_STATE_HANDLE:          
                      {    
                        for(index = 0; index < (s_Output.u32Data_length); index+=4U)
                        {
                           u32Address_Flash = s_Output.u32Address + (index);
                           pu8Data_Flash    = &s_Output.u8Data[index]; 
                           DRIFLASH_Write_Flash(u32Address_Flash, pu8Data_Flash);
                           
                           Driver_LPUART0_Transmission_Milt_byte(">>\r\n",Check_length_str(">>\r\n"));
                           Driver_LPUART0_Transmission_Milt_byte(newlinw,1);
                        }
                           break;                            
                      }
                      case MIDDLEWARE_SREC_PASRE_STATE_COMPLETE:
                      {
                          Driver_LPUART0_Transmission_Milt_byte("Success\r\n",Check_length_str("Success\r\n"));
                          Driver_LPUART0_Transmission_Milt_byte(newlinw,1); 
                          break;
                      }
                      case MIDDLEWARE_SREC_PASRE_STATE_FAIL:
                      {
                           Driver_LPUART0_Transmission_Milt_byte("Error\r\n",Check_length_str("Error\r\n"));
                           Driver_LPUART0_Transmission_Milt_byte(newlinw,1);
                           
                           break;
                      }
                  }   
          }
          
    }
}

/**
   @brief: Switch to running the main application (app).
   *This function is used to convert from the boot program (bootloader) to the 
   * main application (application).
   *The steps performed in the function include:
   * Disable interrupts by calling __disable_irq().
   *Update the stack pointer address (Stack pointer) with the value read from 
   * the flash address BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Update the vector Table Offset Register (VTOR) space with the flash address
     value BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Re-enable interrupts by calling __enable_irq().
   * Reset the main application function via the function pointer ptr_app, the 
     value of this pointer is read from the flash address BOOT_ADDRESS_RESET_HANDLER_APP.
   * This will start the main application execution.
   * This function ensures that the transition from bootloader to main application 
     is smooth and precise,ensure the transition from the boot process to the 
     main application of the system.
*/
void BOOT_APP(void)
{
      /*Disable Interrupt*/
    __disable_irq();
    /*jump app*/
    __set_MSP(DRIFLASH_ReadFlashAddress(BOOT_ADDRESS_START_STACK_POINTER_APP));
    /*Initzation start stack pointer*/
    SCB->VTOR = BOOT_ADDRESS_START_STACK_POINTER_APP;
    /*Enable Interrupt*/
    __enable_irq();    
    /*call app_reset_handler*/
    fptr = (void(*)())(DRIFLASH_ReadFlashAddress(BOOT_ADDRESS_RESET_HANDLER_APP));
    
    fptr();
}

/**
 * @func:   Application_handle_rx_data_intterupt
 * @brief:  Function to handler interupt receiver
 * @param:  u8Data : Reciver Interupt Data
 * @return: None 
 **/
void Application_handle_receiver_data_intterupt(uint8_t u8Data)
{
    MIDware_Queue_PushData(u8Data);
    
    if (u8Data == '\n')
    {
          MIDware_Queue_Enqueue();  
    }
  
}

/*******************************************************************************
 * EOF
 ******************************************************************************/