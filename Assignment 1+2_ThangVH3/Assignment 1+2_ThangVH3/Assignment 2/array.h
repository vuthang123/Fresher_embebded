#ifndef _ARRAY_H_
#define _ARRAY_H

#define ARRAY_SIZE 20


typedef struct node {
    uint8_t data;
    struct node *next;
} Node;


/**

	@brief Check If a given value exists in an integer array.
	@param value: The value to search for in the array.
	@param arr: Pointer to the integer array to be searched.
	@param size: The size of the array.
	Returns 1 if the value exists in the array, 0 otherwise.
*/
uint8_t isValueExist(uint8_t value, uint8_t *arr, uint8_t size);
/**

	@brief Insert a new node with the given value into a linked list.
	@param value: the value to be inserted into the linked list.
	@retval Returns node count
*/
uint8_t insertNode(uint8_t value);
/**

	@brief Print all elements in an integer array.
	@param arr: pointer to the array to be printed.
	@param size: the size of the array.
	@retval None
*/
void printArray(uint8_t *arr, int size);
/**

	@brief Delete the first node in a linked list that contains the given value.
	@param head: pointer to a pointer to the head node of the linked list.
	@param value: the value to be deleted from the linked list.
	@retval None
*/
void delete_node(Node **head, int value);
/**

	@brief Sorts an array of integers in ascending order using the selection sort algorithm.
	@param arr: Pointer to the array of integers to be sorted.
	@param n: The number of elements in the array.
*/
void selectionSort(uint8_t *arr, uint8_t n);

#endif /*_ARRAY_H_*/