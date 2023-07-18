#include<stdio.h>
#include "Srec.h"

#define SUCCESS 1
#define FAIL    0
#define STR_TO_HEX(STR) ((char_to_hex((STR)[0]) << 4) | char_to_hex((STR)[1]))

extern	FILE* input;
extern	FILE* output;

unsigned char char_to_hex(char c)
{
	unsigned char result = 0;
	if (c >= '0' && c <= '9')
	{
		result = c - '0';
	}
	else if(c>= 'A'&&c<='F')
	{
		result = c - 'A' + 10;
	}
	else if(c>= 'a'&&c<='f')
	{
		result = c - 'a' + 10;
	}
	return result;
}

int read_srec_line(FILE* input, SREC_info* info)
{
    static char  Line[512] = {0};
    char* Local_Data;
    int   index = 0;
    info->checksum = 0;
    
    if (fgets(Line, sizeof(Line), input) == NULL) 
	{
        return FAIL;
    }
    // Check the start character
    if (Line[0] != 'S') 
	{
        printf("Invalid start character '%c'\n", Line[0]);
        return FAIL;
    }
    // Parse the record type and byte count
    info->type = (SREC_type) char_to_hex(Line[1]);
    info->byte_count = STR_TO_HEX(&Line[2]);
	// Parse the address field based on the record type
	switch (info->type) 
	{
		case S0_RECORD:	
		case S1_RECORD:
			info->address = (STR_TO_HEX(&Line[4]) << 8) | STR_TO_HEX(&Line[6]);
			break;
		case S2_RECORD:
			info->address = (STR_TO_HEX(&Line[4]) << 16) | (STR_TO_HEX(&Line[6])\
			 << 8) | STR_TO_HEX(&Line[8]);
			break;
		case S5_RECORD:
			info->address = (STR_TO_HEX(&Line[4]) << 8) | STR_TO_HEX(&Line[6]);
			break;
		case S3_RECORD:
		case S7_RECORD:
			info->address = (STR_TO_HEX(&Line[4]) << 24) | (STR_TO_HEX(&Line[6])\
			 << 16) | (STR_TO_HEX(&Line[8]) << 8) | STR_TO_HEX(&Line[10]);
			break;	
		default:
			printf("Invalid record type: %d\n", info->type);
			return FAIL;
	}	
    // Compute the checksum over the byte count, address, and data fields
	info->checksum += (info->byte_count + STR_TO_HEX(&Line[4]) + STR_TO_HEX(&Line[6]));
	
	if (info->byte_count == 0x0D)
	{
		for (index = 0, Local_Data = &Line[12]; index < info->byte_count-4; \
		index++ , Local_Data += 2) 
		{
			
			info->data[index] = STR_TO_HEX(Local_Data);			        
			info->checksum += info->data[index];
	        
	    }
	    info->checksum = (~info->checksum)&0xFF ;
	    
	}
	else
	{
		for (index = 0, Local_Data = &Line[8]; index < info->byte_count-3; \
	    index++ , Local_Data += 2) 
		{
		
			info->data[index] = STR_TO_HEX(Local_Data);  
			info->checksum += info->data[index];
        
    	}
    	info->checksum = (~info->checksum)&0xFF ;
	}  	
	return SUCCESS;
}

void Read_srec_Muilt_line(SREC_info info)
{
	int Read_line_count = 0;
	int Wrote_line_count = 0;
	int Ret_val = 0;
	// Read each line from the SREC file and parse its contents
	while (read_srec_line(input,&info)) 
	{
		Read_line_count++;
		// Print the line data if it is not an S0 record
		switch(info.type)
		{
			case S0_RECORD:
			{
				Wrote_line_count++;
				fprintf(output, "Srec type = S%d ", info.type);
				fprintf(output, "Byte count = %02X ", info.byte_count);
				fprintf(output, "Address= %04X ", info.address);
				fprintf(output, "    Data = [");
				
				for (int i = 0; i < info.byte_count-3; i++) 
				{
					fprintf(output, "%02X", info.data[i]);
				}
				fprintf(output, "]");
				fprintf(output, "             Check_sum = %02X", info.checksum);
				fprintf(output, "\n");
			}
			break;
			
			case  S3_RECORD: 
			{
				Wrote_line_count++;
				fprintf(output, "Srec type = S%d ", info.type);
				fprintf(output, "Byte count = %02X ", info.byte_count);
				fprintf(output, "Address= %08X ", info.address);
				fprintf(output, "Data = [");							
				if(info.byte_count != 0x0D)
				{		
					for (int i = 12; i < info.byte_count - 4; i++) 
					{
						
						fprintf(output, "%02X", info.data[i]);
					}
					fprintf(output, "]");
					fprintf(output, " Check_sum = %02X", info.checksum);
					fprintf(output, "\n");				
				}	
				if(info.byte_count == 0x0D)
				{
					for (int i = 0; i < info.byte_count - 5; i++) 
					{
						fprintf(output, "%02X", info.data[i]);
					}
					fprintf(output, "]");
					fprintf(output, " Check_sum = %02X", info.checksum);
					fprintf(output, "\n");
				}
		
			}
			break;
			case  S5_RECORD:{
				Wrote_line_count++;
				fprintf(output, "Srec type = S%d ", info.type);
				fprintf(output, "Byte count = %02X ", info.byte_count);
				fprintf(output, "Address= %08X ", info.address);
				fprintf(output, "Data = [");
				fprintf(output,"S5_RECORD NO DATA");
				fprintf(output, "]");
				fprintf(output, " Check_sum = %02X", info.checksum);
				fprintf(output, "\n");
				break;
			}
			case  S7_RECORD:{
				Wrote_line_count++;
				fprintf(output, "Srec type = S%d ", info.type);
				fprintf(output, "Byte count = %02X ", info.byte_count);
				fprintf(output, "Address= %08X ", info.address);
				fprintf(output, "Data = [");
				fprintf(output,"S7_RECORD NO DATA");
				fprintf(output, "]");
				fprintf(output, " Check_sum = %02X", info.checksum);
				fprintf(output, "\n");
				break;
			}
				
		}	
	}
	printf("=======>>>>> Read sucssess from SREC file\n");
	printf("=======>>>>> Into in file output.txt \n");
	printf("\nRead %d lines from SREC file\n", Read_line_count);
	if (Wrote_line_count > 0) 
	{
		printf("Wrote %d lines of data to output file\n", Wrote_line_count);
	} 
	else 
	{
		printf("No valid data lines found in SREC file\n");
	}
}

