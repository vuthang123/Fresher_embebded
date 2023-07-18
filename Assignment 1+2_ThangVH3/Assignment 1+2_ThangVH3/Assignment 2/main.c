#include <stdio.h>
#include <stdint.h>
#include "array.h"

int nodeCount = 0;

uint8_t assignment3[ARRAY_SIZE] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,\
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // Khởi tạo mảng gồm 20 bytes



int main() 
{
    
    uint8_t value; 
	uint8_t position;
	uint8_t choice;
    Node *head = NULL;
    int subChoice;
    
	printf("\n******************MANAGER ARRAY************************\n");
	printf("\n*1. Add value to a specified molecule of the array.    \n");
    printf("\n*2. Delete an array block value.                       \n");
    printf("\n*3. Print the value of elements entered into the array.\n");
    printf("\n*4. End program.                                       \n");
    printf("\n*******************************************************\n");
    
    while (1) 
	{
        printf("\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
	
        switch (choice)
		{
            case 1:
			{
            	printf("Enter the location to add value (0-19): ");
                scanf("%d", &position);

                
                if (position < 0 || position >= ARRAY_SIZE) 
				{
                    printf("Invalid location.\n");
                    break;
                }
                
                if (assignment3[position] != 0xFF) 
				{
				    printf("This location has been entered.\n");
				    break;
				}

                
                printf("Enter the value to add (0-100): ");
                scanf("%d", &value);
                                             
                if (value < 0 || value > 100)
				{
                    printf("Invalid value.\n");
                    break;
                }
                
                if (isValueExist(value, assignment3, ARRAY_SIZE)) 
				{
					printf("The value already exists in the array.\n");
					break;
				}
				assignment3[position] = value;
				insertNode(value);
				printf("Added value %d to position %d of array.\n", value, position);
				break;
			}     		
			case 2:
			{
				printf("Enter the location to delete the value (0-19): ");
				scanf("%d", &position);
					
				if (position < 0 || position >= ARRAY_SIZE)
				{
					printf("Invalid location.\n");
					break;
				}
					
				if (assignment3[position] == 0xFF) 
				{
					printf("This location has not been entered.\n");
					break;
				}
					
				value = assignment3[position];
				assignment3[position] = 0xFF;
				delete_node(&head,value);
				printf("Removed value %d from position %d of array.\n", value, position);
				
				break;
			}			
			case 3:
			{
				printf("1. Print the entered values of the unsorted array\n");
                printf("2. Prints the entered values of the sorted array\n");
                printf("Enter your selection(1-2): ");
                scanf("%d", &subChoice);
                
                switch (subChoice) 
				{
                    case 1:
					{
						printArray(assignment3, ARRAY_SIZE);
                        break;
					}
                        
                    case 2:
					{
						printf("\nInput of sorted array\n");
						selectionSort(assignment3,ARRAY_SIZE);
                        printArray(assignment3, ARRAY_SIZE);
                        
						break;
					}
					            
                    default:
					{
                    	
                    	printf("Invalid selection.\n");
                        break;
					}
                        
                }
                break; 	
			}
                
			case 4:
			{
				printf("End program.\n");
				return 0;
				break;
			}	
			default:
			{
				printf("Invalid selection.\n");
				break;			
			}
			
		}
	}
}
