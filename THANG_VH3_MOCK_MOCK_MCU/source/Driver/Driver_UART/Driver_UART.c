#include "HAL_CLOCK.h"
#include "HAL_PORT.h"
#include "HAL_UART.h" 
#include "MKE16Z4.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PCC_PORTB_CGC           (1<<30)
#define PORTB_TRANSMITER        (1U)
#define PORTB_RECEIVER          (0U)

/*******************************************************************************
 * Prototypes
******************************************************************************/

/**
 * @func    Initialization driver call back
 * @brief   Function Initialization for Uart
 * @param   None
 * @return  None 
 **/
static void (*Driver_Uart_Call_back)(uint8_t);
/**
 * @func    Enable port uart enable clock 
 * @brief   Driver uart enable clock 
 * @param   None
 * @return  None 
 **/
static void Driver_Uart_Port_Enbale_Clock();
/**
 * @func    Calculate baudrate
 * @brief   Function Baudrate for Uart
 * @param   None
 * @return  None 
 **/
static void Driver_Uart_Baudrate();
/**
 * @func    Initialization on UART
 * @brief   Function Initialization for Uart
 * @param   None
 * @return  None 
 **/
static uint32_t uart_baudrate_gen(uint32_t clock, uint16_t sbr, uint8_t osr);
/**
 * @func    Initialization on UART
 * @brief   Function Initialization for Uart
 * @param   None
 * @return  None 
 **/
static uint32_t baudrate_absolute_value(uint32_t temp_baud, uint32_t Baud);
/**
 * @func    Initialization on UART
 * @brief   Function Initialization for Uart
 * @param   None
 * @return  None 
 **/
static uint32_t uart_get_sbr(uint32_t clock,uint32_t Baud , uint8_t osr);
/**
 * @func    Initialization on UART
 * @brief   Function Initialization for Uart
 * @param   None
 * @return  None 
 **/
static void Driver_Uart_Baudrate(uart_conf_stuct_t *base);
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  *@brief Initialize the UART driver.
  *@param uart Pointer to uart_conf_stuct_t structure containing UART 
          configuration information.
  *@return None
*/
void Driver_UART_Init(uart_conf_stuct_t *uart)
{

    Driver_Uart_Port_Enbale_Clock();
    
    HALLPUART_Select_Tx_Rx(LPUART0,OFF);
    
    HALLPUART_Data_bit(LPUART0,uart);
    
    HALLPUART_ParityType(LPUART0,uart);
    
    HALUART_Sortbyte(LPUART0,uart);
    
    HALUART_Select_Inversion(LPUART0,OFF);
    
    HALUART_Stopbit(LPUART0,uart);
    
    Driver_Uart_Baudrate(uart);
    
    HALLPUART_Select_Tx_Rx(LPUART0,ON);
    
    HALUART_Enable_Rx_IT(LPUART0);     
}

/**
 * @func    Hander data for driver
 * @brief   Function Initialization for Queue and Uart
 * @param   None
 * @return  None 
 **/
void Driver_Uart_Rx(void)
{       
    HALLPUART_Handle_Rx(Driver_Uart_Call_back);
}

/**
 * @func    Transmission multi byte on app
 * @brief   Function Initialization for Queue and Uart
 * @param   u8Data
 * @param   lenght
 * @return  None 
 **/
void Driver_LPUART0_Transmission_Milt_byte(uint8_t *u8Data ,uint8_t lenght)
{
    uint8_t i =0;
    
    for(i=0;i<lenght;i++)
    {
        HALUART_Transmission_One_byte(*(u8Data+i));
    }
}
/**
 * @func    driver Ennable uart
 * @brief   Function Initialization for Queue and Uart
 * @param   None
 * @return  None 
 **/
static void Driver_Uart_Port_Enbale_Clock()
{
    /*Enable the clock for the PORT associated with the LPUART pins you want to use*/
    HALCLOCK_Enableclock_peripheral(PCC_PORTB,PCC_PORTB_CGC);
    /*Enble Transmitter felid mux in PORT */
    HAL_PORT_Select_Mode(PORTB,PORTB_TRANSMITER,Mode_Alternative_2);
    /*Enble Rceiver felid mux in PORT */
    HAL_PORT_Select_Mode(PORTB,PORTB_RECEIVER,Mode_Alternative_2);
    
    HALCLOCK_FastIRC_FIRC(SCG,PCC);
}


/**
  * @brief Set baudrate for UART.
  * @param base Pointer to uart_conf_stuct_t structure containing UART configuration information.
  * @return None
*/
static void Driver_Uart_Baudrate(uart_conf_stuct_t *base)
{
    uint8_t  osr = 0;
    uint16_t sbr = 0;
    uint32_t baud = 0;
    uint32_t Compare = 0;

    uint32_t Min_compare = 0xFFFFFFFF;

    //uint32_t minOsr_done = 0;
    
    for (osr = 4; osr<= 32; ++osr)
    {
        /*get value srb current*/
        sbr = uart_get_sbr(24000000,base->Baud_Rate, osr);
        /*get value baudrate current*/
        baud = uart_baudrate_gen(24000000, sbr, osr);
        /*Compare*/
        Compare = baudrate_absolute_value(baud, base->Baud_Rate);
        /*get value min for Osr and sbr */
        if (Compare < Min_compare)
        {
            Min_compare = Compare;
            base->Scale.minOsr = osr;
            base->Scale.minSbr = sbr;
        }
        
    }    
    base->Scale.minOsr = base->Scale.minOsr & 0xFF;
    
    HALUART_Baurate(base);
}
/**
  *@brief Calculate baudrate based on clock, sbr and osr parameters.
  *@param clock The system clock frequency.
  *@param sbr SBR value.
  *@param osr OSR value.
  *@return Calculated baudrate.
*/
static uint32_t uart_baudrate_gen(uint32_t clock, uint16_t sbr, uint8_t osr)
{
    uint32_t baudrate = 0;
    
    baudrate = ((clock)/((sbr)*((osr) + 1)));
    
    return baudrate;
}
/**
  *@brief Calculate the SBR value based on clock, Baud and osr parameters.
 * @param clock The system clock frequency.
 * @param Baud Desired baudrate rate.
 * @param osr OSR value.
 * @return The calculated SBR value.
*/
static uint32_t uart_get_sbr(uint32_t clock,uint32_t Baud , uint8_t osr)
{
    uint16_t sbr = 0;
    
    sbr = ((clock) / ((Baud)* ((osr) + 1)));
    
    return sbr;
}


/**
 * @brief Calculate the absolute value of the difference between the actual baudrate and the desired baudrate.
 * @param temp_baud Actual baudrate.
 * @param Baud Desired baudrate rate.
 * @return The absolute value of the difference.
*/
static uint32_t baudrate_absolute_value(uint32_t temp_baud, uint32_t Baud)
{
    uint32_t Compare = 0;
    int32_t abs = 0;
    abs = Baud - temp_baud;
    if(abs >= 0)
    {
        Compare = abs;
    }
    else
    {
        Compare = -abs;
    }
    
    return Compare;
}


/**
 * @brief Register a callback function to handle received data in the UART driver.
 * @param Function_call Callback function pointer to process the received data.
 * @return None
*/
void Driver_Uart_Hander_Data(void(*Function_call)(uint8_t))
{
     Driver_Uart_Call_back = Function_call;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
