#include "MKE16Z4.h"

#define DELAY_CNT               (3000000)
#define GREEN_LED_PIN           (1 << 4)
#define RED_LED_PIN             (1 << 5)

void initLed();
void delay();

int main(void)
{
    initLed();

    while (1)
    {
        delay();
        
        /* Toggle 2 LEDs */
        FGPIOB->PTOR |= (GREEN_LED_PIN | RED_LED_PIN); 
    }
}

void initLed()
{
    /* Enable Clock for PORTB */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);
    
    /* Setup PINB4, PINB5 as GPIO */
    PORTB->PCR[4] |= PORT_PCR_MUX(1);
    PORTB->PCR[5] |= PORT_PCR_MUX(1);
    
    /* Setup PINB4 as Output Mode */
    FGPIOB->PDDR |= GREEN_LED_PIN | RED_LED_PIN;
    
    /* Set default value for PINB4 = HIGH, PINB5 = LOW */
    FGPIOB->PDOR &= ~RED_LED_PIN;
    FGPIOB->PDOR |= GREEN_LED_PIN;
}

void delay()
{
    uint32_t i;
    for (i = 0; i < DELAY_CNT; i++)
    {
        __asm("nop");
    }
}

