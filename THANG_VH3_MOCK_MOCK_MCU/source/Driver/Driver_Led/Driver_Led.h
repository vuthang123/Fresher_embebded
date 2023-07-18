#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#include "HAL_CLOCK.h"
#include "HAL_PORT.h"
#include "HAL_GPIO.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* @brief  initialization clock PORT B for led*/
#define PCC_PORTB_CGC           (1 << 30UL)

/* @brief  initialization clock PORT D */
#define PCC_PORTD_CGC_D           (1 << 30UL)

/* @brief  initialization pin PORT B for led*/
#define LED_RED1     (1<<5UL)
#define LED_GREEN1   (1<<4UL)
#define LED_BLUE1    (1<<1UL)
/* @brief  initialization pin PORT B for led*/
#define RED_LED_PIN    (5UL)
#define GREEN_LED_PIN  (4UL)
#define BLUE_LED_PIN   (1UL)
#define LED_RED        (5UL)
#define LED_GREEN      (4UL)
#define LED_BLUE       (3UL)

/* @brief  return funtion for driver led*/
typedef enum {
    DRIVER_LED_SUCCESS =0,
    DRIVER_LED_FAIL ,
}DRIVER_LED_Erorr;


/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialize LED devices.
 * @param PORTx Pointer to PORT x control register (e.g. PORTA, PORTB, ...)
 * @param PORTy Pointer to control register PORT y (e.g. PORTA, PORTB, ...)
 * @param PORTz Pointer to PORT z control register (e.g. PORTA, PORTB, ...)
 * @return DRIVER_LED_Erorr Initialization state, can be DRIVER_LED_SUCCESS if 
   successful or DRIVER_LED_FAIL if error.
*/
DRIVER_LED_Erorr DRIVER_LED_init(PORT_Type* PORTx,PORT_Type* PORTy,PORT_Type* PORTz);

#endif /*DRIVER_LED_H*/
/*******************************************************************************
 * EOF
 ******************************************************************************/