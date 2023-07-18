#include "HAL_GPIO.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief: Config input or output.
  * @param: FGPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @param: Select_Mode_IO: Select mode        
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_IO(FGPIO_Type* FGPIOx,uint32_t Pin_select,HALGPIO_SelectMode_IO Select_Mode_IO)
{    
    HALGPIO_Erorr_Code E_retval = HAL_GPIO_SUCCESS;
    
    if((FGPIOx == FGPIOA) || (FGPIOx == FGPIOB) || (FGPIOx == FGPIOC)\
          || (FGPIOx == FGPIOD) || (FGPIOx == FGPIOE))
    {
         
         
         switch (Select_Mode_IO)
         {
             case HAL_GPIO_INPUT:
             {
                /*input*/
                FGPIOx->PDDR &= ~Pin_select;
                break;
             }
             case HAL_GPIO_OUTPUT:
             {         
                /*outpot*/
                FGPIOx->PDDR |= Pin_select;
                break;
             }
         }
               
    }
    else
    {
        E_retval =     HAL_GPIO_FAIL;
    }
    
    return E_retval;
    
}
 

/**
  * @brief: Reads the specified input port pin.
  * @param: FGPPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @retval:The input port pin value.
  */
uint32_t HAL_GPIO_Read_Pin(FGPIO_Type* FGPPIOx,uint32_t Pin_select)
{
    uint32_t status = 0;
    
    if ((FGPPIOx->PDIR & Pin_select) != 0)
    {
      status = 1;
    }
    else
    {
      status = 0;
    }

    return status;
    
}

/**
  * @brief: Write the specified input port pin.
  * @param: FGPPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to write.
  * @param: u8status: Status .
  * @retval:none
  */
void HAL_GPIO_Write_Pin(FGPIO_Type* GPIOx,FGPIO_Type* FGPIOy,uint32_t Pin_Select,uint8_t u8status)
{
    
    if (u8status != 0)
    {
      HAL_GPIO_Clear_State(FGPIOy,Pin_Select);
          
    }
    else
    {
      HAL_GPIO_Set_State(FGPIOy,Pin_Select);
    }
}

/**
  * @brief: Setup defaut specified output pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_Setup_Defaut(FGPIO_Type* GPIOx,uint32_t Pin_select)
{

    HALGPIO_Erorr_Code E_retval = HAL_GPIO_SUCCESS;
    if((GPIOx == FGPIOA) || (GPIOx == FGPIOB) || (GPIOx == FGPIOC)\
          || (GPIOx == FGPIOD) || (GPIOx == FGPIOE))
    {
        
        GPIOx->PDOR |= Pin_select;
    }
    else
    {
        E_retval =     HAL_GPIO_FAIL;
    }
        
    return E_retval;

}
/**
  * @brief: Set state specified output gpio pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.        
  * @retval:0 : SUCCESS
            1 : ERROR
  */

 HALGPIO_Erorr_Code HAL_GPIO_Set_State(FGPIO_Type* FGPIOx,uint32_t Pin_select)
{
    HALGPIO_Erorr_Code E_retval = HAL_GPIO_SUCCESS;
    
        if((FGPIOx == FGPIOA) || (FGPIOx == FGPIOB) || (FGPIOx == FGPIOC)\
          || (FGPIOx == FGPIOD) || (FGPIOx == FGPIOE))
    {
        
        FGPIOx->PSOR |= Pin_select;
    }
    else
    {
        E_retval =     HAL_GPIO_FAIL;
    }
    
    
    return E_retval;
    
}

/**
  * @brief: Clear state specified output gpio pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.        
  * @retval:0 : SUCCESS
            1 : ERROR
  */
 HALGPIO_Erorr_Code HAL_GPIO_Clear_State(FGPIO_Type* FGPIOy,uint32_t Pin_select)
{
    HALGPIO_Erorr_Code E_retval = HAL_GPIO_SUCCESS;
    
    if((FGPIOy == FGPIOA) || (FGPIOy == FGPIOB) || (FGPIOy == FGPIOC)\
          || (FGPIOy == FGPIOD) || (FGPIOy == FGPIOE))
    {
        FGPIOy->PCOR |= Pin_select;
    }
    else
    {
        E_retval =     HAL_GPIO_FAIL;
    }
    
    
    return E_retval;
    
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
