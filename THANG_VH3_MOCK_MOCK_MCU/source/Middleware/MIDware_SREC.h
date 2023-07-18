#ifndef Middleware_SREC_H_
#define Middleware_SREC_H_

#include "stdint.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
  *@brief:Defines a maximum value for the data length.
  *This is a macro that defines the maximum value for the length of the data used 
  *in the structures and functions related to data management.
  *The maximum value is defined as 255U.
*/
#define MAX_DATA_LENGTH                   (255U)

/**
    *@brief:List the values representing SREC (Motorola S-record) types.
    *This is an enum that defines values representing SREC types used in
    *SREC data analysis.
    *The enum values include:
    *S0_RECORD: Represents the type S0_RECORD.
    *S1_RECORD: Represents the type S1_RECORD.
    *S2_RECORD: Represents the type S2_RECORD.
    *S3_RECORD: Represents the S3_RECORD type.
    *S4_RECORD: Represents the S4_RECORD type.
    *S5_RECORD: Represents the S5_RECORD type.
    *S6_RECORD: Represents the S6_RECORD type.
    *S7_RECORD: Represents the S7_RECORD type.
    *S8_RECORD: Represents the S8_RECORD type.
    *S9_RECORD: Represents the S9_RECORD type.
    *S_UNKNOWN: Represents an unknown type.
*/
typedef enum srec_type {
    S0_RECORD = 0,
    S1_RECORD,
    S2_RECORD,
    S3_RECORD,
    S4_RECORD,
    S5_RECORD,
    S6_RECORD,
    S7_RECORD,
    S8_RECORD,
    S9_RECORD,
    S_UNKNOWN= 0xFF
} SREC_type;
/**
  *@brief:Data structure for SREC parsing in middleware.
  *This structure contains the data fields involved in the analysis of an SREC stream during
    handled in middleware.
*/
typedef struct
{
    
    uint8_t  u8Srec_type; 
    
    uint8_t  u8Byte_Count;
          
    uint32_t u32Address;
    
    uint8_t  u8Byte_Address;
    
    uint8_t  u8Data[MAX_DATA_LENGTH];
    
    uint32_t u32Data_length;
    
    uint8_t  u8Check_sum;

    
}Middleware_Srec_Pasre_data_line_struct_t;

/**
  @brief:The state of the SREC parsing process in the middleware.
*/
typedef enum
{
    MIDDLEWARE_SREC_PASRE_STATE_START          = 0U,
    MIDDLEWARE_SREC_PASRE_STATE_HANDLE         = 1U,
    MIDDLEWARE_SREC_PASRE_STATE_COMPLETE       = 2U,
    MIDDLEWARE_SREC_PASRE_STATE_FAIL           = 3U,
    MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN        = 0xFF,
}Middleware_Srec_Pasre_status;

/**
  @brief:The file type during SREC parsing in the middleware.
*/
typedef enum
{
    SREC_FILE    = 1,
    UNKNOWN_FILE = 0xFF
}Middleware_Srec_Pasre_Filetype;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * @func    Middleware_SREC_Parse
 * @brief:  Parse the S-record data and extract relevant information.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status indicating the success or failure of the parsing operation.
 */
Middleware_Srec_Pasre_status Middleware_SREC_Parse(uint8_t * input_data,Middleware_Srec_Pasre_data_line_struct_t *out_data);
/**
 * @func: Check_length_str
 * @brief:Check the length of a null-terminated string.
 * @param:str: Pointer to the input string.
 * @return:The length of the string.
*/
uint16_t Check_length_str(uint8_t *str);

#endif /*Middleware_SREC_H_*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
