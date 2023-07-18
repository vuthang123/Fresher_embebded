#include <stdint.h>
#include "MIDware_SREC.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MIDWARE_NEWLINE      (13u)
#define START                (1U)
/*******************************************************************************
 * Prototypes
******************************************************************************/
/**
  @func : Characters_to_Hex
  @brief:Convert a character to its hexadecimal value.
  @param:character: The character to be converted.
  @return:The hexadecimal value of the character.
*/
static uint8_t Characters_to_Hex(const uint8_t character);
/**
 * @func:   Get_Srec_type
 * @brief:  Parse S-record type and extract relevant information.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status of the S-record.
 */
static Middleware_Srec_Pasre_status Get_Srec_type(uint8_t * u8Input_data,\
                        Middleware_Srec_Pasre_data_line_struct_t *Out_data);
/**
 * @func:   Get_Address
 * @brief:  Extract the address from the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status of the address extraction.
 */
static Middleware_Srec_Pasre_status Get_Address(uint8_t* u8Input_data, \
                     Middleware_Srec_Pasre_data_line_struct_t *Out_data);

/**
 * @func:   Check_Length
 * @brief:  Check the length of the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the length check.
 */
static Middleware_Srec_Pasre_status Check_Lenght(uint8_t* u8Input_data,\
                       Middleware_Srec_Pasre_data_line_struct_t *Out_data);
/**
 * @func :   Get_Data
 * @brief:  Get the data from the S-record.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the data retrieval.
 */
static Middleware_Srec_Pasre_status Get_data(uint8_t* u8Input_data,\
            Middleware_Srec_Pasre_data_line_struct_t *Out_data);
/**
 * @func:   Check_Sum
 * @brief:  Check the checksum of the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the checksum verification.
 */
static Middleware_Srec_Pasre_status Check_sum(uint8_t *u8Input_data,\
                       Middleware_Srec_Pasre_data_line_struct_t *Out_data);

/*******************************************************************************
 * Variable
 ******************************************************************************/
/**
  @brief Used to count the number of errors encountered during string processing.
*/
static uint32_t u8Counter_Error_String = 0;
/**
  @brief Used as an offset value during string parsing.
*/
static uint16_t u16Offset = 4; 
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  @func : Characters_to_Hex
  @brief:Convert a character to its hexadecimal value.
  @param:character: The character to be converted.
  @return:The hexadecimal value of the character.
*/
static uint8_t Characters_to_Hex(const uint8_t character)
{
    uint8_t u8Flag = 0xFF;

    if(('0' <= character)&&(character <= '9'))
    {
        u8Flag = character - '0';
    }
    else if('A'<=character && character <='F')
    {
        u8Flag = character - 'A' +10;
    }
    else
    {
        u8Counter_Error_String ++;
    }

    return u8Flag;
}
/**
 * @func:   Get_Srec_type
 * @brief:  Parse S-record type and extract relevant information.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status of the S-record.
 */
