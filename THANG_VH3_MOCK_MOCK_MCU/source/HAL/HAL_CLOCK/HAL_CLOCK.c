#include "HAL_CLOCK.h"

#include "MKE16Z4.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  * @brief: Initialization Clock port  
  * @param: Clock_peripheral  to select reg Initialition 
  * @param: Select: specifies pin select
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALCLOCK_Status HALCLOCK_Enableclock_peripheral(Clock_peripheral_e Clock_peripheral,\
                                                              uint32_t Select)
{
      PCC_Type *Types = PCC;
      HALCLOCK_Status ret = HAL_SUCCESS;
      
      switch(Clock_peripheral)
      {
              case PCC_FLASH :
            {    
                 Types->CLKCFG[PCC_FLASH] &= ~(Select);
                 Types->CLKCFG[PCC_FLASH] |=  (Select);
                 break;
            }
              case PCC_MSCAN0 :
                        {    
                 Types->CLKCFG[PCC_MSCAN0] &= ~(Select);
                 Types->CLKCFG[PCC_MSCAN0] |=  (Select);
                 break;
            }
            case PCC_LPSPI0 :
                        {    
                 Types->CLKCFG[PCC_LPSPI0] &= ~(Select);
                 Types->CLKCFG[PCC_LPSPI0] |=  (Select);
                 break;
            }
            case PCC_CRC :
                              {    
                 Types->CLKCFG[PCC_CRC] &= ~(Select);
                 Types->CLKCFG[PCC_CRC] |=  (Select);
                 break;
            }
            case PCC_PDB0 :
            {    
                 Types->CLKCFG[PCC_PDB0] &= ~(Select);
                 Types->CLKCFG[PCC_PDB0] |=  (Select);
                 break;
            }
            case PCC_LPIT0 :
                        {    
                 Types->CLKCFG[PCC_LPIT0] &= ~(Select);
                 Types->CLKCFG[PCC_LPIT0] |=  (Select);
                 break;
            }
            case PCC_FLEXTMR0 :
                              {    
                 Types->CLKCFG[PCC_FLEXTMR0] &= ~(Select);
                 Types->CLKCFG[PCC_FLEXTMR0] |=  (Select);
                 break;
            }
            case PCC_FLEXTMR1 :
                              {    
                 Types->CLKCFG[PCC_FLEXTMR1] &= ~(Select);
                 Types->CLKCFG[PCC_FLEXTMR1] |=  (Select);
                 break;
            }
                  
            case PCC_ADC0 :
                              {    
                 Types->CLKCFG[PCC_ADC0] &= ~(Select);
                 Types->CLKCFG[PCC_ADC0] |=  (Select);
                 break;
            }
            case PCC_RTC :
                              {    
                 Types->CLKCFG[PCC_RTC] &= ~(Select);
                 Types->CLKCFG[PCC_RTC] |=  (Select);
                 break;
            }
            case PCC_LPTMR0 :
                              {    
                 Types->CLKCFG[PCC_LPTMR0] &= ~(Select);
                 Types->CLKCFG[PCC_LPTMR0] |=  (Select);
                 break;
            }
            case PCC_TSI :
                              {    
                 Types->CLKCFG[PCC_TSI] &= ~(Select);
                 Types->CLKCFG[PCC_TSI] |=  (Select);
                 break;
            }
            case PCC_PORTA :
                            {    
                 Types->CLKCFG[PCC_PORTA] &= ~(Select);
                 Types->CLKCFG[PCC_PORTA] |=  (Select);
                 break;
            }  
            case PCC_PORTB :
                              {    
                 Types->CLKCFG[PCC_PORTB] &= ~(Select);
                 Types->CLKCFG[PCC_PORTB] |=  (Select);
                 break;
            }
            case PCC_PORTC :
                              {    
                 Types->CLKCFG[PCC_PORTC] &= ~(Select);
                 Types->CLKCFG[PCC_PORTC] |=  (Select);
                 break;
            }
            case PCC_PORTD :
                             {    
                 Types->CLKCFG[PCC_PORTD] &= ~(Select);
                 Types->CLKCFG[PCC_PORTD] |=  (Select);
                 break;
            } 
            case PCC_PORTE :
                             {    
                 Types->CLKCFG[PCC_PORTE] &= ~(Select);
                 Types->CLKCFG[PCC_PORTE] |=  (Select);
                 break;
            } 
            case PCC_PWT :
                             {    
                 Types->CLKCFG[PCC_PWT] &= ~(Select);
                 Types->CLKCFG[PCC_PWT] |=  (Select);
                 break;
            } 
            case PCC_EWM :
                             {    
                 Types->CLKCFG[PCC_EWM] &= ~(Select);
                 Types->CLKCFG[PCC_EWM] |=  (Select);
                 break;
            } 
            case PCC_LPI2C0 :
                              {    
                 Types->CLKCFG[PCC_LPI2C0] &= ~(Select);
                 Types->CLKCFG[PCC_LPI2C0] |=  (Select);
                 break;
            }
            case PCC_LPUART0 :
                              {    
                 Types->CLKCFG[PCC_LPUART0] &= ~(Select);
                 Types->CLKCFG[PCC_LPUART0] |=  (Select);
                 break;
            }
            case PCC_LPUART1 :
                              {    
                 Types->CLKCFG[PCC_LPUART1] &= ~(Select);
                 Types->CLKCFG[PCC_LPUART1] |=  (Select);
                 break;
            }
            case PCC_LPUART2 :
                              {    
                 Types->CLKCFG[PCC_LPUART2] &= ~(Select);
                 Types->CLKCFG[PCC_LPUART2] |=  (Select);
                 break;
            }
            case PCC_CMP0 :      
                       {    
                 Types->CLKCFG[PCC_CMP0] &= ~(Select);
                 Types->CLKCFG[PCC_CMP0] |=  (Select);
                 break;
            }
      
      }

    return ret;
}

/**
 * @func    Configuaration Source Clock
 * @brief:  Function Initialization for Configuaration Source Clock Uart
 * @param:  scg_reg Source Clock system 
            pcc_reg Soucr for peripheral
 * @return: None 
 **/
void HALCLOCK_FastIRC_FIRC(SCG_Type* scg_reg  ,PCC_Type* pcc_reg)
{
      /*Configure the divider and enable the clock source to be used for the LPUART*/
      scg_reg->FIRCDIV |=  SCG_FIRCDIV_FIRCDIV2(2);
      
     /*Select clock source and enable clock for LPUART module*/
      pcc_reg->CLKCFG[PCC_LPUART0_INDEX] &=~PCC_CLKCFG_PCS_MASK;
      pcc_reg->CLKCFG[PCC_LPUART0_INDEX] &=~PCC_CLKCFG_CGC_MASK;
      
      pcc_reg->CLKCFG[PCC_LPUART0_INDEX] |= PCC_CLKCFG_PCS(3)|PCC_CLKCFG_CGC(1);
      
}
/*******************************************************************************
 * EOF
 ******************************************************************************/