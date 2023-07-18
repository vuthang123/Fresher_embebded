#ifndef DRIVER_BUTTON_H
#define DRIVER_BUTTON_H
#include "stdbool.h"
#include "HAL_CLOCK.h"
#include "HAL_GPIO.h"
#include "HAL_PORT.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief  initialization clock PORT D */
#define PCC_PORTD_CGC           (1 << 30UL)
/* @brief  initialization pin PORT D for button*/
#define BUTTON_SW3              (2UL)
/* @brief  initialization pin PORT D for button*/
#define BUTTON                  (1 << 2UL)
/* @brief  return funtion for driver button*/
typedef enum 
{
    DRIVER_BUTTON_SUCCESS =0,
    DRIVER_BUTTON_FAIL 
      
}DRIVBUTTON_Erorr;

/*******************************************************************************
 * API
 ******************************************************************************/
void DRIVE_BUUTON(void);
/**
  * @brief  Initialization for Button
  * @param PORT_Type (PORTA ,PORTB,PORTC,PORTD,PORTE)         
  * @retval 0 : SUCCESS
            1 : ERROR
  */
DRIVBUTTON_Erorr DRIVBUTTON_init(PORT_Type* PORTx);

#endif /*DRIVER_BUTTON_H*/
/*******************************************************************************
 * EOF
 ******************************************************************************/