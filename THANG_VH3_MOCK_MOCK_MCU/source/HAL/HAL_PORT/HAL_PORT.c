#include "HAL_PORT.h"
#include "MKE16Z4.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
   * @brief:Set multiplexing for one GPIO pin on the GPIO port.
   * @param:PORTx: Pointer to the GPIO port to be set.
   * @param:Select_Pin: Specify the GPIO pin to be set to mode.
   * @param:Select_Mode_IO: Multimode value from enum Mux_type.
   * @retval:HAL_PORT_Erorr_E: Result of mode setting.
   * - HAL_PORT_SUCCESS: Setup was successful, no errors occurred.
   * - HAL_PORT_FAIL: Setup failed, GPIO port not supported.
   */
HAL_PORT_Erorr_E HAL_PORT_Select_Mode(PORT_Type* PORTx,uint32_t Select_Pin,Mux_type Select_Mode_IO)
{
    HAL_PORT_Erorr_E retval = HAL_PORT_SUCCESS;
    
    if((PORTx == PORTA) || (PORTx == PORTB) || (PORTx == PORTC)\
          || (PORTx == PORTD) || (PORTx == PORTE))
    {
          /* Clean bitfield */
          PORTx->PCR[Select_Pin] &= ~(PORT_PCR_MUX_MASK);
          /* Set bitfield */
          PORTx->PCR[Select_Pin] |=  (PORT_PCR_MUX(Select_Mode_IO));
    
    }
    else
    {
          retval = HAL_PORT_FAIL;
    }
    
    return retval;
}
/**
   * @brief:Sets the pull-up/pull-down resistor enabled or disabled state for a GPIO pin on the GPIO port.
   * @param: PORTx: Pointer to the GPIO port to be set.
   * @param:Select_Pin: Specify the GPIO pin to set the state.
   * @param:Select_Mode_IO: State value from enum Pull_ENABLE_Type_E.
   * @retval:HAL_PORT_Erorr_E: The result of the state setting process.
   * - HAL_PORT_SUCCESS: Setup was successful, no errors occurred.
   * - HAL_PORT_FAIL: Setup failed, GPIO port not supported.
   */
HAL_PORT_Erorr_E HAL_PORT_Select_Pull_Enable(PORT_Type* PORTx,uint32_t Select_Pin,Pull_ENABLE_Type_E Select_Mode_IO)
{

    HAL_PORT_Erorr_E retval = HAL_PORT_SUCCESS;
    
    if((PORTx == PORTA) || (PORTx == PORTB) || (PORTx == PORTC)\
          || (PORTx == PORTD) || (PORTx == PORTE))
    {
          /* Clean bitfield */
          PORTx->PCR[Select_Pin] &= ~(PORT_PCR_PE_MASK);
          /* Set bitfield */
          PORTx->PCR[Select_Pin] |=  (PORT_PCR_PE(Select_Mode_IO));
    }
    
    else
    {
          retval = HAL_PORT_FAIL;
    }

    
    return retval;
}
/**
   * @brief:Sets a pull mode for a GPIO pin on the GPIO port.
   * @param:PORTx: Pointer to the GPIO port to be set.
   * @param:Select_Pin: Specifies the GPIO pin to be set to pull mode.
   * @param:Select_Mode_IO: Pull mode value from the Pull_SELECT_Type_E enum.
   * @retval:HAL_PORT_Erorr_E: Result of drag mode setup.
   * - HAL_PORT_SUCCESS: Setup was successful, no errors occurred.
   * - HAL_PORT_FAIL: Setup failed, GPIO port not supported.
   */
HAL_PORT_Erorr_E HAL_PORT_Select_Pull_Mode(PORT_Type* PORTx,uint32_t Select_Pin,Pull_SELECT_Type_E Select_Mode_IO)
{

    HAL_PORT_Erorr_E retval = HAL_PORT_SUCCESS;
    
    if((PORTx == PORTA) || (PORTx == PORTB) || (PORTx == PORTC)\
          || (PORTx == PORTD) || (PORTx == PORTE))
    {
        /* Clean bitfield */
        PORTx->PCR[Select_Pin] &= ~(PORT_PCR_PS_MASK);
        /* Set bitfield */
        PORTx->PCR[Select_Pin] |=  (PORT_PCR_PS(Select_Mode_IO));
    
    }
    else
    {
          retval = HAL_PORT_FAIL;
    }
    return retval;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/