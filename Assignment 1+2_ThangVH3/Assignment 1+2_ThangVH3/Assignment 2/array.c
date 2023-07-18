#include <stdio.h>
#include <stdint.h>
#include "array.h"


Node nodes[ARRAY_SIZE];

Node *head = NULL;

uint8_t insertNode(uint8_t value) 
{
	uint8_t nodeCount;
	
	if (nodeCount == ARRAY_SIZE) 
	{
        printf("Can't add more, the list is full.\n");
        return 0;
    }
    
    nodes[nodeCount].data = value;
    nodes[nodeCount].next = NULL;

    if (head == NULL) 
	{
        head = &nodes[nodeCount];
    }
	else 
	{
        Node *current = head;  
        while (current->next != NULL) 
		{
            current = current->next;
        }
        current->next = &nodes[nodeCount];
    }
	nodeCount++;
    return nodeCount;
}

uint8_t isValueExist(uint8_t value, uint8_t *arr, uint8_t size) 
{
    for (int i = 0; i < size; i++) 
	{
        if (arr[i] == value) 
		{
            return 1;
        }
    }
    return 0;
}

void printArray(uint8_t *arr, int size)
{
    for (int i = 0; i < size; i++) 
	{
		if (arr[i] != 0xFF)
		{
			printf("Data arr[%d] = %d \n",i,arr[i]);
		}
        
    }
    printf("\n");
}

void delete_node(Node **head, int value) 
{
    Node *current = *head;
    /*
		Khởi tạo biến con trỏ previous là để giúp ta xác định vị trí của node.
		Nếu không có biến previous, trong trường hợp này, chúng ta không thể 
		cập nhật con trỏ head để trỏ đến node thứ hai của danh sách liên kết.
	*/
	Node *previous = NULL;
    
    while (current != NULL) 
	{ 
        if (current->data == value) 
		{
            if (previous == NULL) 
			{
                *head = current->next;
            }
			else
			{
                previous->next = current->next;
            }
            printf("Removed %d value from link list.\n", value);
            break;
        }
        previous = current;
        current = current->next;
    }
}

void selectionSort(uint8_t *arr, uint8_t n) 
{
    uint8_t temp;
    
    for (uint8_t i = 0; i < n-1; i++) 
	{
        uint8_t minIndex = i;
        for (uint8_t j = i+1; j < n; j++) 
		{
            if (arr[j] < arr[minIndex]) 
			{
                minIndex = j;
            }
        }
        
        temp =  arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
        
    }
}
