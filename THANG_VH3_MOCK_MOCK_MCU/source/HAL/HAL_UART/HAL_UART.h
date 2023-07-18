#ifndef _UART_H_
#define _UART_H_

#include "stdint.h"
#include "stdbool.h"
#include "MKE16Z4.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
   * @brief:Structure contains quantitative parameters for baud configuration
   * rate of LPUART.
   */
typedef struct {
    uint32_t minOsr;
    uint32_t minSbr;
}DRILPUART_Scale_struct_t;

/**
   * @brief:The structure contains configuration parameters for the UART.
   */
typedef struct uart_conf {
     uint8_t Con_fig_bits;      /* Number of data bits */
     bool parityEnable;         /* Allow parity */
     uint8_t parityType;        /* Parity type */
     bool select_sort_byte;     /* MSB or LSB before */
     uint32_t Baud_Rate;        /* Baud speed */
     bool select_Tooggle_bit;   /* Select the toggle bit */
     uint8_t Select_Tx_Rx;      /* Select transmit or receive or both */
     DRILPUART_Scale_struct_t Scale; /* Configure UART rate */
     bool stopbit;              /* Number of stop bits */
} uart_conf_stuct_t;
/**
   * @brief:State for UART.
   */
typedef enum {
     OFF = 0, /* Off */
     ON = 1, /* On */
} HALUART_status;
 
/*******************************************************************************
 * API
 ******************************************************************************/
/**
   * @brief:Assigns the address of the function that processes the data received from the LPUART.
   * @param:sfun: The address of the function that handles the received data.
   */
 void HALLPUART_Handle_Rx(void (*sfun)(uint8_t));
/**
   * @brief:Selects the TX (transmit) and RX (receive) states for an LPUART register.
   * @param:reg: Pointer to LPUART register to be set.
   * @param:status: Value from enum HALUART_status to specify TX and RX status.
   * - OFF: Turn off TX and RX status.
   * - ON: Turn on TX and RX status.
   */
void HALLPUART_Select_Tx_Rx(LPUART_Type* reg,HALUART_status status);

/**
   * @brief:Configure the number of data bits in the LPUART register based 
   *      on the UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to the UART configuration structure containing 
   * information about the number of data bits.
   */
void HALLPUART_Data_bit(LPUART_Type* reg,uart_conf_stuct_t* base);
/**
   * @brief:Configure the parity type in the LPUART register based on 
   *the UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to the UART configuration structure 
   * containing information about the parity type.
   */
void HALLPUART_ParityType(LPUART_Type* reg,uart_conf_stuct_t* base);
/**
 * @funcV   Selection one bit and two bit
 * @brief:  Function Selection bit for UART
 * @param:  base: Include Information Configuration
 * @return: None 
 **/
void HALUART_Stopbit(LPUART_Type* reg,uart_conf_stuct_t* base);

/**
   * @brief:Enables the receive interrupt (RX) function in the register
   * LPUART.
   * @param:reg: Pointer to LPUART register to be enabled.
   */
void HALUART_Enable_Rx_IT(LPUART_Type* reg);

/**
   * @brief:Configure byte ordering in
   * LPUART register is based on UART configuration structure.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:base: Pointer to UART configuration structure containing information about
   * byte transmit/receive order.
   */
void HALUART_Sortbyte(LPUART_Type* reg,uart_conf_stuct_t* base);

/**
   * @brief:Configure the inverting mode of data transmission and reception in the register
   * LPUART is based on state.
   * @param:reg: Pointer to LPUART register to be configured.
   * @param:status: Invert mode status, can be OFF or ON.
   */
void HALUART_Select_Inversion(LPUART_Type* reg,HALUART_status status);

/**
   * @brief:Configure LPUART baud rate.
   * @param:base: Structure containing information about the baud rate.
   */
void HALUART_Baurate(uart_conf_stuct_t * base);
/**
   * @brief:Pass a byte of data via LPUART0.
   * @param:u8Data: Byte of data to be transferred.
   */
void HALUART_Transmission_One_byte(uint8_t u8Data);

#endif /*_UART_H_*/
/*******************************************************************************
 * EOF
 ******************************************************************************/