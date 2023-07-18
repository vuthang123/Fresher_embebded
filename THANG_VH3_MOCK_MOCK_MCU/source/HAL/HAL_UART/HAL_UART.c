#include "HAL_UART.h"
#include "HAL_CLOCK.h"
#include "HAL_PORT.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "MKE16Z4.h"

/*******************************************************************************
 * Prototypes
******************************************************************************/
/**
   * @brief:The function pointer `HALUART_sfun_pointer` is used to point to a 
   * handler function with parameter `uint8_t` and no return value.
   * The structure of a function pointer allows it to be assigned 
   * to any function with the same signature type.
   * This provides flexibility and reusability for handling different function
   * through a single function pointer.
   */
static void (*HALUART_sfun_pointer)(uint8_t);
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
   * @brief:Selects the TX (transmit) and RX (receive) states for an LPUART register.
   * @param:reg: Pointer to LPUART register to be set.
   * @param:status: Value from enum HALUART_status to specify TX and RX status.
   * - OFF: Turn off TX and RX status.
   * - ON: Turn on TX and RX status.
   */
void HALLPUART_Select_Tx_Rx(LPUART_Type* reg,HALUART_status status)
{
    
    switch(status)
    {
          case OFF: 
        {
                                    /*Disable TX RX*/
            reg->CTRL &= ~LPUART_CTRL_TE_MASK;
            reg->CTRL &= ~LPUART_CTRL_RE_MASK;
            break;
        }
        case ON:
        {   
            reg->CTRL |= LPUART_CTRL_TE_MASK;
            reg->CTRL |= LPUART_CTRL_RE_MASK;
        
        }
    }
   
}
/**
   * @brief:Configure the number of data bits in the LPUART register based 
   *      on the UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to the UART configuration structure containing 
   * information about the number of data bits.
   */
void HALLPUART_Data_bit(LPUART_Type* reg,uart_conf_stuct_t* base)
{
    /*  Number of data bits */
    switch (base->Con_fig_bits)
    {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
          reg->CTRL |= LPUART_CTRL_M_MASK;
          reg->BAUD &= ~LPUART_BAUD_M10_MASK;
      case 8:
          reg->CTRL &= ~LPUART_CTRL_M_MASK;
          reg->CTRL &= ~LPUART_CTRL_M7_MASK;
          reg->BAUD &= ~LPUART_BAUD_M10(1);
          break;

      case 9:
          reg->CTRL |= LPUART_CTRL_M_MASK;
          reg->CTRL &= ~LPUART_CTRL_M7_MASK;
          reg->BAUD &= ~LPUART_BAUD_M10_MASK;
          break;

      case 10:
          reg->CTRL &= ~LPUART_CTRL_M7_MASK;
          reg->BAUD |=  LPUART_BAUD_M10_MASK;
          break;
    }
}

/**
   * @brief:Configure the parity type in the LPUART register based on 
   *the UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to the UART configuration structure 
   * containing information about the parity type.
   */
void HALLPUART_ParityType(LPUART_Type* reg,uart_conf_stuct_t* base)
{
    reg->CTRL |= base->parityEnable ? LPUART_CTRL_PE(1) : LPUART_CTRL_PE(0);
    reg->CTRL |= base->parityType   ? LPUART_CTRL_PT(1) : LPUART_CTRL_PT(0);

}

/**
 * @funcV   Selection one bit and two bit
 * @brief:  Function Selection bit for UART
 * @param:  base: Include Information Configuration
 * @return: None 
 **/
void HALUART_Stopbit(LPUART_Type* reg,uart_conf_stuct_t* base)
{
    /*Configure number of Stop bit BAUD[SBNS]*/
    reg->BAUD |= base->stopbit ? LPUART_BAUD_SBNS(1): \
                                                 LPUART_BAUD_SBNS(0);
                                                 
}

/**
   * @brief:Enables the receive interrupt (RX) function in the register
   * LPUART.
   * @param:reg: Pointer to LPUART register to be enabled.
   */
void HALUART_Enable_Rx_IT(LPUART_Type* reg)
{    
    reg->CTRL |= LPUART_CTRL_RIE_MASK;
    NVIC_ClearPendingIRQ(LPUART0_IRQn);
    NVIC_EnableIRQ(LPUART0_IRQn);
}

/**
   * @brief:Configure byte ordering in
   * LPUART register is based on UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to UART configuration structure containing information about
   * byte transmit/receive order.
   */
void HALUART_Sortbyte(LPUART_Type* reg,uart_conf_stuct_t* base)
{
    /* MSB or LSB first */
      reg->STAT |= base ->select_sort_byte ? LPUART_STAT_MSBF(1): \
                                                        LPUART_STAT_MSBF(0);
}
/**
   * @brief:Configure the inverting mode of data transmission and reception in the register
   * LPUART is based on state.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:status: Invert mode status, can be OFF or ON.
   */
void HALUART_Select_Inversion(LPUART_Type* reg,HALUART_status status)
{    
    switch (status)
    {
      case OFF:
            {
                reg->STAT &= ~LPUART_STAT_RXINV_MASK;
                reg->CTRL &= ~LPUART_CTRL_TXINV_MASK;
                  break;
            }
      case ON:
          {
                reg->STAT |= LPUART_STAT_RXINV(1);
                reg->CTRL |= LPUART_CTRL_TXINV(1);
                break;
          }
    }
}

/**
   * @brief:Handle the LPUART0 interrupt.
   */
void LPUART0_IRQHandler()
{
    uint8_t receiData = 0;
    
    if(LPUART0->STAT & LPUART_STAT_RDRF_MASK)
    {
        receiData =  LPUART0->DATA;
        if(HALUART_sfun_pointer!= NULL)
        {
            HALUART_sfun_pointer(receiData);
        }
    }
}
/**
   * @brief:Assigns the address of the function that processes the data received from the LPUART.
   * @param:sfun: The address of the function that handles the received data.
   */
void HALLPUART_Handle_Rx(void (*sfun)(uint8_t))
{
    HALUART_sfun_pointer = sfun;
}
/**
   * @brief:Configure LPUART baud rate.
   * @param:base: Structure containing information about the baud rate.
   */
void HALUART_Baurate(uart_conf_stuct_t * base)
{
      /*Configure the baud rate BAUD[SBR, OSR]*/    
    LPUART0->BAUD = (LPUART0->BAUD & (~LPUART_BAUD_SBR_MASK)) | LPUART_BAUD_SBR(base->Scale.minSbr);  
    LPUART0->BAUD = (LPUART0->BAUD & (~LPUART_BAUD_OSR_MASK)) | LPUART_BAUD_OSR(base->Scale.minOsr);      

}
/**
   * @brief:Pass a byte of data via LPUART0.
   * @param:u8Data: Byte of data to be transferred.
   */
void HALUART_Transmission_One_byte(uint8_t u8Data)
{
    /* Wait Data Register is Empty */
    while((LPUART0->STAT & LPUART_STAT_TDRE_MASK) == 0);
    /* Write to Data Register */
    LPUART0->DATA = u8Data;
    
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
