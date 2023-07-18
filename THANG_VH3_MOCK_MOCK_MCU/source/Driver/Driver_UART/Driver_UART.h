#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#include "MKE16Z4.h"
#include "stdbool.h"
#include "HAL_UART.h" 


/*******************************************************************************
 * API
 ******************************************************************************/
/**
  *@brief Initialize the UART driver.
  *@param uart Pointer to uart_conf_stuct_t structure containing UART 
          configuration information.
  *@return None
*/
void Middleware_UART_Init();
/**
 * @brief Register a callback function to handle received data in the UART driver.
 * @param Function_call Callback function pointer to process the received data.
 * @return None
*/
void Driver_Uart_Hander_Data(void(*Function_call)(uint8_t));
/**
 * @func    Hander data for driver
 * @brief   Function Initialization for Queue and Uart
 * @param   None
 * @return  None 
 **/
void Driver_Uart_Rx();

/**
 * @func    Transmission multi byte on app
 * @brief   Function Initialization for Queue and Uart
 * @param   u8Data
 * @param   lenght
 * @return  None 
 **/
void Driver_LPUART0_Transmission_Milt_byte(uint8_t *u8Data ,uint8_t lenght);

/**
  *@brief Initialize the UART driver.
  *@param uart Pointer to uart_conf_stuct_t structure containing UART 
          configuration information.
  *@return None
*/

void Driver_UART_Init(uart_conf_stuct_t *uart);      
      
#endif /*DRIVER_UART_H*/
/*******************************************************************************
 * EOF
 ******************************************************************************/