#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Linked_list.h"

Student new_student;
int position;

void Boot_App()
{
	Node* head = NULL;
	
    int choice;
    char name[50];
	
	
	do 
	{
		Menu();
		scanf("%d", &choice);
		
		switch (choice) 
		{
			case 1:
			{
				Infor_Stusent();
				head = insertNode(head, new_student, position);
				printf("=======>>Student added successfully<<=======\n");
				break;
			}
			
			case 2:
			{
				printf("Enter position to delete: ");
				scanf("%d", &position);
				head = deleteNode(head, position);
				printf("Student deleted successfully!\n");
				break;
			}
			
			case 3:
			{
				printf("\n-----STUDENT LIST-----\n");
				printList(head);
				break;
			}

			case 4:
			{
				printf("Enter name to search: ");
				fflush(stdin);
				scanf("%4s", name);
				
				if (findNode(head,name) != NULL) 
				{
					printf("\nStudent information available \n");
				}
				else
				{
					printf("\nInvalid Student \n ");
				}
				
				break;
			}
			
			case 5:
			{
				Enter_sort();
				head = sortList(head, position);
				printList(head);
				printf("Student list sorted successfully!\n");
				
				break;
				
			}
			
			case 6:
			{
				printf("Goodbye!\n");
				
				break;
			}
			
			default:
			{
				printf("Invalid choice, please try again.\n");
				
				break;	
			}	
		}
	}while (choice != 6);
}