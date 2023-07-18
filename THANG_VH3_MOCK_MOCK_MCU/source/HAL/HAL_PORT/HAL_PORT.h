#ifndef HAL_PORT_H
#define HAL_PORT_H

#include "stdint.h"
#include "MKE16Z4.h"
#include "HAL_GPIO.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
   * @brief Lists HAL (Hardware Abstraction Layer) port error codes.
   * @details This Enum is used to represent error codes during control of HAL ports.
   * There are two defined error codes:
   * - HAL_PORT_SUCCESS: Success error code, no error occurred during HAL port control.
   * - HAL_PORT_FAIL: Failure error code, an error occurred during HAL port control.
   */
typedef enum {
      HAL_PORT_SUCCESS = 0,
      HAL_PORT_FAIL
}HAL_PORT_Erorr_E;
/**
   * @brief List the multiplexing mode options of a GPIO pin.
   * @details This Enum is used for general-purpose mode selection for a single GPIO pin.
   * There are eight options defined:
   * - Mode_Analog: Disabled GPIO pin option, used for analog purposes.
   * - Mode_Gpio: GPIO pin option is GPIO mode (common mode).
   * - Mode_Alternative_2: Alternate mode 2 option, specified by each specific chip.
   * - Mode_Alternative_3: Alternative mode 3 option, specified by each specific chip.
   * - Mode_Alternative_4: Alternate mode 4 option, specified by each specific chip.
   * - Mode_Alternative_5: Alternate mode 5 option, specified by each specific chip.
   * - Mode_Alternative_6: Alternate mode 6 option, specified by each specific chip.
   * - Mode_Alternative_7: Alternate mode option 7, specified by each specific chip.
   */
typedef enum 
{
      Mode_Analog = 0u, /*000 Pin disabled (Alternative 0) (analog)*/
      Mode_Gpio   = 1u, /*001 Alternative 1 (GPIO).*/
      Mode_Alternative_2 = 2u,/*010 Alternative 2 (chip-specific).*/
      Mode_Alternative_3 = 3u,/*011 Alternative 3 (chip-specific).*/
      Mode_Alternative_4 = 4u,/*100 Alternative 4 (chip-specific).*/
      Mode_Alternative_5 = 5u,/*101 Alternative 5 (chip-specific).*/
      Mode_Alternative_6 = 6u,/*110 Alternative 6 (chip-specific).*/
      Mode_Alternative_7 = 7u,/*111 Alternative 7 (chip-specific).*/
      
}Mux_type;
/**
   * @brief Lists the pull-enabled state options of a GPIO pin.
   * @details This Enum is used to select the pull enable state for a GPIO pin.
   * There are two options defined:
   * - PULL_DISABLE: Option to disable drag, no pull enabled on GPIO pin.
   * - PULL_ENABLE: Option to enable pull, pull is enabled on the GPIO pin.
   */
typedef enum 
{
    PULL_DISABLE = 0U,
    PULL_ENABLE  
}Pull_ENABLE_Type_E;

/**
    * @brief Lists pin GPIO pull state options.
    * @details This Enum is used to select the pull state for a GPIO pin.
    * There are two options defined:
    * - PULL_DOWN: Option to pull down, GPIO pin is logic low.
    * - PULL_UP: Option to pull up, GPIO pins to logic high.
    */
typedef enum 
{
    PULL_DOWN = 0U,
    PULL_UP   
}Pull_SELECT_Type_E;

typedef struct
{
    PORT_Type             *PORTx;                     /*Configuration for fort*/
    
    Mux_type              Select_Modemux_Port;        /*Select mode for gpio*/
    
    Pull_ENABLE_Type_E    Select_pullenable_type;     /*Select pull */
    
    Pull_SELECT_Type_E    Select_pull_type;           /*Select pull type */
    
    uint32_t               pin_select;
    
}port_config_struct_t;

/*******************************************************************************
 * API
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
HAL_PORT_Erorr_E HAL_PORT_Select_Mode(PORT_Type* PORTx,uint32_t Select_Pin,Mux_type Select_Mode_IO);
/**
   * @brief:Sets the pull-up/pull-down resistor enabled or disabled state for a GPIO pin on the GPIO port.
   * @param: PORTx: Pointer to the GPIO port to be set.
   * @param:Select_Pin: Specify the GPIO pin to set the state.
   * @param:Select_Mode_IO: State value from enum Pull_ENABLE_Type_E.
   * @retval:HAL_PORT_Erorr_E: The result of the state setting process.
   * - HAL_PORT_SUCCESS: Setup was successful, no errors occurred.
   * - HAL_PORT_FAIL: Setup failed, GPIO port not supported.
   */
HAL_PORT_Erorr_E HAL_PORT_Select_Pull_Enable(PORT_Type* PORTx,uint32_t Select_Pin,Pull_ENABLE_Type_E Select_Mode_IO);
/**
   * @brief:Sets a pull mode for a GPIO pin on the GPIO port.
   * @param:PORTx: Pointer to the GPIO port to be set.
   * @param:Select_Pin: Specifies the GPIO pin to be set to pull mode.
   * @param:Select_Mode_IO: Pull mode value from the Pull_SELECT_Type_E enum.
   * @retval:HAL_PORT_Erorr_E: Result of drag mode setup.
   * - HAL_PORT_SUCCESS: Setup was successful, no errors occurred.
   * - HAL_PORT_FAIL: Setup failed, GPIO port not supported.
   */
HAL_PORT_Erorr_E HAL_PORT_Select_Pull_Mode(PORT_Type* PORTx,uint32_t Select_Pin,Pull_SELECT_Type_E Select_Mode_IO);

void HAL_POrt_IT(void(*func)(void));
#endif /*HAL_PORT_H*/

/*******************************************************************************
 * EOF
 ******************************************************************************/