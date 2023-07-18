#ifndef BOOT_APP_H_
#define BOOT_APP_H_

/*******************************************************************************
 * API
 ******************************************************************************/
/**
    @brief: The BOOT_LOADER function runs the process of loading the program 
    from the UART interface into Flash memory.
    @param: No parameters.
    @return: No return value.
*/
void BOOT_LOADER(void);

/**
   @brief: Switch to running the main application (app).
   *This function is used to convert from the boot program (bootloader) to the 
   * main application (application).
   *The steps performed in the function include:
   * Disable interrupts by calling __disable_irq().
   *Update the stack pointer address (Stack pointer) with the value read from 
   * the flash address BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Update the vector Table Offset Register (VTOR) space with the flash address
     value BOOT_ADDRESS_START_STACK_POINTER_APP.
   *Re-enable interrupts by calling __enable_irq().
   * Reset the main application function via the function pointer ptr_app, the 
     value of this pointer is read from the flash address BOOT_ADDRESS_RESET_HANDLER_APP.
   * This will start the main application execution.
   * This function ensures that the transition from bootloader to main application 
     is smooth and precise,ensure the transition from the boot process to the 
     main application of the system.
*/
void BOOT_APP(void);

#endif /*BOOT_APP_H_*/

/*******************************************************************************
 * EOF
 ******************************************************************************/