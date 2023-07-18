#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Linked_list.h"

extern Student new_student;
extern int position;

void printStudent(Student student) 
{
	printf("\n==================================================================");
    printf("=\nID: %d ===== Name: %s ===== Account: %s ===== GPA: %.2f\n=",\
    student.ID, student.Full_name, student.Account, student.GPA);
    printf("==================================================================\n");
}

void printList(Node* head) 
{
    Node* current = head;
    while (current != NULL) 
	{
        printStudent(current->data);
        current = current->next;
    }
}

Node* insertNode(Node* head, Student student, int position) 
{
    if (position < 0) 
	{
        printf("Invalid position\n");
        return head;
    }
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = student;
    
    if (position == 0 || head == NULL) 
	{
        new_node->next = head;
        head = new_node;
    }
    else 
	{
        Node* current = head;
        int Before_position = 1;
        
        while (Before_position < position && current->next != NULL) 
		{
            current = current->next;
            Before_position++;
        }
        
        new_node->next = current->next;
        current->next = new_node;
    }   
    return head;
}

Node* deleteNode(Node* head, int position) 
{
    if (position < 0) 
	{
        printf("Invalid position\n");
        return head;
    }
    if (head == NULL) 
	{
        printf("List is empty\n");
        return head;
    }
    if (position == 0) 
	{
        Node* temp = head;
        head = head->next;
        free(temp);
    }
    else 
	{
        Node* current = head;
        int i = 1;
        while (i < position && current->next != NULL) 
		{
            current = current->next;
            i++;
        }
        if (current->next == NULL) 
		{
            printf("Invalid position\n");
            return head;
        }
        Node* temp = current->next;
        current->next = current->next->next;
        free(temp);
    }
    return head;
}

Node* findNode(Node* head, char* name) 
{
    Node* current = head;
    while (current != NULL) 
	{
        if (strcmp(current->data.Full_name, name) == 0) 
		{
            printStudent(current->data);
            break;
        }
        current = current->next;
    }
    return current;
}

Node* sortList(Node* head, int option) 
{
	Student temp_data;
	
    if (head == NULL || head->next == NULL) 
	{
        return head;
    }
    
    Node* prev = head;
    Node* current = head->next;
    
    while (current != NULL) 
	{
        Node* temp = head;
        
        while (temp != current) 
		{
            if ((option == 1 && temp->data.GPA < current->data.GPA) || \
            (option == 2 && strcmp( current->data.Full_name,temp->data.Full_name) > 0)) 
			
			{
				temp_data = current->data;
                current->data = temp->data;
                temp->data = temp_data;
            }
            
            temp = temp->next;   
        }
        
        current = current->next;
    }
    
    return head;
    
}

void Infor_Stusent()
{
	printf("Enter student ID: ");
	fflush(stdin);
	scanf("%d", &new_student.ID);
	printf("Enter student name: ");
	fflush(stdin);
	scanf("%s", new_student.Full_name);
	printf("Enter student account: ");
	fflush(stdin);
	scanf("%s", new_student.Account);
	printf("Enter student GPA: ");
	fflush(stdin);
	scanf("%f", &new_student.GPA);
	printf("Enter position to insert: ");
	fflush(stdin);
	scanf("%d", &position);
}
void Menu()
{
	printf("\n==============MENU=======================\n");
	printf("1. Add student\n");
	printf("2. Delete student\n");
	printf("3. Print list\n");
	printf("4. Search student by name\n");
	printf("5. Sort students\n");
	printf("6. Quit\n");
	printf("Enter your choice: ");
}
void Enter_sort()
{
	printf("Sort by:\n");
	printf("1. GPA\n");
	printf("2. Name\n");
	printf("Enter your choice: ");
	fflush(stdin);
	scanf("%d", &position);
}