#include "MIDware_QUEUE.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*******************************************************************************
 * Variable
 ******************************************************************************/
/**
   * @brief The variable q is a struct of type CircularQueue_struct_t, 
             representing the circular queue in the middleware.
   */
static CircularQueue_struct_t q;
/**
   * @brief The index variable is an 8-bit integer variable, used to index the 
            current position in a queue in the queue array..
   */
static uint8_t index = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/**
   * @brief:Initialize queue in middleware.
   */
void MIDware_QueueInit() 
{
    q.front = 0;
    q.rear = 0;
    q.size = 0;
}
/**
   * @brief:Check if the queue is in middleware is empty or not.
   * @retval:true if queue is empty, false
   * if the queue is not empty.
   */
bool MIDware_Queue_Empty() 
{
    uint8_t flag = false;
    if(q.front == q.rear && q.size == 0)
    {
        flag = true;
    }
    return flag;
}
/**
   * @brief:Checks if the queue in the middleware is full.
   * @retval:true if the queue is full, false if the queue is not full.
   */
bool MIDware_Queue_Full() 
{
    uint8_t flag = false;
    if(q.front == q.rear && q.size == MAX_LINE)
    {
        flag = true;
    }
    return flag;
}
/**
   * @brief:Push data to queue in middleware.
   * @param:data Data to be pushed to the queue.
   */
void MIDware_Queue_PushData(uint8_t data)
{
    q.queue[q.rear][index] = data; 
    index++;
}
/**
   * @brief:Add a new element to the queue in the middleware.
   */
void MIDware_Queue_Enqueue(void)
{
     if (!MIDware_Queue_Full()) 
     {
         q.rear = (q.rear + 1) % MAX_LINE; 
         q.size++;                        
         index = 0;                      
                                       
     }
}
/**
   * @brief:Returns the pointer to the data at the first element of the queue in the middleware.
   * @return:Pointer to the data at the first element of the queue.
   */
uint8_t *MIDware_Queue_Popdata()
{
     return q.queue[q.front];
}
/**
   * @brief:Removes the first element from the queue in the middleware.
   */
void MIDware_Queue_Dequeue()
{
      if(!MIDware_Queue_Empty())
      {
         q.front = (q.front + 1) % MAX_LINE;
         q.size--;
      }

}
/*******************************************************************************
 * EOF
 ******************************************************************************/
