#include "Driver_Led.h"
#include "HAL_PORT.h"
#include "HAL_GPIO.h"
#include "HAL_CLOCK.h"
#include "MKE16Z4.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
    * @brief Initialize LED devices.
    * @param PORTx Pointer to PORT x control register (e.g. PORTA, PORTB, ...)
    * @param PORTy Pointer to control register PORT y (e.g. PORTA, PORTB, ...)
    *  @param PORTz Pointer to PORT z control register (e.g. PORTA, PORTB, ...)
    * @return DRIVER_LED_Erorr Initialization state, can be DRIVER_LED_SUCCESS 
    if successful or DRIVER_LED_FAIL if error.
*/
DRIVER_LED_Erorr DRIVER_LED_init(PORT_Type* PORTx,PORT_Type* PORTy,PORT_Type* PORTz)
{
    
    DRIVER_LED_Erorr E_Retval = DRIVER_LED_SUCCESS ;
    
    /*cHECK ENABLE CLOCK PORT  B*/
    if (HALCLOCK_Enableclock_peripheral(PCC_PORTB_INDEX,PCC_PORTB_CGC) != 0)
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*select mode gpio*/
    if(HAL_PORT_Select_Mode(PORTx,LED_RED,Mode_Gpio) != 0)
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*config output*/
    if(HAL_GPIO_IO(FGPIOB,LED_RED1,HAL_GPIO_OUTPUT))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*Setup defaut*/
    if(HAL_GPIO_Setup_Defaut(FGPIOB,LED_RED1))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /************************Enable clock PORTB************************/

    /*select mode gpio*/
    if(HAL_PORT_Select_Mode(PORTy,LED_GREEN,Mode_Gpio) != 0)
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*config output*/
    if(HAL_GPIO_IO(FGPIOB,LED_GREEN1,HAL_GPIO_OUTPUT))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*Setup defaut*/
    if(HAL_GPIO_Setup_Defaut(FGPIOB,LED_GREEN1))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /***********************Enable clock PORTD********************************/
    
        /*cHECK ENABLE CLOCK PORT  D*/
    if (HALCLOCK_Enableclock_peripheral(PCC_PORTD_INDEX,PCC_PORTD_CGC_D) != 0)
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*select mode gpio*/
    if(HAL_PORT_Select_Mode(PORTz,LED_BLUE,Mode_Gpio) != 0)
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*config output*/
    if(HAL_GPIO_IO(FGPIOD,LED_BLUE1,HAL_GPIO_OUTPUT))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    /*Setup defaut*/
    if(HAL_GPIO_Setup_Defaut(FGPIOD,LED_BLUE1))
    {
        E_Retval = DRIVER_LED_FAIL;    
    }
    
    return E_Retval;

}

 /*******************************************************************************
 * EOF
 ******************************************************************************/