static Middleware_Srec_Pasre_status Get_Srec_type(uint8_t * u8Input_data,\
                        Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{
    Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
    
    uint8_t Srec =0;
    
    if('S' == u8Input_data[0])
    {
        Srec   = Characters_to_Hex(u8Input_data[1]);
        
        Out_data->u8Srec_type =(SREC_type)Srec;
        
        Out_data->u8Byte_Count = (Characters_to_Hex(u8Input_data[2]) << 4) | \
                                     (Characters_to_Hex(u8Input_data[3]));
        
        Out_data->u8Check_sum = Out_data->u8Byte_Count ;
        
        switch(Out_data->u8Srec_type)
        {
            case S0_RECORD:
            {
                parse_status = MIDDLEWARE_SREC_PASRE_STATE_START;
                Out_data->u8Byte_Address = 2U;
                break;
            }
            case S1_RECORD:
            case S2_RECORD:
            case S3_RECORD:
            {
                 
                parse_status = MIDDLEWARE_SREC_PASRE_STATE_HANDLE;
                Out_data->u8Byte_Address = Out_data->u8Srec_type + 1;
                break;
            }
            case S4_RECORD:
            case S5_RECORD:
            case S6_RECORD:
            {
                  parse_status = MIDDLEWARE_SREC_PASRE_STATE_FAIL;
                  break;
            }
            case S7_RECORD:
            case S8_RECORD:
            case S9_RECORD:
            {
                
                parse_status = MIDDLEWARE_SREC_PASRE_STATE_COMPLETE;
                Out_data->u8Byte_Address = 11- Out_data->u8Srec_type;
                break;
            }
            
            default:
            {
                parse_status = MIDDLEWARE_SREC_PASRE_STATE_FAIL;
                break;
            }
        }
    }
        
   return parse_status;
}
/**
 * @func:   Get_Address
 * @brief:  Extract the address from the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status of the address extraction.
 */
static Middleware_Srec_Pasre_status Get_Address(uint8_t* u8Input_data, \
                     Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{   
    Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
    
    
    uint8_t  u8Buffer = 0;
    uint8_t  u8Index = 0;
    uint32_t u32Address = 0;
    
    for(u8Index =0; u8Index < Out_data->u8Byte_Address; u8Index++)
    {
       
       u8Buffer = (Characters_to_Hex(u8Input_data[u16Offset]) << 4) | \
                  (Characters_to_Hex(u8Input_data[u16Offset+1]));
       
       u32Address = u32Address << 8 | u8Buffer;
       
       Out_data->u8Check_sum += u8Buffer;
       
       u16Offset += 2;
    }
    
    Out_data->u32Address = u32Address;
    
    return parse_status;
}

/**
 * @func:   Check_Length
 * @brief:  Check the length of the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the length check.
 */
static Middleware_Srec_Pasre_status Check_Lenght(uint8_t* u8Input_data,\
                                Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{
   uint8_t u8Count = 0;
   
   Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
   
   while(u8Input_data[u8Count] != '\r')
   {
      u8Count++;
   }
   
   u8Count = u8Count - 4;
            
   if((Out_data->u8Byte_Count << 1) != u8Count )
   {
       parse_status = MIDDLEWARE_SREC_PASRE_STATE_FAIL;
   }
   
   return parse_status;
}
/**
 * @func :   Get_Data
 * @brief:  Get the data from the S-record.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the data retrieval.
 */
static Middleware_Srec_Pasre_status Get_data(uint8_t* u8Input_data,\
            Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{
   Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
   
   uint8_t  u8Index = 0;
   
   uint8_t  u8Buffer = 0;
   
   Out_data->u32Data_length = Out_data->u8Byte_Count - 1- Out_data->u8Byte_Address;
        
   for(u8Index =0; u8Index < (Out_data->u32Data_length); u8Index++)
   {
      u8Buffer = (Characters_to_Hex(u8Input_data[u16Offset]) << 4) | \
                 (Characters_to_Hex(u8Input_data[u16Offset+1]));
      
      Out_data->u8Data[u8Index] = u8Buffer;
      
      Out_data->u8Check_sum += u8Buffer;
      
      u16Offset += 2;
   }
   
 return parse_status;
}
/**
 * @func:   Check_Sum
 * @brief:  Check the checksum of the S-record data.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure containing the parsed data.
 * @return: The parse status of the checksum verification.
 */
static Middleware_Srec_Pasre_status Check_sum(uint8_t *u8Input_data,\
                       Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{
    Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
    
    Out_data->u8Check_sum += (Characters_to_Hex(u8Input_data[u16Offset]) << 4) | \
                                 (Characters_to_Hex(u8Input_data[u16Offset+1]));
    
    return parse_status;
}

/**
 * @func    Middleware_SREC_Parse
 * @brief:  Parse the S-record data and extract relevant information.
 * @param:  u8Input_data: Pointer to the input S-record data.
 * @param:  Out_data: Pointer to the structure to store the parsed data.
 * @return: The parse status indicating the success or failure of the parsing operation.
 */
Middleware_Srec_Pasre_status Middleware_SREC_Parse(uint8_t *u8Input_data,\
                       Middleware_Srec_Pasre_data_line_struct_t *Out_data)
{
    Middleware_Srec_Pasre_status parse_status = MIDDLEWARE_SREC_PASRE_STATE_UNKNOWN;
               
    parse_status = Get_Srec_type(u8Input_data,Out_data);
    
    if(parse_status != MIDDLEWARE_SREC_PASRE_STATE_FAIL)
    {
        Get_Address(u8Input_data,Out_data);
            
        Check_Lenght(u8Input_data,Out_data);

        Get_data(u8Input_data,Out_data);
        
        Check_sum(u8Input_data,Out_data);
        
        u16Offset  = 4;
        
       if((0xFF != Out_data->u8Check_sum) || (0 != u8Counter_Error_String))
       {
          parse_status = MIDDLEWARE_SREC_PASRE_STATE_FAIL;
       }
    }
    
    else
    {
        parse_status = MIDDLEWARE_SREC_PASRE_STATE_FAIL;
    }

    return parse_status;
}

/**
 * @func: Check_length_str
 * @brief:Check the length of a null-terminated string.
 * @param:str: Pointer to the input string.
 * @return:The length of the string.
*/
uint16_t Check_length_str(uint8_t *str)
{
    uint16_t length = START;
    while(MIDWARE_NEWLINE != str[length])
    {
        length++;
    }
    return length;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
