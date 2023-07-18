#include <stdio.h>
#include "Srec.h"
#include <stdint.h>

FILE* input;
FILE* output;

void Call_Handle_file_Srec()
{
	SREC_info info;
	
	input = fopen("input.txt", "r");
	if (input == NULL) 
	{
		printf("Error: could not open SREC file\n");
		return ;
	}
	output = fopen("output.txt", "w");
	if (output == NULL) 
	{
		printf("Error: could not open output file\n");
		return ;
	}
	
	Read_srec_Muilt_line(info);
	
	fclose(input);
	fclose(output);
}