#ifndef Middleware_QUEUE_H_
#define Middleware_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
   * @brief:4 line queue
   */
#define MAX_LINE         4UL
/**
   * @brief:1 line queue have 255 elements
   */
#define MAX_CAPACITY     255UL

/**
  * queue: A two-dimensional array used to store elements in the ring queue. 
  * The queue array has size MAX_LINE rows and MAX_CAPACITY columns, limiting
  * the number of elements that can be contained in the queue.
  *
  * front: The index of the first element in the queue.
  * rear: The index of the last element in the queue.
  * size: The number of elements currently in the queue.
  */
typedef struct CircularQueue 
{
    uint8_t queue[MAX_LINE][MAX_CAPACITY];
    uint8_t front;
    uint8_t rear;
    uint8_t size;
    
}CircularQueue_struct_t;
/*******************************************************************************
 * API
 ******************************************************************************/
/**
   * @brief:Initialize queue in middleware.
   */
void MIDware_QueueInit();
/**
   * @brief:Check if the queue is in middleware is empty or not.
   * @retval:true if queue is empty, false
   * if the queue is not empty.
   */
bool MIDware_Queue_Empty();
/**
   * @brief:Checks if the queue in the middleware is full.
   * @retval:true if the queue is full, false if the queue is not full.
   */
bool MIDware_Queue_Full();
/**
   * @brief:Push data to queue in middleware.
   * @param:data Data to be pushed to the queue.
   */
void MIDware_Queue_PushData(uint8_t Data);
/**
   * @brief:Add a new element to the queue in the middleware.
   */
void MIDware_Queue_Enqueue(void);
/**
   * @brief:Returns the pointer to the data at the first element of the queue in the middleware.
   * @return:Pointer to the data at the first element of the queue.
   */

uint8_t *MIDware_Queue_Popdata();
/**
   * @brief:Removes the first element from the queue in the middleware.
   */
void MIDware_Queue_Dequeue();

#endif /*Middleware_QUEUE_H_*/
/*******************************************************************************
 * EOF
 ******************************************************************************/