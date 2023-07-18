#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stdint.h"
#include "HAL_PORT.h"
#include "MKE16Z4.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum
{
    HAL_GPIO_SUCCESS = 0u,
    HAL_GPIO_FAIL
}HALGPIO_Erorr_Code;

typedef enum
{    
    HAL_GPIO_INPUT = 0U,
    HAL_GPIO_OUTPUT
}HALGPIO_SelectMode_IO;

typedef struct {
    FGPIO_Type            *FGPIOx;

     HALGPIO_SelectMode_IO Select_Mode_IO;                /*Select gpio in and output*/
    
    uint32_t               pin_select;
    
}Gpio_config_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
  * @brief: Config input or output.
  * @param: FGPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @param: Select_Mode_IO: Select mode        
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_IO(FGPIO_Type* FGPIOx,uint32_t Pin_select,HALGPIO_SelectMode_IO Select_Mode_IO);
/**
  * @brief: Reads the specified input port pin.
  * @param: FGPPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @retval:The input port pin value.
  */
uint32_t HAL_GPIO_Read_Pin(FGPIO_Type* FGPPIOx,uint32_t Pin_select);
/**
  * @brief: Write the specified input port pin.
  * @param: FGPPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to write.
  * @param: u8status: Status .
  * @retval:none
  */
void HAL_GPIO_Write_Pin(FGPIO_Type* GPIOx,FGPIO_Type* FGPIOy,uint32_t Pin_Select,uint8_t u8status);
/**
  * @brief: Setup defaut specified output pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_Setup_Defaut(FGPIO_Type* GPIOx,uint32_t Pin_select);
/**
  * @brief: Clear state specified output gpio pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.        
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_Clear_State(FGPIO_Type* FGPIOy,uint32_t Pin_select);
/**
  * @brief: Set state specified output gpio pin.
  * @param: GPIOx: where x can be (A..E depending on device used) to select the FGPIO peripheral
  * @param: Pin_select: specifies the port bit to read.        
  * @retval:0 : SUCCESS
            1 : ERROR
  */
HALGPIO_Erorr_Code HAL_GPIO_Set_State(FGPIO_Type* FGPIOx,uint32_t Pin_select);

#endif /*HAL_GPIO_H*/
/*******************************************************************************
 * EOF
 ******************************************************************************/