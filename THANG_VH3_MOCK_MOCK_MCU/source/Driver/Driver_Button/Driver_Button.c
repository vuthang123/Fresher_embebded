#include "Driver_Button.h"
#include "HAL_GPIO.h"
#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PCC_PORTD_CGC           (1 << 30UL)
/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Initialization for Button
  * @param PORT_Type (PORTA ,PORTB,PORTC,PORTD,PORTE)         
  * @retval 0 : SUCCESS
            1 : ERROR
  */

DRIVBUTTON_Erorr DRIVBUTTON_init(PORT_Type* PORTx)
{
    
    DRIVBUTTON_Erorr E_Retval = DRIVER_BUTTON_SUCCESS ;
    /*cHECK ENABLE CLOCK PORT  B*/
    if (HALCLOCK_Enableclock_peripheral(PCC_PORTD_INDEX,PCC_PORTD_CGC) != 0)
    {
        E_Retval = DRIVER_BUTTON_FAIL;    
    }
    /*select mode gpio */
    if(HAL_PORT_Select_Mode(PORTx,BUTTON_SW3,Mode_Gpio) != 0)
    {
        E_Retval = DRIVER_BUTTON_FAIL;    
    }
    
    /*pull enable*/
    if(HAL_PORT_Select_Pull_Enable(PORTx,BUTTON_SW3,PULL_ENABLE))
    {
        E_Retval = DRIVER_BUTTON_FAIL;    
    }
    
    /*pull up */
    if(HAL_PORT_Select_Pull_Mode(PORTx,BUTTON_SW3,PULL_UP))
    {
        E_Retval = DRIVER_BUTTON_FAIL;    
    }
    /*config input*/
    if(HAL_GPIO_IO(FGPIOD,BUTTON,HAL_GPIO_INPUT))
    {
        E_Retval = DRIVER_BUTTON_FAIL;    
    }
    
    return E_Retval;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/